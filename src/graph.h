#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

// ---------- Structures pour la liste d'adjacence ----------
typedef struct cellule {
    int arrivee;           // internal: 0-index
    float proba;
    struct cellule *suiv;
} cellule;

typedef struct {
    cellule *head;
} liste;

typedef struct {
    liste *tab;   // tableau dynamique de listes
    int taille;   // nombre de sommets
} liste_adjacence;

// ---------- Fonctions de base (partie 1) ----------
cellule *creerCellule(int arrivee, float proba);
liste creerListe();
void ajouterCellule(liste *L, int arrivee, float proba);
void afficherListe(liste L);
liste_adjacence creerListeAdjacence(int taille);
void afficherListeAdjacence(liste_adjacence g);

// ---------- Lecture / libération ----------
liste_adjacence readGraph(const char *filename);
void freeListeAdjacence(liste_adjacence *g);

// ---------- Vérification Markov ----------
int estGrapheDeMarkov(liste_adjacence g);

// ---------- Export Mermaid ----------
char *getId(int num);
void exportToMermaid(liste_adjacence g, const char *filename);

// ---------- utilitaires demandés dans main ----------
void trouverEtatsAbsorbants(liste_adjacence g);
void afficherEtatsAccessibles(liste_adjacence g, int sommetDepart);
void simulerMarcheAleatoire(liste_adjacence g, int depart, int max_etapes);

#endif
