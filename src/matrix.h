#ifndef MATRIX_H
#define MATRIX_H

#include "tarjan.h"

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;

t_matrix createEmptyMatrix(int n);
void freeMatrix(t_matrix *m);
void copyMatrix(const t_matrix *src, t_matrix *dst);
t_matrix multiplyMatrices(const t_matrix *A, const t_matrix *B);
float diffMatrices(const t_matrix *A, const t_matrix *B);
t_matrix matrixFromAdjacency(const liste_adjacence *g);
t_matrix subMatrix(const t_matrix *matrix, const t_partition *part, int compo_index);
void printMatrix(const t_matrix *m);

#endif
