#include "graph.h"
#include <string.h>
#include <time.h>

// ---------- Fonctions de base ----------
cellule *creerCellule(int arrivee, float proba) {
    cellule *c = (cellule *)malloc(sizeof(cellule));
    if (!c) { perror("malloc"); exit(EXIT_FAILURE); }
    c->arrivee = arrivee; // internal 0-index
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

liste creerListe() {
    liste L;
    L.head = NULL;
    return L;
}

void ajouterCellule(liste *L, int arrivee, float proba) {
    cellule *c = creerCellule(arrivee, proba);
    c->suiv = L->head;
    L->head = c;
}

void afficherListe(liste L) {
    cellule *cur = L.head;
    while (cur != NULL) {
        printf(" -> (%d, %.2f)", cur->arrivee + 1, cur->proba); // display 1-index
        cur = cur->suiv;
    }
    printf("\n");
}

liste_adjacence creerListeAdjacence(int taille) {
    liste_adjacence g;
    g.taille = taille;
    g.tab = (liste *)malloc(taille * sizeof(liste));
    if (!g.tab) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < taille; ++i) g.tab[i] = creerListe();
    return g;
}

void afficherListeAdjacence(liste_adjacence g) {
    for (int i = 0; i < g.taille; ++i) {
        printf("Sommet %d :", i + 1);
        afficherListe(g.tab[i]);
    }
}

// ---------- Lecture du fichier (readGraph) ----------
// expected file format:
// n
// depart arrivee proba
// lines with 1-indexed nodes
liste_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) { perror(filename); exit(EXIT_FAILURE); }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        fprintf(stderr, "Could not read number of vertices from %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    liste_adjacence g = creerListeAdjacence(nbvert);

    int depart, arrivee;
    float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "Warning: invalid edge %d -> %d (ignored)\n", depart, arrivee);
            continue;
        }
        // store internal as 0-index
        ajouterCellule(&g.tab[depart - 1], arrivee - 1, proba);
    }

    fclose(file);
    return g;
}

// ---------- Libération de la mémoire ----------
void freeListeAdjacence(liste_adjacence *g) {
    for (int i = 0; i < g->taille; ++i) {
        cellule *cur = g->tab[i].head;
        while (cur) {
            cellule *tmp = cur;
            cur = cur->suiv;
            free(tmp);
        }
        g->tab[i].head = NULL;
    }
    free(g->tab);
    g->tab = NULL;
    g->taille = 0;
}

// ---------- Étape 2 : Vérifier si le graphe est un graphe de Markov ----------
int estGrapheDeMarkov(liste_adjacence g) {
    int estValide = 1;

    for (int i = 0; i < g.taille; i++) {
        cellule *cur = g.tab[i].head;
        float somme = 0.0f;

        while (cur != NULL) {
            somme += cur->proba;
            cur = cur->suiv;
        }

        if (somme < 0.99f || somme > 1.01f) {
            printf("Sommet %d : somme des probabilites = %.3f \n", i + 1, somme);
            estValide = 0;
        } else {
            printf("Sommet %d : somme des probabilites = %.3f \n", i + 1, somme);
        }
    }

    return estValide;
}

// ---------- Étape 3 : Export vers Mermaid -----------
char *getId(int num) {
    // produce short id (A, B, ..., Z, AA, AB, ...)
    // num is 1-indexed for display convenience here
    int n = num - 1;
    char buf[8];
    int idx = 0;
    // create reversed letters base-26
    char rev[8];
    int pos = 0;
    do {
        rev[pos++] = 'A' + (n % 26);
        n = n / 26 - 1;
    } while (n >= 0 && pos < 7);
    for (int i = 0; i < pos; ++i) buf[i] = rev[pos - 1 - i];
    buf[pos] = '\0';
    char *id = strdup(buf);
    return id;
}

void exportToMermaid(liste_adjacence g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Erreur ouverture fichier Mermaid");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");

    fprintf(f, "flowchart LR\n");

    for (int i = 0; i < g.taille; i++) {
        char *id = getId(i + 1);
        fprintf(f, "%s((%d))\n", id, i + 1);
        free(id);
    }
    fprintf(f, "\n");

    for (int i = 0; i < g.taille; i++) {
        cellule *cur = g.tab[i].head;
        while (cur != NULL) {
            char *from = getId(i + 1);
            char *to = getId(cur->arrivee + 1);
            fprintf(f, "%s -->|%.2f|%s\n", from, cur->proba, to);
            free(from);
            free(to);
            cur = cur->suiv;
        }
    }

    fclose(f);
    printf("Fichier Mermaid exporte dans : %s\n", filename);
}

// ---------- DFS pour accessibilité (utilisé par afficherEtatsAccessibles) ----------
static void dfs_access(const liste_adjacence *g, int u, int *vis) {
    vis[u] = 1;
    cellule *cur = g->tab[u].head;
    while (cur) {
        if (!vis[cur->arrivee]) dfs_access(g, cur->arrivee, vis);
        cur = cur->suiv;
    }
}

void afficherEtatsAccessibles(liste_adjacence g, int sommetDepart) {
    if (sommetDepart < 0 || sommetDepart >= g.taille) {
        printf("Sommet de depart invalide.\n");
        return;
    }
    int *vis = (int *)calloc(g.taille, sizeof(int));
    dfs_access(&g, sommetDepart, vis);
    printf("Etats accessibles depuis le sommet %d : ", sommetDepart + 1);
    for (int i = 0; i < g.taille; ++i) if (vis[i]) printf("%d ", i + 1);
    printf("\n");
    free(vis);
}

// ---------- trouverEtatsAbsorbants ----------
void trouverEtatsAbsorbants(liste_adjacence g) {
    printf("Etats absorbants : ");
    int found = 0;
    for (int i = 0; i < g.taille; ++i) {
        cellule *cur = g.tab[i].head;
        if (!cur) continue;
        // check if only self-loop with proba 1
        if (cur->suiv == NULL && cur->arrivee == i && fabs(cur->proba - 1.0f) < 1e-6) {
            printf("%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("aucun");
    printf("\n");
}

// ---------- Simulation marche aléatoire ----------
void simulerMarcheAleatoire(liste_adjacence g, int depart, int max_etapes) {
    if (depart < 0 || depart >= g.taille) {
        printf("Sommet de depart invalide pour la simulation.\n");
        return;
    }
    int courant = depart;
    printf("Début de la marche aléatoire à partir du sommet %d\n", courant + 1);

    for (int etape = 0; etape < max_etapes; etape++) {
        printf("Étape %d : sommet %d\n", etape + 1, courant + 1);

        cellule *cur = g.tab[courant].head;

        // Pas de sortie possible (état absorbant)
        if (cur == NULL) {
            printf("Sommet %d est un état absorbant. Fin de la marche.\n", courant + 1);
            break;
        }

        // Choisir un voisin selon les probabilités
        float r = (float)rand() / (float)RAND_MAX;
        float somme = 0.0f;
        int choisi = -1;
        while (cur != NULL) {
            somme += cur->proba;
            if (r <= somme) {
                choisi = cur->arrivee;
                break;
            }
            cur = cur->suiv;
        }
        if (choisi == -1) {
            // due to float rounding, pick last neighbor
            cur = g.tab[courant].head;
            while (cur->suiv) cur = cur->suiv;
            choisi = cur->arrivee;
        }
        courant = choisi;
    }
}
