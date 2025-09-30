#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_QUEUE_SIZE 1000

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

void bfs_visit(struct graph *g, size_t start, struct queue *q, bool *visited, ssize_t *p) {
    p[start] = -1;
    enqueue(q, start);
    visited[start] = true;
    while (!is_empty(q)) {
        size_t u = dequeue(q);
        for (struct node *node = g->adj[u]; node; node = node->next) {
            size_t v = node->to;
            if (!visited[v]) {
                p[v] = u;
                visited[v] = true;
                enqueue(q, v);
            }
        }
    }
}

void bfs(struct graph *g, ssize_t *p) {
    bool *visited = calloc(g->n, sizeof(bool));
    struct queue q;
    init_queue(&q);
    for (size_t i = 0; i < g->n; i++) {
        if (!visited[i]) {
            bfs_visit(g, i, &q, visited, p);
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
