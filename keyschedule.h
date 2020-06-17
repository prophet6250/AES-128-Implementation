#ifndef KEYSCHEDULE_H
#define KEYSCHEDULE_H

#define Nb 4
#define Nr 10
#define Nk 4

/* some easy to understand typedefs */
typedef unsigned char byte;
typedef unsigned int  word;

/* function declarations */
void SubWord(word *);
void RotWord(word *);
void KeySchedule(byte [], word []);

#endif