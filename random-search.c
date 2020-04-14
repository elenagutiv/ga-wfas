#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/population.h"
#include "../inc/pcp.h"
#include "../inc/util.h"

clock_t start_algorithm;

int n_repeated = 0;

int MAX_HASH_WORD_SIZE;
int K;
int COMPATIBLE_CROSSOVER_TRIES; // TODO: This variable should be defined in params.h when experimental phase is done
char* type;

FILE* fp_times;

unsigned long long n_matrixMult = 0;
unsigned long long n_potential_matrixMult = 0;
double totalTime_matrixMult = 0.0;

int main(int argc, char** argv)
{
	int i;
	char* input = argv[1];
	type = argv[2];
	char* specification;
	if(!strcmp(type, "difference")){
	  specification = argv[6];
	}else{
	  specification = NULL;
	}

	double id = strtod(input + 20, NULL); // The beginning of the floating number rep. is at position 20
    double M = strtod(argv[3], NULL);
    MAX_HASH_WORD_SIZE = strtod(argv[4], NULL);

    K = (int)M;

	automata* aut;
	individual* argmax;

	FILE* fp_log;
	fp_log = fopen(LOG_FILE, "w");

   	fp_times = (fopen(argv[5], "w"));

	fprintf(fp_log, "******************************\n******************************\n");
	fprintf(fp_log, "Experiment %s\n", argv[1]);

	newIndividual(&argmax);

	srand(time(NULL));

	

	if(initialization(&aut, input, specification, fp_log, NULL, NULL) == -1){
	  return -1;
	};

	clock_t start_i, end_i;
	start_i = clock();

	hashedWord* hashTable =  initializeHashTable(*aut, NULL);

	end_i = clock();

	double total_i = ((double) (end_i - start_i)) / CLOCKS_PER_SEC;

	char* word;
	mpq_t weight;
	mpq_init(weight);
	start_algorithm = clock();

	double time_execution = (double)(clock() - start_algorithm) / CLOCKS_PER_SEC;
	while(time_execution < TIMEOUT){
		word = generateRandomWord(aut->alphabet);
		weightOfWord(&weight, word, *aut, &hashTable);

		if(mpq_cmp(weight, argmax->fitness) > 0){
			strcpy(argmax->gens, word);
			mpq_set(argmax->fitness, weight);
			
			n_repeated = 0;
		}else{
			n_repeated++;
		}
		time_execution = (double)(clock() - start_algorithm) / CLOCKS_PER_SEC;
	}

	// Observed-weight distribution info
	fprintf(fp_times, "%s\t%.25lf\n", argmax->gens, mpq_get_d(argmax->fitness));



	fprintf(fp_log, "******************************\n******************************\n");
	fclose(fp_log);
	fclose(fp_times);
	freeIndividual(argmax);
	freeHashTable(hashTable, *aut);
	freeAutomata(aut);

	return 0;

	}