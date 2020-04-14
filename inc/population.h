#ifndef POPULATION_H_   /* Include guard */
#define POPULATION_H_

#include "gen.h"
#include "automata.h"
#include "pcp.h"

typedef struct{
	individual* indivs;
	int generations;
	int size;
} population;

typedef struct{
	double* elems;
	int size;
} roulette;

typedef struct{
	char** elems;
	int size;
}charList;


int initPopulation(population** pop, automata aut, hashedWord** hashTable);
static int newPopulation (population** pop, int size, int generations);
int newIndividual(individual** indiv);
void freeIndividual(individual* indiv);
void freePopulation(population* pop);
int calcFitness(population* pop, automata automata, hashedWord** hashTable);
int naturalSelection(population *pop);
int getIndexRoulette(int pop_size);
int* getIndicesRouletteNoReps(int pop_size, int n);
int* getIndicesRoulette(int pop_size, int n);
int generate(population* oldpop, population** children, automata aut, FILE* fp_log, hashedWord** hashTable);
int crossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable);
int mutate(population* oldpop, automata aut, individual* argmax, hashedWord** hashTable);
int mutateIndividual(individual* child, automata aut, hashedWord** hashTable);
int SNUCrossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable);
int copyCrossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable);
int prune(population** newpop, population* oldpop, population* children, automata aut, FILE* fp_log, hashedWord** hashTable);
int evaluate(population pop, individual* argmax);
static int orderPopulation(population* pop);
static int quicksortPopulation(population* pop, int first,int last);
int maxIndividual(population pop, individual** max);
int mystrcpy(char** str1, char* str2);
int printPopulation(population pop, FILE* fp_log);
int mutateWordRandomWalk(char** word, automata aut, hashedWord** hashTable);
int mutateWordEagerWalk(char** word, mpq_t* weight, automata aut, hashedWord** hashTable);

hashedWord* initializeHashTable(automata aut, population** pop);
void freeHashTable(hashedWord* hashTable, automata aut);
#endif // POPULATION_H_