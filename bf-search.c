#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


typedef struct{
	int front, last, size;
	unsigned capacity;
	double* weights;
	char** words;
} Queue;

Queue* createQueue(unsigned capacity, int K){
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->capacity = capacity;
	queue->size = 0;
	queue->front = queue->size;
	queue->last = capacity - 1;

	queue->weights = (double*) malloc(queue->capacity * sizeof(double));
	queue->words = (char**) malloc(queue->capacity *sizeof(char*));
	for(int i = 0; i < capacity; i++){
		queue->words[i] = (char*) malloc(sizeof(char)*K);
	}

    return queue;

}

int queueIsFull(Queue* q){
	return(q->capacity == q->size);
}

int queueIsEmpty(Queue* q){
	return(q->size == 0);
}
int dequeue(Queue* queue){
	if(queueIsEmpty(queue)){
		return -1;
	}
	queue->front = (queue->front + 1)%(queue->capacity);
	queue->size = queue->size - 1;
	return 0;
	
}

void enqueue(Queue* queue, double new_weight, char* new_word){
	if(queueIsFull(queue)){
		dequeue(queue);
	}
	queue->last = (queue->last + 1)%(queue->capacity);
	queue->weights[queue->last] = new_weight;
	strcpy(queue->words[queue->last], new_word);
	queue->size = queue->size + 1;
	
}

void printQueue(Queue* queue, FILE* fp_times){
	int i,j;
	for(i = queue->front, j = 0; j < queue->size; j++){
		fprintf(fp_times, "%.25lf\t%s\n",queue->weights[i], queue->words[i]);
		i = (i+1)%queue->capacity;
	}
}

void freeQueue(Queue* queue){
	free(queue->weights);
	for(int i = 0; i < queue->capacity; i++){
		free(queue->words[i]);
	}
	free(queue->words);
}

int main(int argc, char** argv)
{

	char* input = argv[1];
	type = argv[2];
	char* specification;
	if(!strcmp(type, "difference")){
	  specification = argv[7];
	}else{
	  specification = NULL;
	}

	double id = strtod(input + 20, NULL); // The beginning of the floating number rep. is at position 20
    double M = strtod(argv[3], NULL);
    MAX_HASH_WORD_SIZE = strtod(argv[4], NULL);

    K = (int)M;

    Queue* queue = createQueue(100, K);
	

	automata* aut;
	individual* argmax;

	FILE* fp_log;
	fp_log = fopen(LOG_FILE, "w");

	FILE* fp_times;
   	fp_times = (fopen(argv[6], "w"));

	fprintf(fp_log, "******************************\n******************************\n");
	fprintf(fp_log, "Experiment %s\n", argv[1]);

	newIndividual(&argmax);


	if(initialization(&aut, input, specification, fp_log, NULL, NULL) == -1){
	  return -1;
	};

	clock_t start_i, end_i;
	start_i = clock();

	hashedWord* hashTable =  initializeHashTable(*aut, NULL);

	end_i = clock();

	double total_i = ((double) (end_i - start_i)) / CLOCKS_PER_SEC;

	start_algorithm = clock();

	mpq_t weight;
	mpq_init(weight);

	

	int base = strlen(aut->alphabet)-1;
	int i, l, j;
	char* listNumbers = (char*) malloc(sizeof(char)*(K + 1));
	char* string;

	int flag_init = 0;

	double time_execution = (double)(clock() - start_algorithm) / CLOCKS_PER_SEC;

	for(l = 1, j = 0; l <= K; l++){
		for(unsigned long i = 0; i < (unsigned long)pow(base, l); i++, j++){
			itoa (i, listNumbers, base);
			addZeros(listNumbers,l);
			listToWord(listNumbers, &string, *aut, base);

			weightOfWord(&weight, string, *aut, &hashTable);
			
			if(flag_init == 0){
				strcpy(argmax->gens, string);
				mpq_set(argmax->fitness, weight);

				enqueue(queue, mpq_get_d(argmax->fitness), argmax->gens);
				
				flag_init = 1;
			}else{
				if(mpq_cmp(weight, argmax->fitness) > 0){
					strcpy(argmax->gens, string);
					mpq_set(argmax->fitness, weight);

					enqueue(queue, mpq_get_d(argmax->fitness), argmax->gens);

					n_repeated = 0;
				}else{
					n_repeated++;
				}
			}

			free(string);
			
		}
	}

	printf("Time:%.10lf\n", (double)(clock() - start_algorithm)/CLOCKS_PER_SEC);

	free(listNumbers);
	printQueue(queue, fp_times);
	


	fprintf(fp_log, "******************************\n******************************\n");
	fclose(fp_log);
	fclose(fp_times);
	freeIndividual(argmax);
	freeHashTable(hashTable, *aut);
	freeAutomata(aut);
	freeQueue(queue);

	return 0;

}