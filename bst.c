#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct bst {
	int key;
	struct bst *left, *right;
};

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

int main()
{
	int data[] = { 5, 4, 1, 10, 9, 3, 15, 2, 6 };
	struct bst *t;

	empty(&t);

	for (int i = 0; i < 9; ++i)
		insert(&t, data[i]);

	print_set(&t);
	printf("\n");

	for (int i = 0; i < 9; i += 2)
		delete(&t, data[i]);

	print_set(&t);
	printf("\n");

	return 0;
}
