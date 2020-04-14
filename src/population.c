#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> // #include <sys/time.h> when executing on Mac OS
#include "../inc/population.h"
#include "../inc/util.h"


extern int n_repeated;

extern int MAX_HASH_WORD_SIZE;

extern int K;

extern FILE* fp_times;

extern char* type;
extern clock_t start_algorithm;


int initPopulation(population** pop, automata aut, hashedWord** hashTable){

	int i;
	char* rand_word;

	newPopulation(pop, POPULATION_SIZE, 0);
	for(i = 0; i < POPULATION_SIZE; i++){

		rand_word = generateRandomWord(aut.alphabet);

		(*pop)->indivs[i].gens = (char *)malloc((strlen(rand_word) + 1)*sizeof(char));
		if((*pop)->indivs[i].gens == NULL){
			return -1;
		}

		strcpy((*pop)->indivs[i].gens, rand_word);

		free(rand_word);

	}
			
	return 0;
		
}
	
static int newPopulation (population** pop, int size, int generations){

	int i;
	*pop = (population*)malloc(sizeof(population));
	(*pop)->indivs = (individual*)malloc(size*sizeof(individual));

	if((*pop)->indivs == NULL){
		return -1;
	}else{

		for(i = 0; i < size; i++){

			mpq_init((*pop)->indivs[i].fitness); // Init to 0.0 since their fitness has not been computed yet
			
		}
		
		(*pop)->size = size;
		(*pop)->generations = generations;
		return 0;
	}
}


int newIndividual(individual** indiv){
	
	(*indiv) = (individual*)malloc(sizeof(individual));
	(*indiv)->gens = (char*) malloc ((K+1)*sizeof(char));

	mpq_init((*indiv)->fitness);
	return 0;
	

}

void freeIndividual(individual* indiv){
	free(indiv->gens);
	mpq_clear(indiv->fitness);
	free(indiv);
}

void freePopulation(population* pop){
	int i;

	for(i = 0; i < pop->size; i++){
		free(pop->indivs[i].gens);
		mpq_clear(pop->indivs[i].fitness);

	}
	free(pop->indivs);
	free(pop);
}


int calcFitness(population* pop, automata aut, int flag_pcp, hashedWord** hashTable){

	int i;
	mpq_t zero;
	mpq_init(zero);
	mpq_set_d(zero, 0.0);
		
	for(i = 0; i < pop->size; i++){
		if(mpq_equal(pop->indivs[i].fitness, zero) != 0){ //If fitness == zero, returns non-zero. If it is unitialized (or fitness is 0.0) we initialize (again)
			weightOfWord(&(pop->indivs[i].fitness), pop->indivs[i].gens, aut, hashTable);
		}	
	}
	mpq_clear(zero);
	return 0;

}

int naturalSelection(population *pop){

	orderPopulation(pop);

	return 0;
	
}

int getIndexRoulette(int pop_size){

	int i, i_rand;
	double d_rand;
	i_rand = randint(1000001); // Generate numbers between 0 and 1000000 included
	d_rand = (double) i_rand/(double) 1000000; // Convert into a number between 0 and 1

	int ip = floor(((double)pop_size)*(log(1 + d_rand * (BETA - 1))/log(BETA)));

	return ip;	
}

int* getIndicesRouletteNoReps(int pop_size, int n){
	int* output = (int*)malloc(n*sizeof(int));
	int length_output = 0;
	int i, j, idx;

	for(i = 0; i < n; i++){
		idx = getIndexRoulette(pop_size-i-1);
		for(j = 0; j < length_output; j++){
			if(idx >= output[j]){
				idx++;
			}
		}
		output[j] = idx;
		length_output++;
		qsort(output, length_output, sizeof(int), cmpfunc);
	}
	return output;
}
int* getIndicesRoulette(int pop_size, int n){
	int* output = (int*)malloc(n*sizeof(int));
	int i;

	for(i = 0; i < n; i++){
		output[i] = getIndexRoulette(pop_size-1);
	}
	return output;
}


int generate(population* oldpop, population** children, automata aut, FILE* fp_log, hashedWord** hashTable){

	int i, indexA, indexB;
	individual parentA, parentB;
	double d_randA, d_randB;

	fprintf(fp_log, "Parents:\n");

	newPopulation(children, POPULATION_SIZE, oldpop->generations);

	for(i = 0; i < POPULATION_SIZE; i+=2){
		
		indexA = getIndexRoulette(oldpop->size-1);
		indexB = getIndexRoulette(oldpop->size-1);

		parentA = oldpop->indivs[indexA];
		parentB = oldpop->indivs[indexB];

		fprintf(fp_log, "%d\t%s\t%.25lf\t", i, parentA.gens, mpq_get_d(parentA.fitness));
		fprintf(fp_log, "\n");
		fprintf(fp_log, "%d\t%s\t%.25lf\t", i, parentB.gens, mpq_get_d(parentB.fitness));
		fprintf(fp_log, "\n");
		fflush(fp_log);

		crossover(parentA, parentB, &((*children)->indivs[i]), &((*children)->indivs[i+1]), aut, hashTable);
	}

	return 0;
}

int prune(population** newpop, population* oldpop, population* children, automata aut, int flag_pcp, FILE* fp_log, hashedWord** hashTable){

	int i, index, j, k;
	int n_children, n_old;

	newPopulation(newpop, POPULATION_SIZE, (oldpop->generations + 1));
	calcFitness(children, aut, flag_pcp, hashTable);

	fprintf(fp_log, "Children:\n");
    printPopulation(*children, fp_log);

	if(FLAG_PRUNE_RATE){

		naturalSelection(children);

		n_children = bestRound(CHILDREN_RATE*POPULATION_SIZE);
		n_old = POPULATION_SIZE - n_children;
		int* indicesChild, *indicesOld;

		if(FLAG_PRUNE_NO_REPS){
			indicesChild = getIndicesRouletteNoReps(POPULATION_SIZE, n_children);
		}else{
			indicesChild = getIndicesRoulette(POPULATION_SIZE, n_children);
		}
		for(i = 0; i < n_children; i++){
			(*newpop)->indivs[i].gens = (char*) malloc((strlen(children->indivs[indicesChild[i]].gens)+1)*sizeof(char));
			strcpy((*newpop)->indivs[i].gens, children->indivs[indicesChild[i]].gens);
			mpq_set((*newpop)->indivs[i].fitness, children->indivs[indicesChild[i]].fitness);			
		}

		naturalSelection(oldpop); // mutation might have changed the right order in old_pop

		if(FLAG_PRUNE_NO_REPS){
			indicesOld = getIndicesRouletteNoReps(POPULATION_SIZE, n_old);
		}else{
			indicesOld = getIndicesRoulette(POPULATION_SIZE, n_old);
		}
		for(j = i, k = 0; j < POPULATION_SIZE; j++, k++){ // This loop repeats n_old times
			(*newpop)->indivs[j].gens = (char*) malloc((strlen(oldpop->indivs[indicesOld[k]].gens)+1)*sizeof(char));
			strcpy((*newpop)->indivs[j].gens, oldpop->indivs[indicesOld[k]].gens);
			mpq_set((*newpop)->indivs[j].fitness, oldpop->indivs[indicesOld[k]].fitness);
		}
		free(indicesChild);
		free(indicesOld);

	}else{
		population* intermediate;
		newPopulation(&intermediate, 2*POPULATION_SIZE, oldpop->generations);

		int* indices;

		for(i = 0; i < POPULATION_SIZE; i++){
			intermediate->indivs[i].gens = (char*) malloc((strlen(oldpop->indivs[i].gens)+1)*sizeof(char));
			strcpy(intermediate->indivs[i].gens, oldpop->indivs[i].gens);
			mpq_set(intermediate->indivs[i].fitness, oldpop->indivs[i].fitness);
		}

		for(i = POPULATION_SIZE, j = 0; i < 2*POPULATION_SIZE; i++, j++){
			intermediate->indivs[i].gens = (char*) malloc((strlen(children->indivs[j].gens)+1)*sizeof(char));
			strcpy(intermediate->indivs[i].gens, children->indivs[j].gens);
			mpq_set(intermediate->indivs[i].fitness, children->indivs[j].fitness);
		}
		naturalSelection(intermediate);

		if(FLAG_PRUNE_NO_REPS){
			indices = getIndicesRouletteNoReps(2*POPULATION_SIZE, POPULATION_SIZE);
		}else{
			indices = getIndicesRoulette(2*POPULATION_SIZE, POPULATION_SIZE);
		}
		
		for(i = 0; i < POPULATION_SIZE; i++){
			(*newpop)->indivs[i].gens = (char*)malloc((strlen(intermediate->indivs[indices[i]].gens)+1)*sizeof(char));
			strcpy((*newpop)->indivs[i].gens, intermediate->indivs[indices[i]].gens);
			mpq_set((*newpop)->indivs[i].fitness, intermediate->indivs[indices[i]].fitness);			
		}
		freePopulation(intermediate);
		free(indices);

	}
	freePopulation(oldpop);
	freePopulation(children);

	return 0;

}

int crossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable){
	// Single-point crossover: Choose one crossover-point randomly at each parent A and B. 
	// Then form child A with the left part of parent A concatenated with the right part of parent B.
	// And form child B with the left part of parent B concatenated with the right part of parent A.

	int random = randint(1001); // Generate numbers between 0 and 1000 included
	float pc = (float) random/(float) 1000; // Convert into a number between 0 and 1
		
	if(pc <= CROSSOVER_PROB && (strlen(parentA.gens) > 0 &&  strlen(parentB.gens) > 0)){
	
		SNUCrossover(parentA, parentB, childA, childB, aut, hashTable);			

	}else{

		copyCrossover(parentA, parentB, childA, childB, aut, hashTable);			
		
	}
	return 0;
}

int mutate(population* oldpop, automata aut, individual* argmax, hashedWord** hashTable){

	int i;
	for(i = 0; i < POPULATION_SIZE; i++){
		if(strcmp(oldpop->indivs[i].gens, argmax->gens) != 0){ // Argmax is not mutated!
			mutateIndividual(&(oldpop->indivs[i]), aut, hashTable);
		}
	}
	return 0;
}


int mutateIndividual(individual* indiv, automata aut, hashedWord** hashTable){

	int extra;
	double pm;
	int r = randint(1001); // Generate numbers between 0 and 1000 included
	pm = (double)r/(double)1000; // Convert into a number between 0 and 1

	extra = (n_repeated > N_REPS_THRESHOLD)? EXTRA_RATE : 1;
	if(pm < (MUTATION_PROB*extra)){
		int length = strlen(indiv->gens);
		int i, j, n_mutations, index, new_symbol_index, random, flag_mutation = 0;
		int n, lengthList;
		double ptm, ptm_del, ptm_repl, ptm_add;
		int* indexes = (int*)malloc(sizeof(int)*length); // indices to be changed in indiv->gens
		

		i = 0, index = 0;
		n = exponentialGenerator(LAMBDA);
		index = index + n;
		while(index < length){
			indexes[i] = index;
			
			i++;
			index = index + 1;

			n = exponentialGenerator(LAMBDA);
			index = index + n;
			
		}


		lengthList = i; // i has a value one unit greater than the higher valid index in indexes because of the for condition. lenghtList is an upper bound of the # mutations to be done

		if(lengthList + length > K){ //no addition if chances are that the length increases beyond K
			if(strlen(aut.alphabet) > 2){ 
				ptm_del = (double)1/(double)3;
				ptm_repl = (double)2/(double)3;
				ptm_add = (double)2/(double)3;;
			}else{ // Only deletion is possible if the alphabet is unary and length K can be overflowed
				ptm_del = 1.0;
				ptm_repl = 1.0;
				ptm_add = 1.0;
			}
		}else{
			if(strlen(aut.alphabet) > 2){
				ptm_del = (double)1/(double)3;
				ptm_repl = (double)2/(double)3;
				ptm_add = 1.0;
			}else{ //no replacement if the alphabet is unary
				ptm_del = (double)1/(double)2;
				ptm_repl =(double)1/(double)2;
				ptm_add = 1.0;
			}
		}

		for(j = 0; j < lengthList; j++){
			if(indexes[j] <= length){
				flag_mutation = 1;

				random = randint(1001); // Generate numbers between 0 and 1000 included
				ptm = (double)random/(double)1000; // Convert into a number between 0 and 1

				if(ptm < ptm_del && indexes[j] < length && length > 1){
					removeSymbolInWord(&indiv->gens, indexes[j]);
					length = length - 1;
					translateList(indexes, lengthList, -1);

				}else if (ptm >= ptm_del && ptm < ptm_repl && indexes[j] < length){
					new_symbol_index = 1 + randint(strlen(aut.alphabet)-1); // generate a number between 0 and SIZE_ALPH-2 and then add 1 to avoid '&'

					while(indiv->gens[i] == aut.alphabet[new_symbol_index]){
						new_symbol_index = 1 + randint(strlen(aut.alphabet)-1); // generate a number between 0 and SIZE_ALPH-2 and then add 1 to avoid '&'
					}
					replaceSymbolInWord(indiv->gens, indexes[j], aut.alphabet[new_symbol_index]);
				}else if(ptm >= ptm_repl && ptm < ptm_add){
					new_symbol_index = 1 + randint(strlen(aut.alphabet)-1); // generate a number between 0 and SIZE_ALPH-2 and then add 1 to avoid '&'
					addSymbolLeftWord(&indiv->gens, indexes[j], aut.alphabet[new_symbol_index]);
					length = length + 1;
					translateList(indexes, lengthList, 1);
				}
			}
		}

		//If individual has mutated then we recompute its fitness
		if(flag_mutation > 0){
			weightOfWord(&(indiv->fitness), indiv->gens, aut, hashTable);
		}

		free(indexes);
	}
	
	return 0;

}


int SNUCrossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable){
	
	int indexA, indexB;
	int i, j, k;
	int lengthA, lengthB;

	indexA = randint(strlen(parentA.gens));
	indexB  =randint(strlen(parentB.gens));

	lengthA = ((indexA + 1) + (strlen(parentB.gens) - indexB -1) <= K)? ((indexA + 1) + (strlen(parentB.gens) - indexB -1)) : K; // does not include '\0'
	lengthB = ((indexB + 1) + (strlen(parentA.gens) - indexA -1) <= K)? ((indexB + 1) + (strlen(parentA.gens) - indexA -1)) : K;

	childA->gens = (char*)malloc((lengthA+1)*sizeof(char));
	childB->gens = (char*)malloc((lengthB+1)*sizeof(char));

	for(i = 0; i <= indexA; i++){
		childA->gens[i] = parentA.gens[i];
	}
	for(j = i, k = indexB+1; j < lengthA && k < strlen(parentB.gens); j++, k++){
		childA->gens[j] = parentB.gens[k];
	}
	childA->gens[j] = '\0';
	
	for(i = 0; i <= indexB; i++){
		childB->gens[i] = parentB.gens[i];
	}
	for(j = i, k = indexA+1; j < lengthB && k < strlen(parentA.gens); j++, k++){
		childB->gens[j] = parentA.gens[k];
	}
	childB->gens[j] = '\0';

	weightOfWord(&(childA->fitness), childA->gens, aut, hashTable);
	weightOfWord(&(childB->fitness), childB->gens, aut, hashTable);

	return 0;

}

int copyCrossover(individual parentA, individual parentB, individual* childA, individual* childB, automata aut, hashedWord** hashTable){
	
	childA->gens = (char*)malloc((strlen(parentA.gens)+1)*sizeof(char));
	childB->gens = (char*)malloc((strlen(parentB.gens)+1)*sizeof(char));

	strcpy(childA->gens, parentA.gens);
	mpq_set(childA->fitness, parentA.fitness);

	strcpy(childB->gens, parentB.gens);
	mpq_set(childB->fitness, parentB.fitness);

	return 0;
}

/* n_repeated counts the number of consecutive repetitions of the same argmax*/
int evaluate(population pop, individual* argmax){

	individual* max;

	maxIndividual(pop, &max);

	if(pop.generations <= 0 || (mpq_cmp(argmax->fitness, max->fitness) < 0)){
		strcpy(argmax->gens, max->gens);
		mpq_set(argmax->fitness, max->fitness);
		
		if(!strcmp(type, "bf-search")){
			fprintf(fp_times, "%.10lf\t%.25lf\t%s\n", (double)(clock() - start_algorithm)/CLOCKS_PER_SEC, mpq_get_d(argmax->fitness), argmax->gens);
		}
		
		n_repeated = 0; // (Initialize) Reset counter

	}else{
		(n_repeated)++;
	}
	freeIndividual(max);
	if(n_repeated >= MAX_REPEAT){
	
		return 1;

	}else{

		return 0;
	}

}
int maxIndividual(population pop, individual** max){

	int i;

	newIndividual(max);

	strcpy((*max)->gens, pop.indivs[0].gens);
	mpq_set((*max)->fitness,  pop.indivs[0].fitness);

	for (i = 1; i < pop.size; i++ ){
		if ((mpq_cmp ((*max)->fitness, pop.indivs[i].fitness)) < 0){
			mpq_set((*max)->fitness, pop.indivs[i].fitness);
			strcpy((*max)->gens, pop.indivs[i].gens);
		}
	}
	return 0;
	
	
}


static int orderPopulation(population* pop){

	quicksortPopulation(pop, 0, pop->size - 1);
	return 1;

}

static int quicksortPopulation(population* pop, int first, int last){
	int i, j, pivot; 

	if(first<last){
		pivot=first;
		i=first;
		j=last;

		while(i<j){
			while((mpq_cmp(pop->indivs[i].fitness, pop->indivs[pivot].fitness) <= 0) && i<last)
				i++;
			while((mpq_cmp(pop->indivs[j].fitness, pop->indivs[pivot].fitness) > 0))
				j--;
			if(i<j){
				swapWords(&(pop->indivs[i].gens), &(pop->indivs[j].gens));
				mpq_swap (pop->indivs[i].fitness, pop->indivs[j].fitness);
			}
		}

		swapWords(&(pop->indivs[pivot].gens), &(pop->indivs[j].gens));

		mpq_swap(pop->indivs[pivot].fitness, pop->indivs[j].fitness);

		quicksortPopulation(pop,first,j-1);
		quicksortPopulation(pop,j+1,last);

		return 0;		
	}
	return 0;  
}


int printPopulation(population pop, FILE* fp_log){
	int i;
	
	for(i = 0; i < pop.size; i++){
		fprintf(fp_log, "%d\t%s\t%.25lf\t", i, pop.indivs[i].gens, mpq_get_d(pop.indivs[i].fitness));
		//mpq_out_str(fp_log, 10, pop.indivs[i].fitness);
		fprintf(fp_log, "\n");
	}
	fflush(fp_log);
	return 0;
}

int mutateWordRandomWalk(char** word, automata aut, hashedWord** hashTable){

	int length = strlen(*word);
	int random, new_symbol_index;
	double pdel, padd, ptm;

	if(length == K){ // if length has reached the maximum value no insertion is allowed.
		pdel = 1.0;
		padd = 0;
	}else if(length == 0){ //if length is 0 only add is allowed
		pdel = 0.0;
		padd = 1.0;
	}else{
		pdel = 0.5;
		padd = 1.0;
	}

	random = randint(1001); // Generate numbers between 0 and 1000 included
	ptm = (double)random/(double)1000; // Convert into a number between 0 and 1

	if(ptm < pdel){
		removeSymbolInWord(word, length-1);

	}else if(ptm >= pdel && ptm < padd){
		new_symbol_index = 1 + randint(strlen(aut.alphabet)-1); // generate a number between 0 and SIZE_ALPH-2 and then add 1 to avoid '&'
		addSymbolLeftWord(word, length, aut.alphabet[new_symbol_index]);
	}

	return 0;

}

int mutateWordEagerWalk(char** word, mpq_t* weight, automata aut, hashedWord** hashTable){

	int length = strlen(*word);
	int flag_mut_init = 0;
	int i;
	mpq_t _weight;
	mpq_init(_weight);
	char* mutated_word = (char*)malloc(sizeof(char)*(length + 1) + 1);// Length of mutated_word is at most length + 1.
	char* aux_word = (char*)malloc(sizeof(char)*(length + 1) + 1);// Length of aux_word is at most length + 1.
	individual* mutantmax;

	newIndividual(&mutantmax);

	//Insertion of all possible symbols in the alphabet
	for(i = 1; i < strlen(aut.alphabet); i++){ // Add 1 to avoid '&'
		strcpy(aux_word, *word);
		addSymbolLeftWord(&aux_word, length, aut.alphabet[i]);
		weightOfWord(&_weight, aux_word, aut, hashTable);

		if(flag_mut_init == 0){
			strcpy(mutantmax->gens, aux_word);
			mpq_set(mutantmax->fitness, _weight);
			flag_mut_init = 1;
		}else{
			if(mpq_cmp(_weight, mutantmax->fitness) > 0){
				strcpy(mutantmax->gens, aux_word);
				mpq_set(mutantmax->fitness, _weight);				
			}
		}
	}

	

	strcpy(mutated_word, mutantmax->gens);
	free(*word);

	*word = mutated_word;


	mpq_set(*weight, mutantmax->fitness);
	mpq_clear(_weight);

	free(aux_word);

	return 0;

}

// Hash-related automata functions:

hashedWord* initializeHashTable(automata aut, population** pop){

	hashedWord* hashTable = NULL;

	int base = strlen(aut.alphabet)-1;
	int i, l, j;
	char* listNumbers = (char*) malloc(sizeof(char)*(MAX_HASH_WORD_SIZE + 1));
	char* string, *substring;
	mpq_t** matrix1, **product_matrix;
	int* list;
	double sample_size;
	population* aux_pop;

	for(l = 1, j = 0; l <= MAX_HASH_WORD_SIZE; l++){
		for(i = 0; i < (int)pow(base, l) ; i++, j++){
			itoa (i, listNumbers, base);
			addZeros(listNumbers,l);
			listToWord(listNumbers, &string, aut, base);
			
			if(wordToList(&list, string, aut)==-1){
				return NULL;
			}
			if(strlen(string) == 1){
				matrix1 = copyMatrix(aut.matrices[list[0]],aut.n_states, aut.n_states);
				add_hashedWord(string, matrix1, &hashTable);
			}else{
				substring = strndup(string, (strlen(string) - 1)*sizeof(char));
				matrix1 = find_hashedWord(substring, &hashTable);
				product_matrix = matrixMultiplication(matrix1, aut.n_states, aut.n_states, aut.matrices[list[strlen(string)-1]], aut.n_states, aut.n_states, 0, 0);
				add_hashedWord(string, product_matrix, &hashTable);

				free(substring);
			}
			free(string);
			free(list);
		}
	}
	free(listNumbers);
	return hashTable;
}

void freeHashTable(hashedWord* hashTable, automata aut){

	hashedWord *s, *tmp;
    HASH_ITER(hh, hashTable, s, tmp) {
      HASH_DEL(hashTable, s);
      free(s->word);
      freeMatrix(s->matrix_product, aut.n_states, aut.n_states);
      free(s);
    }
    
}
