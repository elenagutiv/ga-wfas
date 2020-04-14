#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/matrix.h"

extern unsigned long long n_matrixMult;
extern double totalTime_matrixMult;

mpq_t** matrixMultiplication(mpq_t** m1, int cols1, int rows1, mpq_t** m2, int cols2, int rows2, int flag_free1, int flag_free2){

	n_matrixMult++;

	clock_t start_matrixMult;
	clock_t end_matrixMult;

	mpq_t** res;
	int i, j, k;
	mpq_t acc, accp, product;
	mpq_init(acc);
	mpq_init(accp);
	mpq_init(product);

	if (cols1!= rows2) return NULL;

	res = (mpq_t**)calloc(rows1, sizeof(mpq_t*));
	for(i = 0; i < rows1; i++){
		res[i] = (mpq_t*)calloc(cols2, sizeof(mpq_t));
		for(j = 0; j < cols2; j++){
			mpq_init(res[i][j]);
		}
	}

	start_matrixMult = clock();
	for(i = 0; i < rows1; i++){
		for(j = 0; j < cols2; j++){
			mpq_set_d(acc, 0.0);
			for(k = 0; k < cols1; k++){
				mpq_mul(product, m1[i][k], m2[k][j]);
				mpq_add (accp, acc, product);
				mpq_set (acc, accp);
			}
			mpq_set (res[i][j], acc);
		}
	}
	end_matrixMult = clock();

	mpq_clear (acc);
	mpq_clear (accp);
	mpq_clear (product);

	if(flag_free1){
		freeMatrix(m1, rows1, cols1);
	}if(flag_free2){
		freeMatrix(m2, rows2, cols2);
	}

	totalTime_matrixMult += ((double) (end_matrixMult - start_matrixMult)) / CLOCKS_PER_SEC;
	return res;

};

mpq_t* vectorMatrixMultiplication(mpq_t* v, int dimension, mpq_t** m, int cols, int rows, int flag_free1, int flag_free2){

	n_matrixMult++;
	clock_t start_matrixMult;
	clock_t end_matrixMult;

	mpq_t* res;
	int i, j;
	mpq_t acc, accp, product;
	mpq_init(acc);
	mpq_init(accp);
	mpq_init(product);

	if (dimension!= rows) return NULL;

	res = (mpq_t*)calloc(cols, sizeof(mpq_t));
	for(i = 0; i < cols; i++){
		mpq_init(res[i]);
	}

	start_matrixMult = clock();
	for(i = 0; i < cols; i++){
		mpq_set_d(acc, 0.0);
		for(j = 0; j < dimension; j++){
			mpq_mul(product, v[j], m[j][i]);
			mpq_add(accp, acc, product);
			mpq_set(acc, accp);

		}
		mpq_set(res[i], acc);
		
	}
	end_matrixMult = clock();

	mpq_clear (acc);
	mpq_clear (accp);
	mpq_clear (product);

	if(flag_free1){
		for(i = 0; i < dimension; i++){
			mpq_clear(v[i]);
		}
		free(v);
	}
	if(flag_free2){
		freeMatrix(m, rows, cols);
	}

	totalTime_matrixMult += ((double) (end_matrixMult - start_matrixMult)) / CLOCKS_PER_SEC;
	return res;

};
mpq_t* matrixVectorMultiplication(mpq_t** m, int rows, int cols, mpq_t* v, int dimension,  int flag_free1, int flag_free2){

	n_matrixMult++;
	clock_t start_matrixMult;
	clock_t end_matrixMult;

	mpq_t* res;
	int i, j;
	mpq_t acc, accp, product;
	mpq_init(acc);
	mpq_init(accp);
	mpq_init(product);

	if (dimension!= cols) return NULL;

	res = (mpq_t*)calloc(cols, sizeof(mpq_t));
	for(i = 0; i < cols; i++){
		mpq_init(res[i]);
	}

	start_matrixMult = clock();
	for(i = 0; i < rows; i++){
		mpq_set_d(acc, 0.0);
		for(j = 0; j < dimension; j++){
			mpq_mul(product, m[i][j], v[j]);
			mpq_add(accp, acc, product);
			mpq_set(acc, accp);

		}
		mpq_set(res[i], acc);
		
	}
	end_matrixMult = clock();

	mpq_clear (acc);
	mpq_clear (accp);
	mpq_clear (product);

	if(flag_free1){
		freeMatrix(m, rows, cols);
	}

	if(flag_free2){
		for(i = 0; i < dimension; i++){
			mpq_clear(v[i]);
		}
		free(v);
	}

	totalTime_matrixMult += ((double) (end_matrixMult - start_matrixMult)) / CLOCKS_PER_SEC;
	return res;

};

int escalarProduct(mpq_t* v1, int dimension1, mpq_t* v2, int dimension2, mpq_t* res){

	n_matrixMult++;
	clock_t start_matrixMult;
	clock_t end_matrixMult;

	int i, j;
	mpq_t acc, accp, product;
	mpq_init(acc);
	mpq_set_d(acc, 0.0);
	mpq_init(accp);
	mpq_init(product);

	start_matrixMult = clock();
	for(i = 0; i < dimension1; i++){
		//acc += v1[i]*v2[i];
		mpq_mul(product, v1[i], v2[i]);
		mpq_add(accp, acc, product);
		mpq_set(acc, accp);

	}
	end_matrixMult = clock();

	mpq_set(*res, acc);
	mpq_clear(acc);
	mpq_clear (accp);
	mpq_clear (product);

	totalTime_matrixMult += ((double) (end_matrixMult - start_matrixMult)) / CLOCKS_PER_SEC;
	return 1;

};


mpq_t** createIdentityMatrix(int dimension){

	mpq_t** identityMatrix;
	int i,j;

	identityMatrix = (mpq_t**)calloc(dimension, sizeof(mpq_t*));
	for(i = 0; i < dimension; i++){
		identityMatrix[i] = (mpq_t*)calloc(dimension, sizeof(mpq_t));
		for(j = 0; j < dimension; j++){
			mpq_init(identityMatrix[i][j]);
		}
	}

	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			if (i == j){
				mpq_set_d(identityMatrix[i][j], 1.0);
			}else{
				mpq_set_d(identityMatrix[i][j], 0.0);
			}
		}
	}

	return identityMatrix;


};

double** createIdentityMatrixDouble(int dimension){

	double** identityMatrix;
	int i,j;

	identityMatrix = (double**)calloc(dimension, sizeof(double*));
	for(i = 0; i < dimension; i++){
		identityMatrix[i] = (double*)calloc(dimension, sizeof(double));
	}

	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			if (i == j){
				identityMatrix[i][j] = 1.0;
			}else{
				identityMatrix[i][j] = 0.0;
			}
		}
	}

	return identityMatrix;


};

mpq_t** transposeMatrix(mpq_t** m, int rows, int cols){

	int i, j;
	mpq_t** res;

	res = (mpq_t**)calloc(cols, sizeof(mpq_t*));
	for(i = 0; i < cols; i++){
		res[i] = (mpq_t*)calloc(rows, sizeof(mpq_t));
		for(j = 0; j < rows; j++){
			mpq_init(res[i][j]);
		}
	}

	for(i = 0; i < cols; i++){
		for(j = 0; j < rows; j++){
			mpq_set(res[i][j], m[j][i]);
		}
	}

	return res;

}


mpq_t* copyVector(mpq_t* v, int dimension){
	int i;
	mpq_t* res;

	res = (mpq_t*)calloc(dimension, sizeof(mpq_t));
	for(i = 0; i < dimension; i++){
		mpq_init(res[i]);		
	}

	for(i = 0; i < dimension; i++){
		mpq_set(res[i], v[i]);		
	}

	return res;

}

mpq_t** copyMatrix(mpq_t** m, int rows, int cols){
	int i, j;
	mpq_t** res;

	res = (mpq_t**)calloc(cols, sizeof(mpq_t*));
	for(i = 0; i < cols; i++){
		res[i] = (mpq_t*)calloc(rows, sizeof(mpq_t));
		for(j = 0; j < rows; j++){
			mpq_init(res[i][j]);
		}
	}

	for(i = 0; i < cols; i++){
		for(j = 0; j < rows; j++){
			mpq_set(res[i][j], m[i][j]);
		}
	}

	return res;

}

void freeMatrix(mpq_t** m, int n_rows, int n_cols){

	int i, j;
	for(i = 0; i < n_rows; i++){
			for(j = 0; j < n_cols; j++){
				mpq_clear(m[i][j]);
			}
			free(m[i]);
		}
		free(m);
}






