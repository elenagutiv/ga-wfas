#ifndef AUTOMATA_H_   /* Include guard */
#define AUTOMATA_H_

#include "params.h"
#include "matrix.h"
#include "hash-util.h"

typedef struct{
	int n_states;
	char* alphabet;
	mpq_t*** matrices; // List of transition matrices: one per alphabet symbol
	mpq_t* init;
	mpq_t* final;
} automata;

int initialization(automata** aut, char* filename, char* filename2, FILE* fp_log, char*** f1, char*** f2);
int initAutomataFromInput(char* filename, automata** aut);
int initAutomataFromPCPInstance(char* filename, automata** res, FILE* fp_log, char*** f1, char***);
int initAutomataFromMultipleInput(char* filename, char* filename2, automata** res, FILE* fp_log);
int phi(char* f, mpq_t* res);
void freeAutomata(automata* aut);
mpq_t* hashedInitVectorMatrixProduct(char* word, automata aut, hashedWord** hashTable);
mpq_t* hashedMatrixFinalVectorProduct (char* word, automata aut, hashedWord** hashTable);
int weightOfWord(mpq_t* weight, char* word, automata aut, hashedWord** hashTable);
int postOfWord(char* prefix, automata aut, mpq_t** u, hashedWord** hashTable);
int preOfWord(char* suffix, automata aut, mpq_t** v, hashedWord** hashTable);
int compatibility(char* prefix, char* suffix, automata aut, mpq_t* compatibility, hashedWord** hashTable);
int wordToList(int** list, char* word, automata aut);
int listToWord(char* listNumbers, char** listSymbols, automata aut, int base);
int hadamardProduct(automata* a1, automata* a2, automata** res);
int escalarByAutomata(double escalar, automata* aut);
int sumAutomata(automata* aut1, automata* aut2, automata** res);
int substractionAutomata(automata* aut1, automata* aut2, automata** res);
int complementedCopy(automata* aut, automata** res);
int printAutomata(automata* aut, FILE* fp_log);

#endif // AUTOMATA_H_
