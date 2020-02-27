#ifndef HASH_UTIL_H_   /* Include guard */
#define HASH_UTIL_H_

#include "uthash.h"
#include "automata.h"

typedef struct {
    char* word;                 /* key */
    mpq_t** matrix_product;
    UT_hash_handle hh;         /* makes this structure hashable */
} hashedWord;

void add_hashedWord(char* word, mpq_t** matrix_product, hashedWord** hashTable);
mpq_t** find_hashedWord(char* word, hashedWord** hashTable);
#endif // HASH_UTIL_H_