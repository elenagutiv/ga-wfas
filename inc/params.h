#ifndef PARAMS_H   /* Include guard */
#define PARAMS_H

#define E_NUMBER 2.71828

#define MAX_SIZE_LINE 1000
#define MAX_ALPHABET_SIZE 100
#define MAX_NUMBER_STATES 100

#define N_REPS_THRESHOLD 10
#define EXTRA_RATE 3

#define POPULATION_SIZE 200 // Even number!
#define CHILDREN_RATE 0.8 //0.5 / Ignored if FLAG_PRUNE_RATE is 0. Note: If FLAG_PRUNE_RATE is 1 and CHILDREN_RATE is 1 then all 
							 // elements in the new population are children.
#define BETA 30.0
#define LAMBDA 0.1 //Lambda value for exponential generator: ~ 1 mutation each 10 symbols. This is the MUTATION_RATE now!
#define CROSSOVER_PROB 1 //0.8
#define MUTATION_PROB 0.1
#define EPSILON_SQR 0.0001
#define MAX_REPEAT 60 // This variable is ignored since the halt variable is assigned to 0 always
#define TIMEOUT 10// In seconds
#define LOG_FILE "../log.txt"

#define FLAG_SELECTION_FPS 1

#define FLAG_PRUNE_NO_REPS 1
#define FLAG_PRUNE_RATE 1

#define FLAG_BOUND_BY_MUT 0
#define FLAG_BOUND_BY_CROSS 0

#endif // PARAMS_H
