#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct node {
    size_t to;
    struct node *next;
};

struct graph {
    struct node **adj;
    size_t n;
};

struct graph *new_graph(size_t n) {
    struct graph *g = malloc(sizeof(struct graph));
    g->n = n;
    g->adj = calloc(n, sizeof(struct node *));
    return g;
}

void add_edge(struct graph *g, size_t from, size_t to) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->to = to;
    new_node->next = g->adj[from];
    g->adj[from] = new_node;
}

void do_dfs(
    struct graph *g, size_t u,
    bool *visited,
    size_t *d, size_t *f, ssize_t *p,
    size_t *timer
) {
    visited[u] = true;
    d[u] = ++(*timer);

    for (struct node *node = g->adj[u]; node; node = node->next) {
        size_t v = node->to;
        if (!visited[v]) {
            p[v] = u;
            do_dfs(g, v, visited, d, f, p, timer);
        }
    }

    f[u] = ++(*timer);
}

void dfs_visit(
    struct graph *g, size_t start,
    bool *visited, size_t *d, size_t *f, ssize_t *p, size_t *timer
) {
    p[start] = -1;
    do_dfs(g, start, visited, d, f, p, timer);
}

void dfs(
    struct graph *g, size_t *d, size_t *f, ssize_t *p
) {
    bool *visited = calloc(g->n, sizeof(bool));
    size_t timer = 0;

    for (size_t i = 0; i < g->n; i++) {
        if (!visited[i]) {
            dfs_visit(g, i, visited, d, f, p, &timer);
        }
    }

    free(visited);
}

void free_graph(struct graph *g) {
    for (size_t i = 0; i < g->n; i++) {
        struct node *node = g->adj[i];
        while (node) {
            struct node *tmp = node;
            node = node->next;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int main() {
    struct graph *g = new_graph(6);

    add_edge(g, 0, 2);
    add_edge(g, 0, 1);
    add_edge(g, 1, 3);
    add_edge(g, 2, 5);
    add_edge(g, 2, 4);
    add_edge(g, 5, 4);
    add_edge(g, 5, 0);

    size_t *d = malloc(g->n * sizeof(size_t));
    size_t *f = malloc(g->n * sizeof(size_t));
    ssize_t *p = malloc(g->n * sizeof(ssize_t));

    for (size_t i = 0; i < g->n; i++) {
        d[i] = f[i] = SIZE_MAX;
        p[i] = -1;
    }

    dfs(g, d, f, p);

    printf("%4s%4s%4s%4s\n", "v", "d", "f", "p");
    for (size_t i = 0; i < g->n; i++) {
        printf("%4zu%4zu%4zu%4zd\n", i, d[i], f[i], p[i]);
    }

    free(d);
    free(f);
    free(p);
    free_graph(g);

    return 0;
}
