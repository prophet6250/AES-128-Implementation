#include <stdio.h>
#include <string.h>

#define Nb 4
#define Nk 4
#define Nr 10;

/* shorthand typedefs to make the program more readable :) */
typedef unsigned char byte;
typedef unsigned int  word;

/* function declarations */
extern void CipherEngine(byte [], byte [], byte []);

int main(int argc, char *argv[])
{
	byte input[4 * Nb] = {0x00},
	     key[4 * Nk] = {0x00},
	     output[4 * Nb] = {0x00};

	printf("enter plaintext\n");
	fgets(input, 4*Nb, stdin);
	memset(input + strlen(input) - 1, 0, 4*Nb);
//	printf("\n");
//	fputs(input, stdout);

	printf("enter key (not password)\n");
	fgets(key, 4*Nk, stdin);
	memset(key + strlen(key) - 1, 0, 4*Nk);
//	printf("\n");
//	fputs(key, stdout);

	CipherEngine(input, output, key);
	printf("decrypted data: ");
	fputs(output, stdout);

//	printf("\n");
	return 0;
}
