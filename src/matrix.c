#include "matrix.h"
#include <string.h>
#include <math.h>

t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;
    m.cols = n;
    m.data = malloc(n * sizeof(float *));
    for (int i = 0; i < n; ++i) {
        m.data[i] = calloc(n, sizeof(float));
    }
    return m;
}

void freeMatrix(t_matrix *m) {
    if (!m->data) return;
    for (int i = 0; i < m->rows; ++i) free(m->data[i]);
    free(m->data);
    m->data = NULL;
    m->rows = m->cols = 0;
}

void copyMatrix(const t_matrix *src, t_matrix *dst) {
    if (dst->rows != src->rows || dst->cols != src->cols) {
        fprintf(stderr, "copyMatrix: size mismatch\n");
        return;
    }
    for (int i = 0; i < src->rows; ++i)
        for (int j = 0; j < src->cols; ++j)
            dst->data[i][j] = src->data[i][j];
}

t_matrix multiplyMatrices(const t_matrix *A, const t_matrix *B) {
    if (A->cols != B->rows) { fprintf(stderr, "multiplyMatrices: size mismatch\n"); exit(EXIT_FAILURE); }
    t_matrix C = createEmptyMatrix(A->rows);
    for (int i = 0; i < A->rows; ++i) {
        for (int k = 0; k < A->cols; ++k) {
            float aik = A->data[i][k];
            if (aik == 0.0f) continue;
            for (int j = 0; j < B->cols; ++j) {
                C.data[i][j] += aik * B->data[k][j];
            }
        }
    }
    return C;
}

float diffMatrices(const t_matrix *A, const t_matrix *B) {
    if (A->rows != B->rows || A->cols != B->cols) { fprintf(stderr, "diffMatrices: size mismatch\n"); exit(EXIT_FAILURE); }
    float s = 0.0f;
    for (int i = 0; i < A->rows; ++i)
        for (int j = 0; j < A->cols; ++j)
            s += fabsf(A->data[i][j] - B->data[i][j]);
    return s;
}

t_matrix matrixFromAdjacency(const liste_adjacence *g) {
    int n = g->taille;
    t_matrix M = createEmptyMatrix(n);
    for (int i = 0; i < n; ++i) {
        cellule *cur = g->tab[i].head;
        while (cur) {
            M.data[i][cur->arrivee] += cur->proba; // in case multiple entries
            cur = cur->suiv;
        }
    }
    return M;
}

t_matrix subMatrix(const t_matrix *matrix, const t_partition *part, int compo_index) {
    if (compo_index < 0 || compo_index >= part->nb) {
        fprintf(stderr, "subMatrix: component index out of range\n"); exit(EXIT_FAILURE);
    }
    int m = part->classes[compo_index].size;
    t_matrix S = createEmptyMatrix(m);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            int vi = part->classes[compo_index].verts[i];
            int vj = part->classes[compo_index].verts[j];
            S.data[i][j] = matrix->data[vi][vj];
        }
    }
    return S;
}

void printMatrix(const t_matrix *m) {
    for (int i = 0; i < m->rows; ++i) {
        for (int j = 0; j < m->cols; ++j) printf("%.4f ", m->data[i][j]);
        printf("\n");
    }
}
