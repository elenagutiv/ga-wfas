#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "../inc/params.h"

extern int K;

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 * The value of RAND_MAX is granted to be at least 32767
 * Therefore, random numbers that I generate are always smaller than RAND_MAX
 */

int randint(int n) {
	if ((n - 1) == RAND_MAX) {
		return rand(); // Returns a 
	} else {
		// Supporting larger values for n would requires an even more
		// elaborate implementation that combines multiple calls to rand()
		assert (n <= RAND_MAX);

		// Chop off all of the values that would cause skew...
		int end = RAND_MAX / n; // truncate skew
		assert (end > 0);
		end *= n;

		// ... and ignore results from rand() that fall above that limit.
		// (Worst case the loop condition should succeed 50% of the time,
		// so we can expect to bail out of this loop pretty quickly.)
		int r;
		while ((r = rand()) >= end);
		return r % n;
	}
}

// Generates a random word from the alphabet
char* generateRandomWord(char* alphabet){
	int length, i, index;
	char* word;

    length = (randint(K)) + 1; // words of length 1 to K
    word = (char*)malloc((K+1)*sizeof(char)); // words include \0

	if (length == 1){
		index = (randint((strlen(alphabet)-1)))+1; // generate a number between 0 and SIZE_ALPH-1 excluding &!
    	word[0] = alphabet[index];
	}else{

		for(i = 0; i < length; i++){
	    	index = (randint((strlen(alphabet)-1)))+1; // Excludes '&'
	    	word[i] = alphabet[index];
    	}
	}
	word[length] = '\0'; //Add termination of string character

    return word;

}
// Generates a random word from the alphabet of length k
char* generateRandomKWord(char* alphabet, int L){
	int length, i, index;
	char* word;

    length = (randint(L)) + 1; // words of length 1 to L
    word = (char*)malloc((L+1)*sizeof(char)); // words include \0

	if (length == 1){
		index = randint(strlen(alphabet)); // generate a number between 0 and SIZE_ALPH-1
    	word[0] = alphabet[index];
	}else{

		for(i = 0; i < length; i++){
    	index = (randint((strlen(alphabet)-1)))+1; // Excludes '&'
    	word[i] = alphabet[index];
    	}
	}
	word[length] = '\0'; //Add termination of string character

    return word;

}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int addEspecialCharacterToWord(char** word, char* old_word, char ch){

	int length = strlen(old_word);
	int newlength = 2*length + 1;
	int i;
	*word = (char*) malloc(sizeof(char)*(newlength+2));

	for(i = 0; i < newlength; i++){
		if(i%2 == 0){
			(*word)[i] = ch;
		}else{
			(*word)[i] = old_word[i/2];
		}
	}
	(*word)[i] = '\0';

	free(old_word);
	return 0;

}

int removeEspecialCharacterFromWord(char** word, char* old_word, char ch){

	int length = strlen(old_word);
	int i, j, flag_character;

	for(i = 0; i < length; i++){
		if(old_word[i] == ch){
			for(j = i; j < length -1 ; j++){
				old_word[j] = old_word[j+1];
			}
			old_word[j] = '\0';
			i = -1;
			length = strlen(old_word);
		}else if( i == length -1){
			break;
		}
	}
	*word = (char*)malloc((strlen(old_word)+1)*sizeof(char));
	strcpy(*word, old_word);
	free(old_word);
	return 0;

}
int bestRound (double num){

	return (((num - floor(num)) >= 0.5)? ceil(num):floor(num));
}

//TODO: This function should trully swap words
void swapWords(char** w1, char** w2){

	char *aux;

	aux = *w1;
	*w1 = *w2;
	*w2 = aux;

}

int mystrcpy(char** str1, char* str2){

	int length2 = strlen(str2);

	*str1 = (char*)realloc((*str1), sizeof(char)*(length2+1));
	if(*str1 == NULL){
		printf("Something bad happened\n");
		return -1;
	}else{

		strcpy(*str1, str2);
		return 0;
	}
}

/* The code of this function is adapted from: https://www.geeksforgeeks.org/implement-itoa/ */
/* A utility function to reverse a string */
void reverse(char str[], int length){ 
	int start = 0; 
	int end = length -1;
	char aux; 
	while (start < end) 
	{ 
		//swap(*(str+start), *(str+end));
		aux = str[start];
		str[start] = str[end];
		str[end] = aux;
		start++; 
		end--; 
	} 
} 
/* The code of this function is adapted from: https://www.geeksforgeeks.org/implement-itoa/ */
/*Implementation of itoa() */
char* itoa(int num, char* str, int base){ 
	int i = 0; 
	int isNegative = 0; 

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) 
	{ 
		str[i++] = '0'; 
		str[i] = '\0'; 
		return str; 
	} 

	// In standard itoa(), negative numbers are handled only with 
	// base 10. Otherwise numbers are considered unsigned. 
	if (num < 0 && base == 10) 
	{ 
		isNegative = 1; 
		num = -num; 
	} 

	// Process individual digits 
	while (num != 0) 
	{ 
		int rem = num % base; 
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
		num = num/base; 
	} 

	// If number is negative, append '-' 
	if (isNegative) 
		str[i++] = '-'; 

	str[i] = '\0'; // Append string terminator 

	// Reverse the string 
	reverse(str, i); 

	return str; 
}

/*Given a list of numbers listNumbers representing a number n on base given by the size of the alphabet
  and a value of the length of the word to generate l, it completes with as many zeros in the front
  of listNumbers as needed to l*/
int addZeros(char* listNumbers, int l){
	int i;
	int lenghtList = strlen(listNumbers);
	if(lenghtList<l){
		char* aux = (char*)malloc(sizeof(char)*(l+1));
		strcpy(aux, listNumbers);
		strcpy(listNumbers + (l - lenghtList), aux);
		for(i = 0; i < l - lenghtList; i++){
			listNumbers[i] = '0';
		}
		free(aux);
	}
	return 0;
}

int poissonGenerator(double lambda){
	int x = 0, r;
	double p = pow(E_NUMBER, -lambda);
	double s = p;

	r = randint(1001);
	double u = (double)r/(double)1000;
	//printf("u: %lf, s: %lf\n", u,s);
	while(u > s){
		x++;
		p = (p * (double)lambda)/(double)x;
		s = s + p;
	}
	return x;
}

double exponentialGenerator(double lambda){
	int r;
    double u;

    r = randint(1000); // u cannot be 1.0!!
	u = (double)r/(double)1000.0;


    return -log(1-u)/lambda;
}

int removeSymbolInWord(char** word, int index){
	if(index >= strlen(*word)){
		printf("Error removeSymbol: Index out of bounds.\n");
		return -1;
	}
	char* new_word = (char*)malloc(sizeof(char)*strlen(*word)); // Malloc memory for word of length strlen(word) - 1.
	strncpy(new_word, *word, index);
	new_word[index] = '\0';
	strcat(new_word, *word+index+1);

	free(*word);
	*word = new_word;

	return 0;

}

int translateList(int* list, int length, int n){

	int i;

	for(i = 0; i < length; i++){
		list[i] = list[i] + n;
	}

	return 0;
}


int replaceSymbolInWord(char* word, int index, char ch){
	if(index >= strlen(word)){
		printf("Error replaceSymbol: Index out of bounds.\n");
		return -1;
	}
	word[index] = ch;

	return 0; 
}

int addSymbolLeftWord(char** word, int index, char ch){
	if(index > strlen(*word)){
		printf("Error addSymbol: Index out of bounds.\n");
		return -1;
	}

	char* new_word;

	if(strcmp(*word, "&") == 0){  // Special case for greedy-walk. The search starts from "&"
		index = index -1;
		new_word = (char*)malloc(sizeof(char)*(strlen(*word)+1));

	}else{
		new_word = (char*)malloc(sizeof(char)*(strlen(*word)+2)); // Malloc memory for word of length strlen(word) + 1.
		strncpy(new_word, *word, index);
	}

	new_word[index] = ch;
	new_word[index+1] = '\0';
	
	if(strcmp(*word, "&") != 0){
		strcat(new_word, *word+index);
	}
	

	free(*word);
	*word = new_word;

	return 0;

}

