#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define INF SIZE_MAX

struct node {
    size_t v;
    size_t w;
    struct node *next;
};

struct graph {
    size_t n;
    struct node **adj;
};

// Read graph from input
void read_graph(struct graph *g) {
    size_t n, m, u, v, w;
    scanf("%lu", &n);
    scanf("%lu", &m);

    g->n = n;
    g->adj = calloc(n, sizeof(struct node *));

    for (size_t i = 0; i < m; ++i) {
        scanf("%lu %lu %lu", &u, &v, &w);
        struct node *uv = malloc(sizeof(struct node));
        uv->v = v;
        uv->w = w;
        uv->next = g->adj[u];
        g->adj[u] = uv;
    }
}

void free_list(struct node *p) {
    if (p)
        free_list(p->next);
    free(p);
}

void free_graph(struct graph *g) {
    for (size_t u = 0; u < g->n; ++u)
        free_list(g->adj[u]);
    free(g->adj);
}

// Find the vertex with the minimum distance that is not yet visited
size_t min_distance(size_t *d, bool *visited, size_t n) {
    size_t min = INF;
    size_t u = INF;
    for (size_t i = 0; i < n; ++i) {
        if (!visited[i] && d[i] <= min) {
            min = d[i];
            u = i;
        }
    }
    return u;
}

// Dijkstra's algorithm using arrays
void dijkstra(struct graph *g, size_t s, size_t *d) {
    bool *visited = calloc(g->n, sizeof(bool));

    // Initialize distances
    for (size_t i = 0; i < g->n; ++i)
        d[i] = INF;
    d[s] = 0;

    for (size_t count = 0; count < g->n; ++count) {
        size_t u = min_distance(d, visited, g->n);
        if (u == INF)  // Remaining vertices are unreachable
            break;
        visited[u] = true;

        // Update distances of neighbors
        for (struct node *v = g->adj[u]; v; v = v->next) {
            if (!visited[v->v] && d[u] != INF && d[u] + v->w < d[v->v]) {
                d[v->v] = d[u] + v->w;
            }
        }
    }

    free(visited);
}

int main() {
    struct graph g;
    size_t s;

    read_graph(&g);
    scanf("%lu", &s);

    size_t *d = malloc(g.n * sizeof(size_t));
    dijkstra(&g, s, d);

    printf("<");
    for (size_t i = 0; i < g.n; ++i)
        printf(" %lu ", d[i]);
    printf(">\n");

    free(d);
    free_graph(&g);

    return 0;
}

/*
Sample input:
5
10
0 1 10
0 4 5
1 2 1
1 4 2
2 3 4
3 2 6
3 0 7
4 1 3
4 2 9
4 3 2
0
*/
