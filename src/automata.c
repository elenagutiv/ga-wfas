#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../inc/automata.h"
#include "../inc/util.h"
#include "../inc/pcp.h"

extern int MAX_HASH_WORD_SIZE;
extern FILE* fp_times;

extern unsigned long long n_potential_matrixMult;

extern char* type;

int initialization(automata** aut, char* filename, char* filename2, FILE* fp_log, char*** f1, char*** f2){
	if(type == NULL){
		printf("Error: introduce a valid parameter pcp or aut.\n");
		return -1;
	}else if(!strcmp(type, "comparison") || !strcmp(type, "memo") || !strcmp(type, "bf-search")){
		initAutomataFromInput(filename, aut);
		return 0;

	}else if(!strcmp(type, "difference")){
		initAutomataFromMultipleInput(filename, filename2, aut, fp_log);
		return 0;

	}else{
		printf("Error: wrong parameter, try pcp, aut or difference.\n");
		return -1;
	}

}

int initAutomataFromInput(char* filename, automata** aut){

	*aut = (automata*)malloc(sizeof(automata));

	FILE *fp;
    char str[MAX_SIZE_LINE];
    
    int i, j, k, index_init = 0, index_final = 0, n_init = 1, n_final=1, index_column;
    double* final = (double*)malloc(sizeof(double)*MAX_NUMBER_STATES);
    double* init = (double*)malloc(sizeof(double)*MAX_NUMBER_STATES);
    int n_alphabet = 1, index_alphabet = 0;
    char* alphabet = (char*)malloc(sizeof(char)*MAX_ALPHABET_SIZE);
    double*** matrices = (double***)malloc(sizeof(double**)*MAX_ALPHABET_SIZE);
    char delim[] = "[ ]\t\n";
    char* ptr;
    int n_states;

    alphabet[0] = '&';
    n_alphabet++;
    index_alphabet++;
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return -1;
    }
    while (fgets(str, MAX_SIZE_LINE, fp) != NULL){
        if (!strcmp(str, "# q0\n")){
        	fgets(str, MAX_SIZE_LINE, fp);

        	ptr = strtok(str, delim);
			while(ptr != NULL)
			{	
				init[index_init] = strtod(ptr, NULL);
				ptr = strtok(NULL, delim);
				n_init++;
				index_init++;
			}

        	
        }else if(!strcmp(str, "# final\n")){
        	fgets(str, MAX_SIZE_LINE, fp);

        	ptr = strtok(str, delim);
			while(ptr != NULL)
			{	
				final[index_final] = strtod(ptr, NULL);
				ptr = strtok(NULL, delim);
				n_final++;
				index_final++;
			}
			if(n_init == n_final){
				n_states = n_init-1;
				matrices[0] = createIdentityMatrixDouble(n_states);
			}else{
				printf("WFA is not properly defined. Initial and final vector have different dimension.\n");
			}
        	
        }else if(!strncmp(str, "# delta[", 8)){
        	ptr = strtok(str+8, delim);
        	
        	alphabet[index_alphabet] = ptr[0];
        	        	
        	matrices[index_alphabet] = (double **)malloc(n_states*sizeof(double*));
			for(j = 0; j < n_states; j++){
				matrices[index_alphabet][j] = (double *)malloc(n_states*sizeof(double));
				fgets(str, MAX_SIZE_LINE, fp);

				index_column = 0;
	        	ptr = strtok(str, delim);
				while(ptr != NULL)
				{	
					matrices[index_alphabet][j][index_column] = strtod(ptr, NULL);
					ptr = strtok(NULL, delim);
					index_column++;
				}
				if((index_column)!=n_states){
					printf("WFA is not properly defined. The number of columns is different from the number of states.\n");
				}
			}

        	n_alphabet++;
        	index_alphabet++;
        }

    }
    fclose(fp);
    
    alphabet[index_alphabet] = '\0';
    n_alphabet--; // n_alphabet value was the # of symbols including & + 1 due to the while end
    (*aut)->n_states = n_states;
    (*aut)->alphabet = (char*)malloc(sizeof(char)*(n_alphabet+1)); 
    strcpy((*aut)->alphabet, alphabet);

	(*aut)->matrices = (mpq_t***)malloc(strlen((*aut)->alphabet)*sizeof(mpq_t**));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen((*aut)->alphabet); i++){
		(*aut)->matrices[i] = (mpq_t**)malloc((*aut)->n_states*sizeof(mpq_t*));
		for(j = 0; j < (*aut)->n_states; j++){
			(*aut)->matrices[i][j] = (mpq_t*)malloc((*aut)->n_states*sizeof(mpq_t));
			for(k = 0; k < (*aut)->n_states; k++){
				//(*aut)->matrices[i][j][k] = matrices[i][j][k];
				mpq_init((*aut)->matrices[i][j][k]);
				mpq_set_d((*aut)->matrices[i][j][k], matrices[i][j][k]);
			}
		}
	};

	(*aut)->init = (mpq_t*)malloc((*aut)->n_states*sizeof(mpq_t));
	(*aut)->final = (mpq_t*)malloc((*aut)->n_states*sizeof(mpq_t));

	for(i = 0; i < n_states; i++){

		mpq_init((*aut)->init[i]);
		mpq_set_d((*aut)->init[i], init[i]);

		mpq_init((*aut)->final[i]);
		mpq_set_d((*aut)->final[i], final[i]);
	}
	

	for(i = 0; i < strlen((*aut)->alphabet); i++){
		for (j = 0; j < (*aut)->n_states; j++){
			free(matrices[i][j]);
		}
	}
	free(matrices);
	free(alphabet);
	free(init);
	free(final);
	return 0;

};

int initAutomataFromPCPInstance(char* filename, automata** res, FILE* fp_log, char*** f1, char*** f2){

	FILE *fp;
    char str[MAX_SIZE_LINE];
    
    int i,j,k, n_alphabet, index_f1 = 0, index_f2 = 0, n_f1 = 1, n_f2=1, function = 0;
    mpq_t phi_1_i, phi_2_i, power1, power2, one, inverse1, inverse2, sub1, sub2;
    mpq_init(phi_1_i); mpq_init(phi_2_i); mpq_init(power1); mpq_init(power2); mpq_init(one); mpq_init(inverse1); mpq_init(inverse2); mpq_init(sub1); mpq_init(sub2);
    mpq_set_d(one, 1.0);

    *f1 = (char**)malloc(sizeof(char*)*MAX_SIZE_PCP);
    *f2 = (char**)malloc(sizeof(char*)*MAX_SIZE_PCP);

    automata* aut1;
    automata* aut2;
    automata* sum;
    automata* comp;

    for(i = 0; i < MAX_SIZE_PCP; i++){
    	(*f1)[i] = (char*)malloc(sizeof(char)*(MAX_WIDTH_PCP + 1));
    	(*f2)[i] = (char*)malloc(sizeof(char)*(MAX_WIDTH_PCP + 1));
    }

    char delim[] = "[ ]\t\n";
    char* ptr;
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAX_SIZE_LINE, fp) != NULL){

        	ptr = strtok(str, delim);
			while(ptr != NULL)
			{	
				if(function == 0){
					strcpy((*f1)[index_f1], ptr);
					index_f1++;
				}else if(function == 1){
					strcpy((*f2)[index_f2], ptr);
					index_f2++;
				}else{
					printf("PCP instance is not properly defined.\n");
				}
				ptr = strtok(NULL, delim);
				
			}
			function++;   	
    }
    if(index_f1 == index_f2){
    	n_alphabet = index_f1;

    }else{
    	printf("PCP instance is not properly defined.\n");
    }
    	
    fclose(fp);


    char* alphabet = (char*)malloc(sizeof(char)*(n_alphabet+2)); // Allocate memory for '&' and '\0'.

    if(n_alphabet == 2){ //There are only 3 or 4 symbols in the alphabet of my PCP instances
    	strcpy(alphabet, "&ab");
    }else if(n_alphabet == 3){ //There are only 3 or 4 symbols in the alphabet of my PCP instances
    	strcpy(alphabet, "&abc");
    }else if(n_alphabet == 4){
    	strcpy(alphabet, "&abcd");
    }

    aut1 = (automata*)malloc(sizeof(automata));

    aut1->n_states = 2; // Each PCP instance defines 2 automata of 2 states each
    aut1->alphabet = (char*)malloc(sizeof(char)*(n_alphabet+2)); 
    strcpy(aut1->alphabet, alphabet);

	aut1->matrices = (mpq_t ***)malloc(strlen(aut1->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen(aut1->alphabet); i++){
		aut1->matrices[i] = (mpq_t **)malloc(aut1->n_states*sizeof(mpq_t*));
		for(j = 0; j < aut1->n_states; j++){
			aut1->matrices[i][j] = (mpq_t *)malloc(aut1->n_states*sizeof(mpq_t));
			for(k = 0; k < aut1->n_states; k++){
				mpq_init(aut1->matrices[i][j][k]);
			}
		}

	};
	aut1->init = (mpq_t*)malloc(aut1->n_states*sizeof(mpq_t));
	aut1->final = (mpq_t*)malloc(aut1->n_states*sizeof(mpq_t));

	aut2 = (automata*)malloc(sizeof(automata));

    aut2->n_states = 2;
    aut2->alphabet = (char*)malloc(sizeof(char)*(n_alphabet+2)); 
    strcpy(aut2->alphabet, alphabet);

	aut2->matrices = (mpq_t ***)malloc(strlen(aut2->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen(aut2->alphabet); i++){
		aut2->matrices[i] = (mpq_t **)malloc(aut2->n_states*sizeof(mpq_t*));
		for(j = 0; j < aut2->n_states; j++){
			aut2->matrices[i][j] = (mpq_t *)malloc(aut2->n_states*sizeof(mpq_t));
			for(k = 0; k < aut2->n_states; k++){
				mpq_init(aut2->matrices[i][j][k]);
			}
		}
	};
	aut2->init = (mpq_t*)malloc(aut2->n_states*sizeof(mpq_t));
	aut2->final = (mpq_t*)malloc(aut2->n_states*sizeof(mpq_t));


	// Init matrices
	double** identityMatrix;
	identityMatrix = createIdentityMatrixDouble(aut1->n_states);
	for(j = 0; j < aut1->n_states; j++){  // aut1->n_states == aut2->n_states
		for(k = 0; k < aut1->n_states; k++){
			mpq_set_d(aut1->matrices[0][j][k], identityMatrix[j][k]);
			mpq_set_d(aut2->matrices[0][j][k], identityMatrix[j][k]);
		}
	}

	for(i = 0; i < aut1->n_states; i++){
		mpq_init(aut1->init[i]);
		mpq_init(aut2->init[i]);		
		mpq_init(aut1->final[i]);
		mpq_init(aut2->final[i]);

		if(i == 0){
			mpq_set_d(aut1->init[i], 1.0);
			mpq_set_d(aut2->init[i], 1.0);
			mpq_set_d(aut1->final[i], 0.0);
			mpq_set_d(aut2->final[i], 1.0);
		}else{
			mpq_set_d(aut1->init[i], 0.0);
			mpq_set_d(aut2->init[i], 0.0);
			mpq_set_d(aut1->final[i], 1.0);
			mpq_set_d(aut2->final[i], 0.0);
		}
	}

	for(i = 1; i < strlen(aut1->alphabet); i++){ //Skip '&' since it is already initialized
		phi((*f1)[i-1], &phi_1_i);
		phi((*f2)[i-1], &phi_2_i);

		mpq_set_d(power1, (pow(2, 2*(strlen((*f1)[i-1])))));
		mpq_set_d(power2, (pow(2, 2*(strlen((*f2)[i-1])))));

		mpq_div(inverse1, one, power1); // 1/(pow(2, 2*(strlen((*f1)[i-1]))))
		mpq_div(inverse2, one, power2); // 1/(pow(2, 2*(strlen((*f2)[i-1]))))

		mpq_sub(sub1, one, phi_1_i); //1 - phi_1_i
		mpq_sub(sub2, one, phi_2_i); //1 - phi_2_i

		for(j = 0; j < aut1->n_states; j++){
			for(k = 0; k < aut1->n_states; k++){
				if(j == 0 && k == 0){
					// aut1->matrices[i][j][k] = 1 - phi_1_i;
					// aut2->matrices[i][j][k] = 1 - phi_2_i;
					mpq_set(aut1->matrices[i][j][k], sub1);
					mpq_set(aut2->matrices[i][j][k], sub2);
				}else if(j == 0 && k == 1){
					// aut1->matrices[i][j][k] = phi_1_i;
					// aut2->matrices[i][j][k] = phi_2_i;
					mpq_set(aut1->matrices[i][j][k], phi_1_i);
					mpq_set(aut2->matrices[i][j][k], phi_2_i);

				}else if(j == 1 && k == 0){
					// aut1->matrices[i][j][k] = 1 - phi_1_i - 1/(pow(2, (strlen((*f1)[i-1]) +1)));
					// aut2->matrices[i][j][k] = 1 - phi_2_i - 1/(pow(2, (strlen((*f2)[i-1]) + 1)));
					mpq_sub(aut1->matrices[i][j][k], sub1, inverse1);
					mpq_sub(aut2->matrices[i][j][k], sub2, inverse2);

				}else if(j == 1 && k == 1){
					// aut1->matrices[i][j][k] = phi_1_i + 1/(pow(2, (strlen((*f1)[i-1]) + 1)));
					// aut2->matrices[i][j][k] = phi_2_i + 1/(pow(2, (strlen((*f2)[i-1]) + 1)));
					mpq_add(aut1->matrices[i][j][k], phi_1_i, inverse1);
					mpq_add(aut2->matrices[i][j][k], phi_2_i, inverse2);

				}
			}
		}
	}
	mpq_clear(phi_1_i); mpq_clear(phi_2_i); mpq_clear(power1); mpq_clear(power2); mpq_clear(one); mpq_clear(inverse1); mpq_clear(inverse2); mpq_clear(sub1); mpq_clear(sub2);

	escalarByAutomata(0.5, aut1);
	escalarByAutomata(0.5, aut2);
	
	sumAutomata(aut1, aut2, &sum);


	fprintf(fp_log, "\nSum automata\n");
	printAutomata(sum, fp_log);


	complementedCopy(sum, &comp);
	hadamardProduct(sum, comp, res);


    fprintf(fp_log, "Res automata\n");
    printAutomata(*res, fp_log);

    // Solution of PCP3: babbababbabbcbbccbaacbbabcbbaabcbbbacbcbccac
    // Solution of PCP2: caacacabcabcaabbbcabaabbcababbcaaabcacabbcabbabbababbab

    
    for(i = 0; i < aut1->n_states; i++){
    	free(identityMatrix[i]);
    }
    free(identityMatrix);

    freeAutomata(comp);
    freeAutomata(sum);
    freeAutomata(aut1);
    freeAutomata(aut2);

    free(alphabet);
    
    return 0;
}

int initAutomataFromMultipleInput(char* filename, char* filename2, automata** res, FILE* fp_log){

	automata *res1, *res2;
	initAutomataFromInput(filename, &res1);
	initAutomataFromInput(filename2, &res2);

	substractionAutomata(res1, res2, res);

	fprintf(fp_log, "Res automata\n");
    printAutomata(*res, fp_log);

    free(res1);
    free(res2);

    return 0;

}


int phi(char* f, mpq_t* res){

	int i, n;

	char string[2];
	mpq_t strtodf; mpq_t power; mpq_t division; mpq_t one;
	mpq_init(*res); mpq_init(strtodf); mpq_init(power); mpq_init(division); mpq_init(one);

	mpq_set_d(*res, 0.0); mpq_set_d(one, 1.0);

	for(i = (strlen(f) - 1), n = 1; i >= 0 ; i--, n++){
		string[0] = f[i];
		string[1] = '\0';
		mpq_set_d(strtodf, strtod(string, NULL));
		mpq_set_d(power, pow(2,n));
		mpq_div(division, strtodf, power);
		mpq_add(*res, *res, division);
		n++;
		mpq_div(division, one, power);
		mpq_add(*res, *res, division);
	}
	mpq_clear(strtodf); mpq_clear(power); mpq_clear(division); mpq_clear(one);
	return 0;

}


void freeAutomata(automata* aut){

	int i, j, k;

	for(i = 0; i < strlen(aut->alphabet); i++){
		for (j =0; j < aut->n_states; j++){
			for(k = 0; k < aut->n_states; k++){
				mpq_clear(aut->matrices[i][j][k]);
			}
			free(aut->matrices[i][j]);
		}
	}
	free(aut->matrices);

	for(i = 0; i < aut->n_states; i++){
		mpq_clear(aut->init[i]);
		mpq_clear(aut->final[i]);
	}

	free(aut->init);
	free(aut->final);
	free(aut->alphabet);

}


mpq_t* hashedInitVectorMatrixProduct(char* word, automata aut, hashedWord** hashTable){

	int lengthWord = strlen(word);
	int i, j;

	int n_frames = lengthWord/MAX_HASH_WORD_SIZE;
	char* word_frame;
	char* aux_word = strdup(word);
	char* next_ptr;
	mpq_t** hashedWordWeight, *wordWeight, **frameWeight;

	if(lengthWord == 0){
		wordWeight = copyVector(aut.init, aut.n_states);
		free(aux_word);
	}else if(n_frames < 1){
		word_frame = strndup(aux_word, (lengthWord)*sizeof(char));
		hashedWordWeight = find_hashedWord(word_frame, hashTable);
		if(hashedWordWeight == NULL){
			printf("Error in hashedInitVectorMatrixProduct function. \n");
			return NULL;
		}else{
			wordWeight = vectorMatrixMultiplication(aut.init, aut.n_states, hashedWordWeight, aut.n_states, aut.n_states, 0, 0);
		}

		free(word_frame);
		free(aux_word);

	}else{

		word_frame = strndup(aux_word, MAX_HASH_WORD_SIZE*sizeof(char));
		hashedWordWeight = find_hashedWord(word_frame, hashTable);
		if(hashedWordWeight == NULL){
			printf("Error in hashedInitVectorMatrixProduct function. \n");
			return NULL;
		}
		wordWeight = vectorMatrixMultiplication(aut.init, aut.n_states, hashedWordWeight, aut.n_states, aut.n_states, 0, 0);

		next_ptr = aux_word + MAX_HASH_WORD_SIZE;
		free(word_frame);

		for(i = 1; i < n_frames; i++){
			word_frame = strndup(next_ptr, MAX_HASH_WORD_SIZE*sizeof(char));
			hashedWordWeight = find_hashedWord(word_frame,hashTable);
			if(hashedWordWeight == NULL){
				printf("Error in hashedInitVectorMatrixProduct function. \n");
				return NULL;
			}else{
				wordWeight = vectorMatrixMultiplication(wordWeight, aut.n_states, hashedWordWeight, aut.n_states, aut.n_states, 1, 0);
			}
			
			free(word_frame);
			next_ptr = next_ptr + MAX_HASH_WORD_SIZE;
		}
		if((lengthWord - n_frames*MAX_HASH_WORD_SIZE)>0){
			word_frame = strndup(next_ptr, (lengthWord - n_frames*MAX_HASH_WORD_SIZE)*sizeof(char));
			hashedWordWeight = find_hashedWord(word_frame,hashTable);
			if(hashedWordWeight == NULL){
				printf("Error in hashedInitVectorMatrixProduct function. \n");
				return NULL;
			}else{
				wordWeight = vectorMatrixMultiplication(wordWeight, aut.n_states, hashedWordWeight, aut.n_states, aut.n_states, 1, 0);
			}

			free(word_frame);
		}
		free(aux_word);

	}

	return wordWeight;

}

mpq_t* hashedMatrixFinalVectorProduct (char* word, automata aut, hashedWord** hashTable){

	int lengthWord = strlen(word);
	int i, j;

	int n_frames = lengthWord/MAX_HASH_WORD_SIZE;
	char* word_frame;
	char* aux_word = strdup(word);
	char* next_ptr;
	mpq_t** hashedWordWeight, *wordWeight, **frameWeight;

	if(lengthWord == 0){
		wordWeight = copyVector(aut.final, aut.n_states);
		free(aux_word);

	}else if(n_frames < 1){
		word_frame = strndup(aux_word, (lengthWord)*sizeof(char));
		hashedWordWeight = find_hashedWord(word_frame, hashTable);
		if(hashedWordWeight == NULL){
			printf("Error in hashedMatrixFinalVectorProduct function. \n");
			return NULL;
		}else{
			wordWeight = matrixVectorMultiplication(hashedWordWeight, aut.n_states, aut.n_states, aut.final, aut.n_states, 0, 0);
		}

		free(word_frame);
		free(aux_word);

	}else{
		if((lengthWord - n_frames*MAX_HASH_WORD_SIZE) > 0){
			next_ptr = aux_word + n_frames*MAX_HASH_WORD_SIZE;
			word_frame = strndup(next_ptr, (lengthWord - n_frames*MAX_HASH_WORD_SIZE)*sizeof(char));
			hashedWordWeight = find_hashedWord(word_frame, hashTable);
			if(hashedWordWeight == NULL){
				printf("Error in hashedMatrixFinalVectorProduct function. \n");
				return NULL;
			}else{
				wordWeight = matrixVectorMultiplication(hashedWordWeight, aut.n_states, aut.n_states, aut.final, aut.n_states, 0, 0);
			}

			free(word_frame);
		}else{
			wordWeight = copyVector(aut.final, aut.n_states);
		}
		
		for(i = 0; i < n_frames; i++){
			next_ptr = aux_word + (n_frames - (i+1))*MAX_HASH_WORD_SIZE;

			word_frame = strndup(next_ptr, MAX_HASH_WORD_SIZE*sizeof(char));
			
			hashedWordWeight = find_hashedWord(word_frame,hashTable);
			if(hashedWordWeight == NULL){
				printf("Error in hashedMatrixFinalVectorProduct function. \n");
				return NULL;
			}else{
				wordWeight = matrixVectorMultiplication(hashedWordWeight, aut.n_states, aut.n_states, wordWeight, aut.n_states, 0, 1);
			}
			free(word_frame);
		}

		free(aux_word);

	}

	return wordWeight;

}


int weightOfWord(mpq_t* weight, char* word, automata aut, hashedWord** hashTable){
	int i;
	int* list;

	n_potential_matrixMult += strlen(word) + 1;

	if(wordToList(&list, word, aut)==-1){
		return -1;
	}
	if(strcmp(word, "&") == 0){ // We are looking for non-trivial solutions
		mpq_set_d(*weight, 0.0);
		return 0;
	}

	mpq_t* initWordWeight;
	mpq_t _weight;
	mpq_init(_weight);

	initWordWeight = hashedInitVectorMatrixProduct(word, aut, hashTable);

	/*Product of initWordWeight and final*/
	escalarProduct(initWordWeight, aut.n_states, aut.final, aut.n_states, &_weight);
	
	mpq_set(*weight, _weight);

	if(!strcmp(type, "comparison") || !strcmp(type, "difference") ){
		fprintf(fp_times, "%s\t%.25lf\n", word, mpq_get_d(*weight));
	}
	
	mpq_clear(_weight);
	
	for(i = 0; i < aut.n_states; i++){
		mpq_clear(initWordWeight[i]);
	}
	free(initWordWeight);
	free(list);

	return 0;

};

int wordToList(int** alist, char* word, automata aut){

	int i, j;
	int flag_in_alpha;

	*alist = (int*)malloc(strlen(word)*sizeof(int));

	for(i = 0; i < strlen(word); i++){	
		flag_in_alpha = 0;
		for(j = 0; j < strlen(aut.alphabet); j++){
			if (word[i] == aut.alphabet[j]){
				(*alist)[i] = j;
				flag_in_alpha = 1;
				break;
			}
		}
		if(!flag_in_alpha){
			printf("The word %s contains the unknown symbol %c.\n", word, word[i]);
			return -1;
		}
		
	}

	return 0;
}
/* Given a word of numbers where each element i represents the symbols aut.alphabet[i+1] (so we skip &), this function
  returns a word containing the corresponding alphabet symbols */

int listToWord(char* listNumbers, char** listSymbols, automata aut, int base){

 	int lengthList = strlen(listNumbers), i;
 	*listSymbols = (char*)malloc((lengthList+1)*sizeof(char));
 	char* ch = (char*)malloc(sizeof(char)*2); // String of 1 character + '\0'

 	for(i = 0; i < lengthList; i++){
 		ch[0] = listNumbers[i];
 		ch[1] = '\0';
 		(*listSymbols)[i] = aut.alphabet[strtol(ch, NULL, base)+1]; // aut.alphabet starts with &
 	}
 	(*listSymbols)[i] = '\0';
 	return 0;
}


int hadamardProduct (automata* aut1, automata* aut2, automata** res){

	int i,j,k, a1, a2, b1, b2;

	*res = (automata*)malloc(sizeof(automata));

	(*res)->n_states = aut1->n_states*aut2->n_states;
    (*res)->alphabet = (char*)malloc(sizeof(char)*(strlen(aut1->alphabet)+1)); 
    strcpy((*res)->alphabet, aut1->alphabet); // Alphabets of a2 and a2 must be equal

	(*res)->matrices = (mpq_t ***)malloc(strlen((*res)->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen((*res)->alphabet); i++){
		(*res)->matrices[i] = (mpq_t **)malloc((*res)->n_states*sizeof(mpq_t*));
		for(j = 0; j < (*res)->n_states; j++){
			(*res)->matrices[i][j] = (mpq_t *)malloc((*res)->n_states*sizeof(mpq_t));
			for(k = 0; k < (*res)->n_states; k++){
				mpq_init((*res)->matrices[i][j][k]);
			}
		}
	};

	(*res)->init = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));
	(*res)->final = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));

	for(i = 0; i < (*res)->n_states; i++){
		mpq_init((*res)->init[i]);
		mpq_init((*res)->final[i]);
	}

	double** identityMatrix;
	identityMatrix = createIdentityMatrixDouble((*res)->n_states);
	for(j = 0; j < (*res)->n_states; j++){  
		for(k = 0; k < (*res)->n_states; k++){
			mpq_set_d((*res)->matrices[0][j][k], identityMatrix[j][k]);
		}
	}

	for(k = 1; k < strlen((*res)->alphabet); k++){
		for(i = 0; i < (*res)->n_states; i++){
			for(j = 0; j < (*res)->n_states; j++){
				a1 = i/aut2->n_states; // returns floor(i/a2)
				a2 = j/aut2->n_states;

				b1 = i%aut2->n_states;
				b2 = j%aut2->n_states;

				mpq_mul((*res)->matrices[k][i][j], aut1->matrices[k][a1][a2], aut2->matrices[k][b1][b2]);

			}
		}
	}
	
	for(i = 0, j = 0; j < aut1->n_states; j++){
		for(k = 0; k < aut2->n_states; k++, i++){

			mpq_mul((*res)->init[i], aut1->init[j], aut2->init[k]);
			mpq_mul((*res)->final[i], aut1->final[j], aut2->final[k]);
		}
	}

	for(i = 0; i < (*res)->n_states; i++){
    	free(identityMatrix[i]);
    }
    free(identityMatrix);
	
	return 0;

}

int escalarByAutomata(double escalar, automata* aut){

	int i;
	mpq_t mpq_escalar, multiplicand;
	mpq_init(mpq_escalar);
	mpq_set_d(mpq_escalar, escalar);

	mpq_init(multiplicand);

	for(i = 0; i < aut->n_states; i++){
		//aut->init[i] = escalar*(aut->init[i]);
		mpq_set(multiplicand, aut->init[i]);
		mpq_mul(aut->init[i], mpq_escalar, multiplicand);
	}
	return 0;
}

int sumAutomata(automata* aut1, automata* aut2, automata** res){

	int i, j, k, l, m;
	*res = (automata*)malloc(sizeof(automata));

	(*res)->n_states = aut1->n_states+aut2->n_states;
    (*res)->alphabet = (char*)malloc(sizeof(char)*(strlen(aut1->alphabet)+1)); 
    strcpy((*res)->alphabet, aut1->alphabet); // Alphabets of a2 and a2 must be equal

	(*res)->matrices = (mpq_t ***)malloc(strlen((*res)->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen((*res)->alphabet); i++){
		(*res)->matrices[i] = (mpq_t **)malloc((*res)->n_states*sizeof(mpq_t*));
		for(j = 0; j < (*res)->n_states; j++){
			(*res)->matrices[i][j] = (mpq_t *)malloc((*res)->n_states*sizeof(mpq_t));
			for(k = 0; k < (*res)->n_states; k++){
				mpq_init((*res)->matrices[i][j][k]);
			}
		}
	};

	(*res)->init = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));
	(*res)->final = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));

	for(i = 0; i < (*res)->n_states; i++){
		mpq_init((*res)->init[i]);
		mpq_init((*res)->final[i]);
	}

	double** identityMatrix;
	identityMatrix = createIdentityMatrixDouble((*res)->n_states);
	for(j = 0; j < (*res)->n_states; j++){  
		for(k = 0; k < (*res)->n_states; k++){
			mpq_set_d((*res)->matrices[0][j][k], identityMatrix[j][k]);
		}
	}

	for(k = 1; k < strlen((*res)->alphabet); k++){
		for(i = 0; i < (*res)->n_states; i++){
			for(j = 0; j < (*res)->n_states; j++){
				if(i < aut1->n_states){
					if(j < aut1->n_states){
						mpq_set((*res)->matrices[k][i][j], aut1->matrices[k][i][j]);
					}else{
						mpq_set_d((*res)->matrices[k][i][j], 0.0);
					}
				}else{
					if(j < aut1->n_states){
						mpq_set_d((*res)->matrices[k][i][j], 0.0);
					}else{
						l = i - aut1->n_states;
						m = j - aut1->n_states;
						mpq_set((*res)->matrices[k][i][j], aut2->matrices[k][l][m]);
					}

				}

			}
		}
	}
	for(i = 0; i < (*res)->n_states; i++){
		if(i < aut1->n_states){
			mpq_set((*res)->init[i], aut1->init[i]);
			mpq_set((*res)->final[i], aut1->final[i]);
		}else{
			l = i - aut1->n_states;
			mpq_set((*res)->init[i],  aut2->init[l]);
			mpq_set((*res)->final[i], aut2->final[l]);
		}
	}

	for(i = 0; i < (*res)->n_states; i++){
    	free(identityMatrix[i]);
    }
    free(identityMatrix);

	return 0;

}

int substractionAutomata(automata* aut1, automata* aut2, automata** res){
	int i, j, k, l, m;
	*res = (automata*)malloc(sizeof(automata));

	(*res)->n_states = aut1->n_states+aut2->n_states;
    (*res)->alphabet = (char*)malloc(sizeof(char)*(strlen(aut1->alphabet)+1)); 
    strcpy((*res)->alphabet, aut1->alphabet); // Alphabets of a2 and a2 must be equal

	(*res)->matrices = (mpq_t ***)malloc(strlen((*res)->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen((*res)->alphabet); i++){
		(*res)->matrices[i] = (mpq_t **)malloc((*res)->n_states*sizeof(mpq_t*));
		for(j = 0; j < (*res)->n_states; j++){
			(*res)->matrices[i][j] = (mpq_t *)malloc((*res)->n_states*sizeof(mpq_t));
			for(k = 0; k < (*res)->n_states; k++){
				mpq_init((*res)->matrices[i][j][k]);
			}
		}
	};

	(*res)->init = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));
	(*res)->final = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));

	for(i = 0; i < (*res)->n_states; i++){
		mpq_init((*res)->init[i]);
		mpq_init((*res)->final[i]);
	}

	double** identityMatrix;
	identityMatrix = createIdentityMatrixDouble((*res)->n_states);
	for(j = 0; j < (*res)->n_states; j++){  
		for(k = 0; k < (*res)->n_states; k++){
			mpq_set_d((*res)->matrices[0][j][k], identityMatrix[j][k]);
		}
	}

	for(k = 1; k < strlen((*res)->alphabet); k++){
		for(i = 0; i < (*res)->n_states; i++){
			for(j = 0; j < (*res)->n_states; j++){
				if(i < aut1->n_states){
					if(j < aut1->n_states){
						mpq_set((*res)->matrices[k][i][j], aut1->matrices[k][i][j]);
					}else{
						mpq_set_d((*res)->matrices[k][i][j], 0.0);
					}
				}else{
					if(j < aut1->n_states){
						mpq_set_d((*res)->matrices[k][i][j], 0.0);
					}else{
						l = i - aut1->n_states;
						m = j - aut1->n_states;
						mpq_set((*res)->matrices[k][i][j], aut2->matrices[k][l][m]);
					}

				}

			}
		}
	}
	for(i = 0; i < (*res)->n_states; i++){
		if(i < aut1->n_states){
			mpq_set((*res)->init[i], aut1->init[i]);
			mpq_set((*res)->final[i], aut1->final[i]);
		}else{
			l = i - aut1->n_states;
			mpq_neg((*res)->init[i],  aut2->init[l]); // Only the initials of the second operand vary
			mpq_set((*res)->final[i], aut2->final[l]);
		}
	}

	for(i = 0; i < (*res)->n_states; i++){
    	free(identityMatrix[i]);
    }
    free(identityMatrix);

	return 0;

}


int complementedCopy(automata* aut, automata** res){
	int i, j, k;
	*res = (automata*)malloc(sizeof(automata));
	mpq_t one;
	mpq_init(one);
	mpq_set_d(one, 1.0);

	(*res)->n_states = aut->n_states;
    (*res)->alphabet = (char*)malloc(sizeof(char)*(strlen(aut->alphabet)+1)); 
    strcpy((*res)->alphabet, aut->alphabet);

	(*res)->matrices = (mpq_t ***)malloc(strlen((*res)->alphabet)*sizeof(mpq_t **));//As many matrices as the size of the alphabet
	for(i = 0; i < strlen((*res)->alphabet); i++){
		(*res)->matrices[i] = (mpq_t **)malloc((*res)->n_states*sizeof(mpq_t*));
		for(j = 0; j < (*res)->n_states; j++){
			(*res)->matrices[i][j] = (mpq_t *)malloc((*res)->n_states*sizeof(mpq_t));
			for(k = 0; k < (*res)->n_states; k++){
				mpq_init((*res)->matrices[i][j][k]);
			}
		}
	};

	(*res)->init = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));
	(*res)->final = (mpq_t*)malloc((*res)->n_states*sizeof(mpq_t));

	for(i = 0; i < (*res)->n_states; i++){
		mpq_init((*res)->init[i]);
		mpq_init((*res)->final[i]);
	}

	for(i = 0; i <strlen((*res)->alphabet); i++){
		for(j = 0; j < (*res)->n_states; j++){
			for(k = 0; k < (*res)->n_states; k++){
				mpq_set((*res)->matrices[i][j][k], aut->matrices[i][j][k]);
			}
		}
	}
	for(i = 0; i < (*res)->n_states; i++){
		mpq_set((*res)->init[i], aut->init[i]);
		if(mpq_equal(aut->final[i], one) != 0){
			mpq_set_d((*res)->final[i], 0.0);
		}else{ // If aut->final[i] != 1 then aut->final[i] = 0
			mpq_set((*res)->final[i], one);
		}
	}
	return 0;

}

int printAutomata(automata* aut, FILE* fp_log){

	int i, j, k;
	for(i = 0; i < strlen(aut->alphabet); i++){
		fprintf(fp_log,"\ndelta[%c]:\n", aut->alphabet[i]);
		for(j = 0; j < aut->n_states; j++){
			for(k = 0; k < aut->n_states; k++){
				mpq_out_str(fp_log, 10, aut->matrices[i][j][k]);
				fprintf(fp_log, " ");
			}
			fprintf(fp_log,"\n");
		}
		fprintf(fp_log,"\n");
	}
	fprintf(fp_log,"Init:\n");
	for(i = 0; i < aut->n_states; i++){
		mpq_out_str(fp_log, 10, aut->init[i]);
		fprintf(fp_log, " ");

	}
	fprintf(fp_log,"\n");

	fprintf(fp_log,"Final:\n");
	for(i = 0; i < aut->n_states; i++){
		mpq_out_str(fp_log, 10, aut->final[i]);
		fprintf(fp_log, " ");
	}
	fprintf(fp_log,"\n");
	fflush(fp_log);

	return 0;

}






