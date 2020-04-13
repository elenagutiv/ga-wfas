#ifndef MATRIX_H_   /* Include guard */
#define MATRIX_H_

#include <gmp.h>

mpq_t** matrixMultiplication(mpq_t** m1, int cols1, int rows1, mpq_t** m2, int cols2, int rows2, int free1, int free2);
mpq_t* vectorMatrixMultiplication(mpq_t* v, int dimension, mpq_t** m, int cols, int rows, int flag_free1, int flag_free2);
mpq_t* matrixVectorMultiplication(mpq_t** m, int cols, int rows, mpq_t* v, int dimension,  int flag_free1, int flag_free2);
int escalarProduct(mpq_t* v1, int dimension1, mpq_t* v2, int dimension2, mpq_t* res);
mpq_t** createIdentityMatrix(int dimension);
double** createIdentityMatrixDouble(int dimension);
mpq_t** transposeMatrix(mpq_t** m, int rows, int cols);
mpq_t* copyVector(mpq_t* v, int dimension);
mpq_t** copyMatrix(mpq_t** m, int rows, int cols);
void freeMatrix(mpq_t** m, int n_rows, int n_cols);

#endif // MATRIX_H_
