#include "hasse.h"
#include <string.h>

static int link_exists(t_link_array *la, int a, int b) {
    for (int i = 0; i < la->nb; ++i) if (la->links[i].from == a && la->links[i].to == b) return 1;
    return 0;
}

t_link_array build_hasse(liste_adjacence g, t_partition part) {
    t_link_array la;
    la.nb = 0;
    la.links = NULL;
    int cap = 0;
    // build a vertex->class table
    int n = g.taille;
    int *v2c = malloc(n * sizeof(int));
    for (int i = 0; i < part.nb; ++i) {
        for (int j = 0; j < part.classes[i].size; ++j) {
            int v = part.classes[i].verts[j];
            v2c[v] = i;
        }
    }
    for (int v = 0; v < n; ++v) {
        cellule *cur = g.tab[v].head;
        int cv = v2c[v];
        while (cur) {
            int u = cur->arrivee;
            int cu = v2c[u];
            if (cv != cu && !link_exists(&la, cv, cu)) {
                if (la.nb == cap) {
                    cap = cap ? cap * 2 : 8;
                    la.links = realloc(la.links, cap * sizeof(t_link));
                }
                la.links[la.nb].from = cv;
                la.links[la.nb].to = cu;
                la.nb++;
            }
            cur = cur->suiv;
        }
    }
    free(v2c);
    return la;
}

void free_links(t_link_array *la) {
    if (la->links) free(la->links);
    la->links = NULL;
    la->nb = 0;
}

static char *getClassId(int idx) {
    // produce label C1, C2...
    char *s = malloc(16);
    snprintf(s, 16, "C%d", idx + 1);
    return s;
}

void print_hasse_mermaid(t_link_array la, t_partition part, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) { perror("open hasse"); return; }
    fprintf(f, "---\nconfig:\n  layout: elk\n  theme: neo\n  look: neo\n---\n\nflowchart LR\n");
    // print classes as nodes
    for (int i = 0; i < part.nb; ++i) {
        char *id = getClassId(i);
        fprintf(f, "%s((%s))\n", id, id);
        free(id);
    }
    fprintf(f, "\n");
    for (int i = 0; i < la.nb; ++i) {
        char *from = getClassId(la.links[i].from);
        char *to = getClassId(la.links[i].to);
        fprintf(f, "%s --> %s\n", from, to);
        free(from); free(to);
    }
    fclose(f);
}
