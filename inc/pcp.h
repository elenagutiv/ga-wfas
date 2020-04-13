#ifndef PCP_H   /* Include guard */
#define PCP_H

#include "automata.h"

#define FLAG_PCP 1

#define MAX_SIZE_PCP 4
#define MAX_WIDTH_PCP 100

int symbolToIndex(char ch, char* alphabet);
int checkSolForPCP(char** f1, char** f2, char* word, char** word1, char** word2, automata aut);

#endif // PCP_H