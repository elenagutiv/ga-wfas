#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../inc/automata.h"


void add_hashedWord(char* word, mpq_t** matrix_product, hashedWord** hashTable) {
    hashedWord *h;

    h = malloc(sizeof(hashedWord));
    h->word = strdup(word);
    h->matrix_product = matrix_product;

    HASH_ADD_KEYPTR(hh, *hashTable, h->word, strlen(h->word), h);  /* id: name of key field */

}

mpq_t** find_hashedWord(char* word, hashedWord** hashTable){
	hashedWord *h;
	HASH_FIND_STR(*hashTable, word, h);

	return h->matrix_product;

}

