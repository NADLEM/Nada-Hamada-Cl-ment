#include "tarjan.h"
#include <string.h>

#define STACK_INIT_CAP 64

typedef struct {
    int *data;
    int top;
    int cap;
} IntStack;

static IntStack *stack_new() {
    IntStack *s = malloc(sizeof(IntStack));
    s->cap = STACK_INIT_CAP;
    s->data = malloc(s->cap * sizeof(int));
    s->top = 0;
    return s;
}
static void stack_free(IntStack *s) { free(s->data); free(s); }
static void stack_push(IntStack *s, int v) {
    if (s->top == s->cap) {
        s->cap *= 2;
        s->data = realloc(s->data, s->cap * sizeof(int));
    }
    s->data[s->top++] = v;
}
static int stack_pop(IntStack *s) { return s->data[--s->top]; }
static int stack_size(IntStack *s) { return s->top; }
static int stack_contains(IntStack *s, int v) {
    for (int i = 0; i < s->top; ++i) if (s->data[i] == v) return 1;
    return 0;
}

// dynamic array of classes
static t_class *append_class(t_class *arr, int *nb, int *cap, int *verts, int nverts) {
    if (*nb == *cap) {
        *cap = (*cap == 0) ? 8 : (*cap * 2);
        arr = realloc(arr, (*cap) * sizeof(t_class));
    }
    arr[*nb].verts = malloc(nverts * sizeof(int));
    memcpy(arr[*nb].verts, verts, nverts * sizeof(int));
    arr[*nb].size = nverts;
    (*nb)++;
    return arr;
}

typedef struct {
    t_tarjan_vertex *V;
    int n;
    int idx;
    IntStack *stack;
    t_class *classes;
    int classes_nb;
    int classes_cap;
} TarjanCtx;

static void strongconnect(TarjanCtx *ctx, liste_adjacence *g, int v) {
    ctx->V[v].index = ctx->idx;
    ctx->V[v].lowlink = ctx->idx;
    ctx->idx++;
    stack_push(ctx->stack, v);
    ctx->V[v].onstack = 1;

    cellule *cur = g->tab[v].head;
    while (cur) {
        int w = cur->arrivee;
        if (ctx->V[w].index == -1) {
            strongconnect(ctx, g, w);
            if (ctx->V[w].lowlink < ctx->V[v].lowlink) ctx->V[v].lowlink = ctx->V[w].lowlink;
        } else if (ctx->V[w].onstack) {
            if (ctx->V[w].index < ctx->V[v].lowlink) ctx->V[v].lowlink = ctx->V[w].index;
        }
        cur = cur->suiv;
    }

    if (ctx->V[v].lowlink == ctx->V[v].index) {
        // root of SCC
        int tmp_cap = 8;
        int *tmp = malloc(tmp_cap * sizeof(int));
        int tmp_n = 0;
        int w;
        do {
            w = stack_pop(ctx->stack);
            ctx->V[w].onstack = 0;
            if (tmp_n == tmp_cap) {
                tmp_cap *= 2;
                tmp = realloc(tmp, tmp_cap * sizeof(int));
            }
            tmp[tmp_n++] = w;
        } while (w != v);
        ctx->classes = append_class(ctx->classes, &ctx->classes_nb, &ctx->classes_cap, tmp, tmp_n);
        free(tmp);
    }
}

t_partition tarjan_partition(liste_adjacence g) {
    TarjanCtx ctx;
    ctx.n = g.taille;
    ctx.V = malloc(ctx.n * sizeof(t_tarjan_vertex));

    for (int i = 0; i < ctx.n; ++i) {
        ctx.V[i].id = i;
        ctx.V[i].index = -1;
        ctx.V[i].lowlink = -1;
        ctx.V[i].onstack = 0;
    }

    ctx.idx = 0;
    ctx.stack = stack_new();
    ctx.classes = NULL;
    ctx.classes_nb = 0;
    ctx.classes_cap = 0;

    for (int v = 0; v < ctx.n; ++v) {
        if (ctx.V[v].index == -1) {
            strongconnect(&ctx, &g, v);
        }
    }

    t_partition p;
    p.nb = ctx.classes_nb;
    p.classes = malloc(p.nb * sizeof(t_class));

    for (int i = 0; i < p.nb; ++i) {
        p.classes[i].size = ctx.classes[i].size;
        p.classes[i].verts = ctx.classes[i].verts;
    }

    free(ctx.V);
    stack_free(ctx.stack);
    free(ctx.classes);

    return p;
}


void free_partition(t_partition *p) {
    for (int i = 0; i < p->nb; ++i) free(p->classes[i].verts);
    free(p->classes);
    p->classes = NULL;
    p->nb = 0;
}
