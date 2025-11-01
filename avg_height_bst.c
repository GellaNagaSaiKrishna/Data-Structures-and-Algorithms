#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For time() and srand()

// FIX: Define unreachable() to prevent linker error.
#define unreachable() abort() 
#define N_NODES 1000
#define N_EXPERIMENTS 1000

struct bst {
	int key;
	struct bst *left, *right;
};

// --- Utility Functions ---

// Calculates the height of a subtree recursively.
int height(struct bst *n)
{
	if (n == NULL) {
		return 0;
	}
	int left_height = height(n->left);
	int right_height = height(n->right);
	
	// Height is max of children's heights plus one for the current node.
	return (left_height > right_height ? left_height : right_height) + 1;
}

// Function to shuffle an array (Fisher-Yates algorithm)
void shuffle(int *array, int n)
{
	if (n > 1) {
		for (int i = 0; i < n - 1; i++) {
			int j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

// Function to free all nodes in the tree recursively
void destroy_tree(struct bst *t)
{
    if (t == NULL) return;
    destroy_tree(t->left);
    destroy_tree(t->right);
    free(t);
}

// --- BST Functions (Unchanged, but included for completeness) ---

void empty(struct bst **t)
{
	*t = NULL;
}

bool is_empty(struct bst **t)
{
	return *t == NULL;
}

struct bst *node(int k)
{
	struct bst *n = malloc(sizeof(struct bst));
	n->key = k;
	n->left = NULL;
	n->right = NULL;
	return n;
}

struct bst **search(struct bst **root, int key)
{
	while (*root) {
		struct bst *t = *root;

		if (t->key == key)
			return root;

		if (t->key < key)
			root = &(*root)->right;

		if (t->key > key)
			root = &(*root)->left;
	}
	return root;
}

struct bst **min(struct bst **root)
{
	while (*root) {
		struct bst *t = *root;
		if (t->left == NULL)
			return root;
		root = &(*root)->left;
	}
	unreachable();
}

struct bst **max(struct bst **root)
{
	while (*root) {
		struct bst *t = *root;
		if (t->right == NULL)
			return root;
		root = &(*root)->right;
	}
	unreachable();
}

void insert(struct bst **root, int key)
{
	struct bst **t = search(root, key);

	if (!*t)
		*t = node(key);
}

struct bst *unlink(struct bst **node)
{
	struct bst *save = *node;

	if ((*node)->left == NULL)
		*node = (*node)->right;
	else if ((*node)->right == NULL)
		*node = (*node)->left;
	else
		unreachable();

	return save;
		
}

struct bst *delete(struct bst **root, int key)
{
	struct bst **t = search(root, key);

	if (*t) {
		struct bst *p = *t;

		if (p->left == NULL || p->right == NULL)
			return unlink(t);

		struct bst **s = min(&(*t)->right);
		struct bst *m = unlink(s);
		m->left = (*t)->left;
		m->right = (*t)->right;
		*t = m;
		return p;
	}
}

struct bst **successor(struct bst **root, int key)
{
	struct bst **left = NULL;
	struct bst **e = NULL;

	while (*root) {
		struct bst *t = *root;

		if (t->key < key) {
			root = &(*root)->right;
		} else if (t->key > key) {
			left = root;
			root = &(*root)->left;
		} else {
			if ((*root)->right)
				return min(&(*root)->right);
			return left;
		}
	}

	return NULL;
}

struct bst **predecessor(struct bst **root, int key)
{
	struct bst **right = NULL;
	struct bst **e = NULL;

	while (*root) {
		struct bst *t = *root;

		if (t->key < key) {
			right = root;
			root = &(*root)->right;
		} else if (t->key > key) {
			root = &(*root)->left;
		} else {
			if ((*root)->left)
				return max(&(*root)->left);
			return right;
		}
	}

	return NULL;
}

void do_print_set(struct bst *t)
{
	if (t) {
		do_print_set(t->left);
		printf("%d, ", t->key);
		do_print_set(t->right);
	}
}

void print_set(struct bst **root)
{
	printf("{");
	do_print_set(*root);
	printf("}");
}

// --- Main Simulation ---

int main()
{
	// Initialize the random number generator
	srand(time(NULL));

	// Array to hold the numbers [1, 1000]
	int data[N_NODES];
	for (int i = 0; i < N_NODES; i++) {
		data[i] = i + 1; // Populate with 1 to 1000
	}

	double total_height = 0.0;
	int max_observed_height = 0;
	int min_observed_height = N_NODES;

	for (int i = 0; i < N_EXPERIMENTS; ++i) {
		struct bst *t;
		empty(&t);

		// 1. Shuffle the data array
		shuffle(data, N_NODES);

		// 2. Insert the shuffled data into the BST
		for (int j = 0; j < N_NODES; ++j) {
			insert(&t, data[j]);
		}

		// 3. Calculate height
		int current_height = height(t);
		total_height += (double)current_height;

		// Track min/max observed heights
		if (current_height > max_observed_height) {
			max_observed_height = current_height;
		}
		if (current_height < min_observed_height) {
			min_observed_height = current_height;
		}

		// 4. Clean up the tree for the next experiment (important for memory)
		destroy_tree(t);
	}

	double average_height = total_height / N_EXPERIMENTS;

	printf("--- BST Average Depth Experiment ---\n");
	printf("Nodes (N): %d\n", N_NODES);
	printf("Experiments Run: %d\n", N_EXPERIMENTS);
	printf("------------------------------------\n");
	printf("Theoretical Min Height (log2(N)): %d\n", (int)ceil(log2(N_NODES + 1)) - 1);
	printf("Min Observed Height: %d\n", min_observed_height);
	printf("Max Observed Height: %d\n", max_observed_height);
	printf("Average Depth/Height: %.2f\n", average_height);
	printf("------------------------------------\n");

	return 0;
}
