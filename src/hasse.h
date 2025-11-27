#ifndef HASSE_H
#define HASSE_H

#include "tarjan.h"
#include "graph.h"

typedef struct {
    int from;
    int to;
} t_link;

typedef struct {
    t_link *links;
    int nb;
} t_link_array;

// prototypes
t_link_array build_hasse(liste_adjacence g, t_partition part);
void free_links(t_link_array *la);
void print_hasse_mermaid(t_link_array la, t_partition part, const char *filename);

#endif
