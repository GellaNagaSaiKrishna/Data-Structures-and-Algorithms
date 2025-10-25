#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INF (SIZE_MAX)

// We store both the vertex and best distance found so far to it.
struct heapnode {
	size_t v;
	size_t d;
};

// index[v] stores the index of v in elts.
struct heap {
	struct heapnode *elts;
	size_t *index;
	size_t len;
};

bool is_empty(struct heap *h)
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

void make(struct heap *h, size_t n)
{
	h->elts = malloc((n + 1) * sizeof(struct heapnode));
	h->index = malloc(n * sizeof(size_t));
	h->len = 0;
}

void free_heap(struct heap *h)
{
	free(h->elts);
	free(h->index);
}

// swap two elements in heap while taking care of index[].
void swap(struct heap *h, size_t i, size_t j)
{
	struct heapnode *elts = h->elts;
	size_t *index = h->index;
	struct heapnode t = elts[i];
	elts[i] = elts[j];
	elts[j] = t;
	index[elts[i].v] = i;
	index[elts[j].v] = j;
}

// move cur up the heap to maintain heap property.
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
	size_t *index = h->index;

	size_t cur = index[v];
	elts[cur].d = d;

	sift_up(h, cur);
}

// move cur down the heap to maintain heap property.
void sift_down(struct heap *h, size_t cur)
{
	struct heapnode *elts = h->elts;

	size_t l, r, n;

	while (cur <= h->len) {
		if (left(cur) > h->len)
			break;

		l = left(cur);

		if (right(cur) > h->len) {
			// if only left child is present,
			// then left must be a leaf.
			if (elts[cur].d > elts[l].d)
				swap(h, cur, l);
			return;
		}

		// both children are present

		r = right(cur);

		if (elts[cur].d <= elts[l].d && elts[cur].d <= elts[r].d)
			break;

		// at least one child is smaller
		// move to the smallest child.
		if (elts[l].d <= elts[r].d)
			n = l;
		else
			n = r;

		swap(h, cur, n);
		cur = n;
	}
}

struct heapnode extract_min(struct heap *h)
{
	if (h->len == 1) {
		h->len = 0;
		return h->elts[1];
	}

	swap(h, 1, h->len);
	h->len -= 1;
	sift_down(h, 1);
	return h->elts[h->len + 1];
}

size_t dist(struct heap *h, size_t v)
{
	return h->elts[h->index[v]].d;
}

// We store the weight of (u, v) edge along with its tail v.
struct node {
	size_t v;
	size_t w;
	struct node *next;
};

struct graph {
	size_t n;
	struct node **adj;
};

void dijkstra(struct graph *g, size_t s, size_t *d)
{
	struct heap h;

	make(&h, g->n);

	for (size_t v = 0; v < g->n; ++v)
		add(&h, v, INF);
	decrease_key(&h, s, 0);

	while (!is_empty(&h)) {
		struct heapnode hn = extract_min(&h);
		size_t u = hn.v;
		d[u] = hn.d;
		if (d[u] == INF)
			break;
		for (struct node *v = g->adj[u]; v; v = v->next)
			if (dist(&h, v->v) > d[u] + v->w)
				decrease_key(&h, v->v, d[u] + v->w);
	}

	free_heap(&h);
}

void read_graph(struct graph *g)
{
	size_t n, m, u, v, w;

	scanf("%lu\n", &n);
	scanf("%lu\n", &m);

	g->n = n;
	g->adj = calloc(n, sizeof(struct node));

	for (size_t i = 0; i < m; ++i) {
		scanf("%lu %lu %lu\n", &u, &v, &w);
		struct node *uv = malloc(sizeof(struct node));
		uv->v = v;
		uv->w = w;
		uv->next = g->adj[u];
		g->adj[u] = uv;
	}
}

void free_list(struct node *p)
{
	if (p)
		free_list(p->next);
	free(p);
}

void free_graph(struct graph *g)
{
	for (size_t u = 0; u < g->n; ++u)
		free_list(g->adj[u]);
	free(g->adj);
}

int main()
{
	struct graph g;
	size_t s;

	read_graph(&g);
	scanf("%lu\n", &s);

	size_t *d = malloc(g.n * sizeof(size_t));

	dijkstra(&g, s, d);

	printf("<");
	for (size_t v = 0; v < g.n; ++v)
		printf(" %lu ", d[v]);
	printf(">\n");

	free(d);
	free_graph(&g);

	return 0;
}

// sample input from CLRS
// 5
// 10
// 0 1 10
// 0 4 5
// 1 2 1
// 1 4 2
// 2 3 4
// 3 2 6
// 3 0 7
// 4 1 3
// 4 2 9
// 4 3 2
// 0
