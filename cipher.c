#include <stdio.h>

#include "constants.h"

/* shorthand macro for mod-4 calculation */
#define MOD4(a) ((a) > 0 ? ((a) & 3) : (4 + ((a) % -4)))

/* extern function declarations */
extern void KeySchedule(byte [], word []);

/* internal functinon declarations */
void Show(byte [][Nb]);
void StateConversion(byte [], byte [][Nb], int);
void SubBytes(byte [][Nb], int);
void ShiftRows(byte [][nb], int);
byte MULTIPLY(byte, byte);
void MixColumns(byte [][Nb], int);
void AddRoundKey(byte [][Nb]);

/******************************* CIPHER ENGINE *******************************/
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

void SubBytes(byte state[][Nb], int inverse_mode)
{
	int r, c;
	for (c = 0; c < Nb; c += 1) {
		for (r = 0; r < 4; r += 1) {
			if (!inverse_mode) 
				state[r][c] = Sbox[state[r][c]];
			
			else 
				state[r][c] = InvSbox[state[r][c]];
			
		}
	}
	printf("after SubBytes\n");
	Show(state);
	printf("\n");
}

void ShiftRows(byte state[][Nb], int inverse_mode)
{
	int r, c, C;
	byte column[Nb];

	for (r = 1; r < 4; r += 1) {
		for (C = 0; C < Nb; C += 1) {
			column[C] = state[r][C];
		}
		for (c = 0; c < Nb; c += 1) {
			C = 0x00;
			/* ideally this should be mod Nb, but here we know Nb is
			   4, so optimization is okay */
			if (!inverse_mode)
				C = MOD4(r + c + Nb);
			else
				C = MOD4(c + Nb - r);

			state[r][c] = column[C];
		}			
	}
	printf("after ShiftRows\n");
	Show(state);
	printf("\n");
}

/* stolen from https://tinyurl.com/gf-multiplication */
byte MULTIPLY(byte a, byte b)
{
	byte product = 0x00, 
	     counter, 
	     carry;
	
	/* this is a trivial implementation of russian peasnat multiplication
	 * method: http://mathforum.org/dr.math/faq/faq.peasant.html */
	for (counter = 0; counter < 8; counter += 1) {
		if (b & 1) 
			product ^= a;
		carry = a & 0x80;
		a <<= 1;
		
		if (carry)
			a ^= 0x1b;
		b >>= 1;
	}
	return product;
}

/* needs some optimizations, if time is on our side */
void MixColumns(byte state[][Nb], int inverse_mode)
{
	int col;
	byte a, b, c, d;

	for (col = 0; col < 4; col += 1) {
		a = state[0][col], 
		b = state[1][col],
		c = state[2][col],
		d = state[3][col];

		/* see the wiki page on MixColumns() to see why this works */
		if (!inverse_mode) {
			state[0][col] = MULTIPLY(0x02, a) ^ MULTIPLY(0x03, b) ^
			                c ^ d;
			state[1][col] = MULTIPLY(0x03, c) ^ MULTIPLY(0x02, b) ^ 
			                a ^ d;
			state[2][col] = MULTIPLY(0x02, c) ^ MULTIPLY(0x03, d) ^
			                a ^ b;
			state[3][col] = MULTIPLY(0x03, a) ^ MULTIPLY(0x02, d) ^
			                b ^ c;
		}
		else {
			state[0][col] = MULTIPLY(0x02, a) ^ MULTIPLY(0x03, b) ^
			                c ^ d;
			state[1][col] = MULTIPLY(0x03, c) ^ MULTIPLY(0x02, b) ^ 
			                a ^ d;
			state[2][col] = MULTIPLY(0x02, c) ^ MULTIPLY(0x03, d) ^
			                a ^ b;
			state[3][col] = MULTIPLY(0x03, a) ^ MULTIPLY(0x02, d) ^
			                b ^ c;	
		}
		
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
	
	printf("enter key\n");
	KeySchedule(key, w);	
	
	/* state formation */
	StateConversion(in, state, 1);
	
	/* add initial round key */
	printf("ROUND 0\n");
	AddRoundKey(state, 0, w);
	for (i = 1; i < Nr; i += 1) {
		printf("ROUND %d\n", i);
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, i, w);
	}

	printf("ROUND 10\n");
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, Nr, w);

	StateConversion(out, state, 0);

	for (i = 0; i < Nb; i += 1) {
		printf("%.2x %.2x %.2x %.2x\n", out[i], out[i + 1], out[i + 2], 
		        out[i + 2]);
	}
	printf("\n");

	return 0;
}
