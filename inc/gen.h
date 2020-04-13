#ifndef GEN_H_   /* Include guard */
#define GEN_H_

#include <gmp.h>

typedef struct{
	char* gens;
	mpq_t fitness;
} individual;

individual initIndividual();

#endif // GEN_H_