#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../inc/pcp.h"

int symbolToIndex(char ch, char* alphabet){

	int i;

	for(i = 0; i < strlen(alphabet); i++){
		if(ch == alphabet[i]){
			return i-1;  // The first symbol in the alphabet is &
		}
	}
	return -1;
}

int checkSolForPCP(char** f1, char** f2, char* word, char** word1, char** word2, automata aut){
	int length = strlen(word);
	int index, i, j, k, length1, length2, diff, limit;
	*word1 = (char*)malloc(sizeof(char)*(length*MAX_WIDTH_PCP + 1));
	*word2 = (char*)malloc(sizeof(char)*(length*MAX_WIDTH_PCP + 1));

	for(i = 0, k = 0; i < length; i++){
		index = symbolToIndex(word[i], aut.alphabet);
		if(index == -1){ return -1;};
		for(j = 0; j < strlen(f1[index]); j++, k++){
			(*word1)[k] = f1[index][j];
		}
	}
	(*word1)[k] = '\0';


	for(i = 0, k = 0; i < length; i++){
		index = symbolToIndex(word[i], aut.alphabet);
		for(j = 0; j < strlen(f2[index]); j++, k++){
			(*word2)[k] = f2[index][j];
		}
	}
	(*word2)[k] = '\0';

	length1 = strlen(*word1);
	length2 = strlen(*word2);

	diff = (length1 >= length2)? (length1 - length2) : (length2 - length1);
	limit = (length1 >= length2)? length2 : length1;

	for(i = 0; i < limit; i++){
		if((*word1)[i] != (*word2)[i]){
			diff++;
		}
	}
	return diff;

}
