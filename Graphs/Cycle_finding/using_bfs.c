#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define MAX_QUEUE_SIZE 1000
#define MAX_VERTICES 10000 
#define MAX_PATH_SIZE 1000 

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
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc(n, sizeof(struct node *));
    if (!g->adj) { free(g); return NULL; }
    return g;
}
void add_edge(struct graph *g, size_t from, size_t to) {
    struct node *new_node = malloc(sizeof(struct node));
    if (!new_node) return;
    new_node->to = to;
    new_node->next = g->adj[from];
    g->adj[from] = new_node;
}
void free_graph(struct graph *g) {
    if (!g) return;
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
struct queue {
    size_t elts[MAX_QUEUE_SIZE];
    size_t head;
    size_t tail;
    size_t len;
};
void init_queue(struct queue *q) {
    q->head = 0;
    q->tail = 0;
    q->len = 0;
}
void enqueue(struct queue *q, size_t elt) {
    assert(q->len < MAX_QUEUE_SIZE);
    q->elts[q->tail] = elt;
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
    ++(q->len);
}
size_t dequeue(struct queue *q) {
    assert(q->len > 0);
    size_t u = q->elts[q->head];
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;
    --(q->len);
    return u;
}
bool is_empty(struct queue *q) {
    return q->len == 0;
}
void print_cycle(int u, int v, int *parent, int V_limit) {
    int path_u[MAX_PATH_SIZE];
    int path_v[MAX_PATH_SIZE];
    int len_u = 0;
    int len_v = 0;
    bool *on_path_u = calloc(V_limit, sizeof(bool));
    if (!on_path_u) { printf("N\n"); return; }
    int curr = u;
    while (curr != -1 && len_u < MAX_PATH_SIZE) {
        path_u[len_u++] = curr;
        on_path_u[curr] = true;
        curr = parent[curr];
    }
    curr = v;
    int meet_node = -1; 
    while (curr != -1 && len_v < MAX_PATH_SIZE) {
        if (on_path_u[curr]) {
            meet_node = curr;
            break;
        }
        path_v[len_v++] = curr;
        curr = parent[curr];
    }
    if (meet_node == -1) {
        free(on_path_u);
        printf("N\n");
        return;
    }

    printf("%d", meet_node);
    for (int i = 0; i < len_v; i++) {
        printf(" %d", path_v[i]);
    }
    int start_u_index = -1;
    for (int i = 0; i < len_u; i++) {
        if (path_u[i] == meet_node) {
            start_u_index = i;
            break;
        }
    }
    for (int i = start_u_index - 1; i >= 0; i--) {
        printf(" %d", path_u[i]);
    }
    printf(" %d\n", meet_node);

    free(on_path_u);
}

bool findAndPrintCycle(int V, int E) {
    if (V <= 0) return false;
    struct graph *g = new_graph(V);
    if (!g) return false;
    int *visited = calloc(V, sizeof(int));
    int *parent = malloc(V * sizeof(int));
    if (!visited || !parent) { free(visited); free(parent); free_graph(g); return false; }
    for (int i = 0; i < V; i++) { parent[i] = -1; }  
    for(int i = 0; i < E; i++){
        int u_in, v_in;
        if (scanf("%d %d", &u_in, &v_in) != 2) break; 
        if (u_in < 0 || u_in >= V || v_in < 0 || v_in >= V) continue; 
        add_edge(g, (size_t)u_in, (size_t)v_in);
        add_edge(g, (size_t)v_in, (size_t)u_in);
    }
    struct queue q;
    init_queue(&q);
    bool cycle_found = false;
    for (int i = 0; i < V; i++) {
        if (visited[i]) continue;
        visited[i] = 1;
        enqueue(&q, (size_t)i); 
        while (!is_empty(&q)) {
            size_t u_size = dequeue(&q);
            int u = (int)u_size;           
            for (struct node *node = g->adj[u_size]; node; node = node->next) {
                size_t v_size = node->to;
                int v = (int)v_size;               
                if (visited[v]) {
                    if (v != parent[u]) {
                        print_cycle(u, v, parent, V);
                        cycle_found = true;
                        goto cleanup;
                    }
                } else {
                    visited[v] = 1;
                    parent[v] = u;
                    enqueue(&q, v_size);
                }
            }
        }
    }
    cleanup:
    free(visited);
    free(parent);
    free_graph(g);
    
    return cycle_found;
}
int main() {
    int V, E;
    if (scanf("%d %d", &V, &E) != 2) {
        printf("N\n");
        return 0;
    }
    if (!findAndPrintCycle(V, E)) {
        printf("N\n");
    }
    return 0;
}
