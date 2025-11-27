#ifndef TARJAN_H
#define TARJAN_H

#include "graph.h"

// Vertex info used by Tarjan
typedef struct {
    int id;        // 0-index
    int index;     // -1 if not visited
    int lowlink;
    int onstack;   // 0/1
} t_tarjan_vertex;

typedef struct {
    int *verts;    // array of vertex indices (0-index)
    int size;
} t_class;

typedef struct {
    t_class *classes;
    int nb; // number of classes
} t_partition;

// Prototypes
t_partition tarjan_partition(liste_adjacence g);
void free_partition(t_partition *p);

#endif
