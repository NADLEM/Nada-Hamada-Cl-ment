#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

int main(int argc, char *argv[]) {
    const char *filename;
    if (argc >= 2) filename = argv[1];
    else filename = "../data/exemple_tarjan.txt"; // slonle fichier que je veux tester

    /* ../data/exemple1.txt , ../data/exemple_tarjan.txt */

    printf("Lecture du fichier : %s\n", filename);
    liste_adjacence g = readGraph(filename);
    printf("Graphe chargé. Nombre de sommets = %d\n\n", g.taille);

    // ---- PARTIE 1 ----
    afficherListeAdjacence(g);

    printf("\n--- Vérification du graphe de Markov ---\n");
    int ok = estGrapheDeMarkov(g);

    if (ok)
        printf("\n Le graphe est bien un graphe de Markov.\n");
    else
        printf("\n Le graphe n'est pas un graphe de Markov.\n");

    printf("\n--- Exportation du graphe au format Mermaid ---\n");
    exportToMermaid(g, "exemple_valid_step3.txt");




    // ---- PARTIE 2 : TARJAN ----
    printf("\n--- Calcul des composantes fortement connexes (Tarjan) ---\n");
    t_partition part = tarjan_partition(g);
    printf("Nombre de composantes : %d\n", part.nb);

    for (int i = 0; i < part.nb; ++i) {
        printf("Composante C%d: {", i+1);
        for (int j = 0; j < part.classes[i].size; ++j) {
            printf("%d", part.classes[i].verts[j] + 1);
            if (j + 1 < part.classes[i].size) printf(", ");
        }
        printf("}\n");
    }

    // build Hasse
    t_link_array la = build_hasse(g, part);
    print_hasse_mermaid(la, part, "hasse.txt");
    printf("Diagramme de Hasse exporte vers hasse.txt\n");

    // classes transitoires/persistantes/absorbantes
    printf("\n--- Propriétés des classes ---\n");
    int *is_persistent = calloc(part.nb, sizeof(int));
    for (int ci = 0; ci < part.nb; ++ci) is_persistent[ci] = 1;
    for (int i = 0; i < la.nb; ++i) is_persistent[la.links[i].from] = 0;

    for (int ci = 0; ci < part.nb; ++ci) {
        if (is_persistent[ci]) printf("Classe C%d est persistante\n", ci+1);
        else printf("Classe C%d est transitoire\n", ci+1);
    }

    for (int ci = 0; ci < part.nb; ++ci) {
        if (is_persistent[ci] && part.classes[ci].size == 1) {
            printf("Classe C%d est absorbante (etat %d)\n",
                   ci+1, part.classes[ci].verts[0] + 1);
        }
    }




    // ---- PARTIE 3 : MATRICES ----

    printf("\n--- Matrice de transition M ---\n");
    t_matrix M = matrixFromAdjacency(&g);
    printMatrix(&M);



    // ---- M^k (puissances de matrice) ----
    int k = 5;  // puissance à tester (modifiable)

    printf("\n--- Matrice M puissance ---\n", k);

    t_matrix Mk = createEmptyMatrix(M.rows);
    copyMatrix(&M, &Mk);  // Mk = M

    for (int i = 1; i < k; i++) {
        t_matrix tmp = multiplyMatrices(&Mk, &M);
        freeMatrix(&Mk);
        Mk = tmp;
    }

    printMatrix(&Mk);



    // ---- Convergence de M^n ----
    printf("\n--- Test de convergence de M^n ---\n");

    t_matrix A = createEmptyMatrix(M.rows);
    t_matrix B = createEmptyMatrix(M.rows);

    copyMatrix(&M, &A);
    copyMatrix(&M, &B);

    float epsilon = 1e-6;
    int max_iter = 1000;
    int iter = 0;

    while (iter < max_iter) {
        t_matrix C = multiplyMatrices(&B, &M);
        freeMatrix(&B);
        B = C;

        float diff = diffMatrices(&A, &B);
        if (diff < epsilon) break;

        copyMatrix(&B, &A);
        iter++;
    }

    printf("Convergence atteinte en %d itérations.\n", iter);
    printf("Matrice limite :\n");
    printMatrix(&B);



    // ---- Sous-matrice d’une classe ----
    if (part.nb > 0) {
        printf("\n--- Sous-matrice de la classe C1 ---\n");
        t_matrix S = subMatrix(&M, &part, 0);
        printMatrix(&S);
        freeMatrix(&S);
    }



    // ---- FREE ----
    freeMatrix(&M);
    freeMatrix(&Mk);
    freeMatrix(&A);
    freeMatrix(&B);
    free(is_persistent);

    freeListeAdjacence(&g);

    return 0;
}
