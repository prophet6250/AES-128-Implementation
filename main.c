/* version 0.1 Changelog:
 *   - two files, main.c and aes.c
 *   - naming conventions according to the specification 
 *   - function size is variable, no 60 line per function limit
 *   - no helper/static functions (unless specified by the paper)
 *   - no weird indentation styles on function naming
 *   - no chunkify and no padding scheme (NULL padding only, bitch)
 *   - official state array & key schedule outputs match to FIPS specification
 *   - takes data of 128-bit, and key of 128-bit as well
 */

#include <stdio.h>
#include <string.h>

void
string_input(uint8_t *input, int size)
{
	int i = 0;
	while (i < size) {
		scanf("%x %x %x %x", &input[i], &input[i + 1], 
				&input[i + 2], &input[i + 3]);
		i += 4;
	}
	
	i = 0;
	while (i < Nb*4) {
		printf("%X%X%X%X", input[i], input[i + 1], 
				input[i + 2], input[i + 3]);
		i += 4;
	}
}


#include "aes.h"

/* Nb = block size (in words)
 * Nk = key size (in words)
 * Nr = number of rounds (absolute value)
 */
int main()
{
	uint8_t input[4 * Nb],
		key[4 * Nk],
		output[4 * Nb];
	uint32_t w[Nb * (Nr + 1)];
	int i, j;

	/* all null padded */
	memset(input, 0, sizeof(input));
	memset(output, 0, sizeof(output));
	memset(w, 0, sizeof(w));
	
	string_input(input, 4*Nb);
	string_input(key, 4*Nk);

	printf("\n");
	return 0;
}
