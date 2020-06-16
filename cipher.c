#include <stdio.h>

#include "constants.h"

/* optimize this later, if it matters so much */
/* 2a mod (x^4 + 1) */
#define MULTIPLY2(x) ((x << 1) % 0x11)
#define MOD4(a) (a > 0 ? (a & 3) : (4 + (a % -4)))

void SubWord(word *w)
{
	byte b[4] = {
		(byte)(*w >> 24), 
		(byte)(*w >> 16), 
		(byte)(*w >>  8), 
		(byte)(*w)
	};

	/* byte substituition */
	b[0] = Sbox[b[0]]; 
	b[1] = Sbox[b[1]]; 
	b[2] = Sbox[b[2]]; 
	b[3] = Sbox[b[3]];

	*w = 0x00000000;
	*w = (word)b[0] << 24 | (word)b[1] << 16 | (word)b[2] << 8 | (word)b[3];
}

void RotWord(word *w)
{
	byte b = *w >> 24;
	*w <<= 8;
	*w |= b;
}

void KeySchedule(byte key[], word w[])
{
	word temp;
	int i = 0;

	/* input */
	while (i < Nk*4) {
		scanf("%x%x%x%x", &key[i], &key[i + 1], &key[i + 2], 
		      &key[i + 3]);

		i += 4;
	}
	
	/* key scheduling */
	i = 0;
	while (i < Nk) {
		w[i] = (word)key[4*i + 0] << 24 | (word)key[4*i + 1] << 16 | 
		       (word)key[4*i + 2] <<  8 | (word)key[4*i + 3];
		i += 1;
	}
	
	/* key expansion */
	i = Nk;
	while (i < Nb * (Nr + 1)) {
		temp = w[i - 1];
		
		if (i % Nk == 0) {
			RotWord(&temp);
			SubWord(&temp);
			temp ^= Rcon[i / Nk];
		}
		else if (Nk > 0 && i % Nk == 4) {
			SubWord(&temp);
		}

		w[i] = w[i - Nk] ^ temp;
		i += 1;
	}
}

void Show(byte state[][Nb]) 
{
	int r, c;
	for (r = 0; r < 4; r += 1) {
		for (c = 0; c < 4; c += 1) {
			printf("state(%d, %d) = %x ", r, c, state[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

void StateConversion(byte buffer[], byte state[][Nb], int flag)
{
	int r, c, bit = 0;
	/* input to state conversion */
	if (flag) {
		for (c = 0; c < Nb; c += 1) {
			for (r = 0; r < 4; r += 1, bit += 1) {
				state[r][c] = buffer[bit];
			}
		}
		Show(state);
	}
	/* state to output conversion */
	else {
		for (c = 0; c < Nb; c += 1) {
			for (r = 0; r < 4; r += 1, bit += 1) {
				buffer[bit] = state[r][c];
			}
			printf("\n");
		}
	}
}

void SubBytes(byte state[][Nb])
{
	int r, c;
	for (c = 0; c < Nb; c += 1) {
		for (r = 0; r < 4; r += 1) {
			state[r][c] = Sbox[state[r][c]];
		}
	}
	printf("after SubBytes\n");
	Show(state);
	printf("\n");
}

void ShiftRows(byte state[][Nb])
{
	int r, c, C;
	byte temp_state[4][Nb];

	/* think how this redundant memory can be removed */
	for (r = 0; r < 4; r += 1) {
		for (c = 0; c < Nb; c += 1) {
			temp_state[r][c] = state[r][c];
		}
	}

	for (r = 1; r < 4; r += 1) {
		for (c = 0; c < Nb; c += 1) {
			/* ideally this should be mod Nb, but here we know Nb is
			   4, so optimization is okay */
			C = MOD4(r + c + Nb);

			state[r][c] = temp_state[r][C];
		}			
	}
	printf("after ShiftRows\n");
	Show(state);
	printf("\n");
}

/* needs some optimizations, if time is on our side */
void MixColumns(byte state[][Nb])
{
	int r;
	byte a, b, c, d;
	for (r = 0; r < 4; r += 1) {
		a = state[r][0], 
		b = state[r][1],
		c = state[r][2],
		d = state[r][3];

		/* 2a + 3b + c + d */
		state[r][0] = MULTIPLY2(a ^ b) ^ b ^ c ^ d;
		/* a + 2b + 3c + d */
		state[r][1] = MULTIPLY2(b ^ c) ^ c ^ d ^ a;
		/* a + b + 2c + 3d */
		state[r][2] = MULTIPLY2(c ^ d) ^ d ^ a ^ b;
		/* 3a + b + c + 2d */
		state[r][3] = MULTIPLY2(d ^ a) ^ a ^ b ^ c;
	}
	printf("after MixColumns\n");
	Show(state);
	printf("\n");
}

void AddRoundKey(byte state[][Nb], int round, word w[])
{
	int r, c, word_index, f = 1;
	
	for (c = 0; c < Nb; c += 1) {
		word_index = (Nb * round) + c;

		state[0][c] ^= (byte)(w[word_index] >> 24);
		state[1][c] ^= (byte)(w[word_index] >> 16);
		state[2][c] ^= (byte)(w[word_index] >> 8);
		state[3][c] ^= (byte)(w[word_index]);
	}
	printf("after AddRoundKey\n");
	Show(state);
	printf("\n");
}

int main()
{
	byte state[4][Nb];
	byte in[4 * Nb], out[4 * Nb], key[4 * Nk];
	word w[Nb * (Nr + 1)];
	int i = 0;

	/* input i/o */
	printf("enter plaintext\n");
	i = 0;
	while (i < Nb * 4) {
		scanf("%x%x%x%x", &in[i], &in[i + 1], &in[i + 2], &in[i + 3]);
		i += 4;
	}
	printf("\n");
	
	printf("enter key\n");
	KeySchedule(key, w);	
	printf("\n");
	
	/* state formation */
	StateConversion(in, state, 1);
	
	/* add initial round key */
	AddRoundKey(state, 0, w);
	for (i = 1; i < Nr - 1; i += 1) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, i, w);
	}

	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, Nr, w);

	StateConversion(out, state, 0);

	for (i = 0; i < Nb; i += 1) {
		printf("%.2x %.2x %.2x %.2x ", out[i], out[i + 1], out[i + 2], 
		        out[i + 2]);
	}
	printf("\n");

	return 0;
}
