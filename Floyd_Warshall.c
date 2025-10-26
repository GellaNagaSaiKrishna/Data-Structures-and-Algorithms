#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// NB: Every quantity should be far away from INF.
#define INF INT_MAX

struct dense_graph {
	size_t n;
	int **w;
};

void print_table(int **d, size_t n, size_t m)
{
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c)
			printf("%4d ", d[r][c]);
		printf("\n");
	}
}

int minplus(int x, int y, int z)
{
	if (y == INF || z == INF)
		return x;

	if (x > y + z)
		return y + z;

	return x;
}

void floyd_warshall(struct dense_graph *g, int **d)
{
	for (size_t u = 0; u < g->n; ++u)
		for (size_t v = 0; v < g->n; ++v)
			d[u][v] = g->w[u][v];

	for (size_t k = 0; k < g->n; ++k)
		for (size_t u = 0; u < g->n; ++u)
			for (size_t v = 0; v < g->n; ++v)
				d[u][v] = minplus(d[u][v], d[u][k], d[k][v]);
}

void read_inf(int *v)
{
	char c;
	if (scanf(" %c", &c) && c == 'i') {
		*v = INF;
		return;
	}

	ungetc(c, stdin);

	scanf("%d", v);
}

void read_dense_graph(struct dense_graph *g)
{
	scanf("%lu\n", &(g->n));

	g->w = malloc(g->n * sizeof(int *));
	for (size_t u = 0; u < g->n; ++u)
		g->w[u] = malloc(g->n * sizeof(int));

	for (size_t u = 0; u < g->n; ++u)
		for (size_t v = 0; v < g->n; ++v)
			read_inf(&g->w[u][v]);
}

void free_table(int **d, size_t n)
{
	for (size_t u = 0; u < n; ++u)
		free(d[u]);
	free(d);
}

void free_dense_graph(struct dense_graph *g)
{
	for (size_t u = 0; u < g->n; ++u)
		free(g->w[u]);
	free(g->w);
}

int main()
{
	struct dense_graph g;

	read_dense_graph(&g);

	int **d = malloc(g.n * sizeof(int *));
	for (size_t u = 0; u < g.n; ++u)
		d[u] = malloc(g.n * sizeof(int));

	floyd_warshall(&g, d);

	print_table(d, g.n, g.n);

	free_dense_graph(&g);
	free_table(d, g.n);

	return 0;
}

// sample input from CLRS
// 5
// 0 3 8 i -4
// i 0 i 1 7
// i 4 0 i i
// 2 i -5 0 i
// i i i 6 0
