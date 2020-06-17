#ifndef CIPHER_H
#define CIPHER_H

/* shorthand macro for mod-4 calculation */
#define MOD4(a) ((a) > 0 ? ((a) & 3) : (4 + ((a) % -4)))
/* constants */
#define Nb 4
#define Nr 10
#define Nk 4

/* some easy to understand typedefs */
typedef unsigned char byte;
typedef unsigned int  word;

/* internal functinon declarations */
void Show(byte [][Nb]);
void StateConversion(byte [], byte [][Nb], int);
void SubBytes(byte [][Nb], int);
void ShiftRows(byte [][Nb], int);
byte MULTIPLY(byte, byte);
void MixColumns(byte [][Nb], int);
void AddRoundKey(byte [][Nb], int, word []);

#endif