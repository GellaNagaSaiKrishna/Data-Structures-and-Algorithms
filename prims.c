#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

struct heapnode {
	size_t v;
	int d;
};

struct heap {
	struct heapnode *elts;
	ssize_t *index;
	size_t len;
};

void init(struct heap *h, size_t n)
{
	h->elts = malloc((n + 1) * sizeof(struct heapnode));
	h->index = malloc((n + 1) * sizeof(ssize_t));
	h->len = 0;
}

void free_heap(struct heap *h)
{
	free(h->elts);
	free(h->index);
}

bool empty(struct heap *h)
{
	return h->len == 0;
}

size_t parent(size_t i)
{
	return i / 2;
}

size_t left(size_t i)
{
	return 2 * i;
}

size_t right(size_t i)
{
	return 2 * i + 1;
}

void swap(struct heap *h, size_t i, size_t j)
{
	struct heapnode *elts = h->elts;
	ssize_t *index = h->index;
	struct heapnode t = elts[i];
	elts[i] = elts[j];
	elts[j] = t;
	index[elts[i].v] = i;
	index[elts[j].v] = j;
}

void sift_down(struct heap *h, size_t cur)
{
	struct heapnode *elts = h->elts;

	size_t l, r, n;

	while (cur <= h->len) {
		if (left(cur) > h->len)
			break;

		l = left(cur);

		if (right(cur) > h->len) {
			if (elts[cur].d > elts[l].d)
				swap(h, cur, l);
			return;
		}

		r = right(cur);

		if (elts[cur].d <= elts[l].d && elts[cur].d <= elts[r].d)
			break;

		if (elts[l].d <= elts[r].d)
			n = l;
		else
			n = r;

		swap(h, cur, n);
		cur = n;
	}
}

struct heapnode min(struct heap *h)
{
	if (h->len == 1) {
		h->len = 0;
		h->index[h->elts[1].v] = -1;
		return h->elts[1];
	}

	swap(h, 1, h->len);
	h->len -= 1;
	h->index[h->elts[h->len + 1].v] = -1;
	sift_down(h, 1);
	return h->elts[h->len + 1];
}

void sift_up(struct heap *h, size_t cur)
{
	size_t p;
	struct heapnode *elts = h->elts;

	while ((p = parent(cur)) > 0) {
		if (elts[cur].d < elts[p].d)
			swap(h, cur, p);
		cur = p;
	}
}

void add(struct heap *h, size_t v, size_t d)
{
	h->elts[++h->len] = (struct heapnode) {
		.v = v,
		.d = d
	};
	h->index[v] = h->len;
	sift_up(h, h->len);
}

void decrease_key(struct heap *h, size_t v, size_t d)
{
	struct heapnode *elts = h->elts;
	ssize_t *index = h->index;

	ssize_t cur = index[v];
	elts[cur].d = d;

	sift_up(h, cur);
}

bool member(struct heap *h, size_t u)
{
	return h->index[u] >= 0;
}

int key(struct heap *h, size_t v)
{
	return h->elts[h->index[v]].d;
}

#define for_each_edge(e, g, u)					\
	for (struct edge *e = g->adj[u]; e; e = e->next)

struct edge {
	size_t v;
	int w;
	struct edge *next;
};

struct graph {
	struct edge **adj;
	size_t n;
};

void add_edge(struct graph *g, size_t u, size_t v, int w)
{
	struct edge *e = malloc(sizeof(struct edge));
	e->v = v;
	e->w = w;
	e->next = g->adj[u];
	g->adj[u] = e;
}

void read_undirected_graph(struct graph *g)
{
	size_t m, u, v;
	int w;

	scanf("%lu\n", &g->n);
	scanf("%lu\n", &m);

	g->adj = malloc(g->n * sizeof(struct edge *));
	for (size_t u = 0; u < g->n; ++u)
		g->adj[u] = NULL;

	for (size_t i = 0; i < m; ++i) {
		scanf("%lu %lu %d\n", &u, &v, &w);
		add_edge(g, u, v, w);
		add_edge(g, v, u, w);
	}
}

void prim(struct graph *g, ssize_t *p, size_t r)
{
	struct heap h;

	init(&h, g->n);
	for (size_t u = 0; u < g->n; ++u) {
		add(&h, u, INF);
		p[u] = -1;
	}
	decrease_key(&h, r, 0);

	while (!empty(&h)) {
		struct heapnode u = min(&h);
		for_each_edge(e, g, u.v) {
			if (member(&h, e->v) && e->w < key(&h, e->v)) {
				p[e->v] = u.v;
				decrease_key(&h, e->v, e->w);
			}
		}
	}

	free_heap(&h);
}

void print_array(ssize_t *p, size_t n)
{
	printf("< ");
	for (size_t i = 0; i < n; ++i)
		printf("%ld ", p[i]);
	printf(">\n");
}

int main()
{
	struct graph g;
	read_undirected_graph(&g);
	ssize_t *p = malloc(g.n * sizeof(ssize_t));
	prim(&g, p, 0);
	print_array(p, g.n);
	free(p);
	return 0;
}

/*Sample input (Here we use 1 based indexing)
6
9
6 1 4
6 2 4
1 2 2
2 3 3
2 5 2
3 4 3
4 5 3
3 5 4
1 5 6
*/
