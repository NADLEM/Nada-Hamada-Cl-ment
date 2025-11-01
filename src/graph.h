#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

// ---------- Structures ----------
typedef struct cellule {
    int arrivee;
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

// ---------- Fonctions de base ----------
cellule *creerCellule(int arrivee, float proba);
liste creerListe();
void ajouterCellule(liste *L, int arrivee, float proba);
void afficherListe(liste L);
liste_adjacence creerListeAdjacence(int taille);
void afficherListeAdjacence(liste_adjacence g);

// ---------- Lecture du fichier -----------
liste_adjacence readGraph(const char *filename);

// ---------- Libération de la mémoire ----------
void freeListeAdjacence(liste_adjacence *g);

// ---------- Étape 2 : Vérification Markov ----------
int estGrapheDeMarkov(liste_adjacence g);

// ---------- Étape 3 : Export vers Mermaid ----------
char *getId(int num);
void exportToMermaid(liste_adjacence g, const char *filename);

#endif
