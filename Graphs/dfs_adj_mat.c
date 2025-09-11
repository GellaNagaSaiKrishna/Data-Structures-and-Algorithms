#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void do_dfs(
    int **adj, size_t n, size_t u,
    bool *visited,
    size_t *d, size_t *f, ssize_t *p,
    size_t *timer
) {
    visited[u] = true;
    d[u] = ++(*timer);

    for (size_t v = 0; v < n; v++) {
        if (adj[u][v] && !visited[v]) {
            p[v] = u;
            do_dfs(adj, n, v, visited, d, f, p, timer);
        }
    }

    f[u] = ++(*timer);
}

void dfs(
    int **adj, size_t n,
    size_t *d, size_t *f, ssize_t *p
) {
    bool *visited = calloc(n, sizeof(bool));
    size_t timer = 0;

    for (size_t i = 0; i < n; i++) {
        if (!visited[i]) {
            p[i] = -1;
            do_dfs(adj, n, i, visited, d, f, p, &timer);
        }
    }
    free(visited);
}

int main() {
    size_t n = 6;
    // Allocate adjacency matrix
    int **adj = malloc(n * sizeof(int*));
    for (size_t i = 0; i < n; i++) {
        adj[i] = calloc(n, sizeof(int));
    }

    // Add edges (same as your example)
    adj[0][2] = 1;
    adj[0][1] = 1;
    adj[1][3] = 1;
    adj[2][5] = 1;
    adj[2][4] = 1;
    adj[5][4] = 1;
    adj[5][0] = 1;

    size_t *d = malloc(n * sizeof(size_t));
    size_t *f = malloc(n * sizeof(size_t));
    ssize_t *p = malloc(n * sizeof(ssize_t));

    for (size_t i = 0; i < n; i++) {
        d[i] = f[i] = SIZE_MAX;
        p[i] = -1;
    }

    dfs(adj, n, d, f, p);

    printf("%4s%4s%4s%4s\n", "v", "d", "f", "p");
    for (size_t i = 0; i < n; i++) {
        printf("%4zu%4zu%4zu%4zd\n", i, d[i], f[i], p[i]);
    }

    free(d);
    free(f);
    free(p);
    for (size_t i = 0; i < n; i++) {
        free(adj[i]);
    }
    free(adj);

    return 0;
}
