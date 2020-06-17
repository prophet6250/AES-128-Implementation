#include <stdio.h>

#include "constants.h"
#include "keyschedule.h"

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
