#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define MAX_QUEUE_SIZE 1000
struct avl {
	int key;
	struct avl *left, *right;
	int8_t height;
};

void unreachable(){
    abort();
}
int8_t height(struct avl *n)
{
	if (n)
		return n->height;
	return 0;
}

int8_t max_height(int8_t x, int8_t y)
{
	return x < y ? y : x;
}

int8_t height_from_children(struct avl *n)
{
	return max_height(height(n->left), height(n->right)) + 1;
}

void empty(struct avl **t)
{
	*t = NULL;
}

bool is_empty(struct avl **t)
{
	return *t == NULL;
}

struct avl *node(int k)
{
	struct avl *n = malloc(sizeof(struct avl));
	n->key = k;
	n->left = NULL;
	n->right = NULL;
	n->height = 1;
	return n;
}

struct avl **search(struct avl **root, int key)
{
	while (*root) {
		struct avl *t = *root;

		if (t->key == key)
			return root;

		if (t->key < key)
			root = &(*root)->right;

		if (t->key > key)
			root = &(*root)->left;
	}
	return root;
}

struct avl **min(struct avl **root)
{
	while (*root) {
		struct avl *t = *root;
		if (t->left == NULL)
			return root;
		root = &(*root)->left;
	}
	unreachable();
}

struct avl **max(struct avl **root)
{
	while (*root) {
		struct avl *t = *root;
		if (t->right == NULL)
			return root;
		root = &(*root)->right;
	}
	unreachable();
}

void rotate_left(struct avl **n)
{
	struct avl *x = *n;
	struct avl *y = x->right;
	struct avl *z = y->left;

	*n = y;
	y->left = x;
	x->right = z;

	x->height = height_from_children(x);
	y->height = height_from_children(y);
}

void rotate_right(struct avl **n)
{
	struct avl *x = *n;
	struct avl *y = x->left;
	struct avl *z = y->right;

	*n = y;
	y->right = x;
	x->left = z;

	x->height = height_from_children(x);
	y->height = height_from_children(y);
}

void balance(struct avl **np)
{
	struct avl *n = *np;

	if (!n)
		return;

	int8_t bfn = height(n->left) - height(n->right);

	if (bfn > 1) {
		struct avl *l = n->left;
		int8_t bfl = height(l->left) - height(l->right);
		if (bfl >= 0) {
			rotate_right(np);
		} else {
			rotate_left(&n->left);
			rotate_right(np);
		}
	} else if (bfn < -1) {
		struct avl *r = n->right;
		int8_t bfr = height(r->left) - height(r->right);
		if (bfr <= 0) {
			rotate_left(np);
		} else {
			rotate_right(&n->right);
			rotate_left(np);
		}
	}

	n->height = height_from_children(n);
}

void insert(struct avl **np, int key)
{
	struct avl *n = *np;

	if (!n) {
		*np = node(key);
		return;
	}

	if (n->key < key)
		insert(&(*np)->right, key);
	else if (n->key > key)
		insert(&(*np)->left, key);
	else
		return;

	balance(np);
}

struct avl *unlink(struct avl **n)
{
	struct avl *save = *n;

	if ((*n)->left == NULL)
		*n = (*n)->right;
	else if ((*n)->right == NULL)
		*n = (*n)->left;
	else
		unreachable();

	return save;
		
}

struct avl *delete(struct avl **np, int key)
{
	struct avl *n = *np;
	struct avl *d;

	if (!n)
		return NULL;

	if (n->key < key)
		d = delete(&(*np)->right, key);
	else if (n->key > key)
		d = delete(&(*np)->left, key);
	else {
		if (n->left == NULL || n->right == NULL) {
			d = unlink(np);
			if (*np)
				(*np)->height = height_from_children(*np);
		} else {
			struct avl **m = max(&n->left);
			int save = (*m)->key;
			d = delete(np, save);
			n->key = save;
		}
	}

	balance(np);

	return d;
}

struct avl **successor(struct avl **root, int key)
{
	struct avl **left = NULL;
	struct avl **e = NULL;

	while (*root) {
		struct avl *t = *root;

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

struct avl **predecessor(struct avl **root, int key)
{
	struct avl **right = NULL;
	struct avl **e = NULL;

	while (*root) {
		struct avl *t = *root;

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

void do_print_set(struct avl *t)
{
	if (t) {
		do_print_set(t->left);
		printf("%d, ", t->key);
		do_print_set(t->right);
	}
}

void print_set(struct avl **root)
{
	printf("{");
	do_print_set(*root);
	printf("}");
}

void sert(int*arr,struct avl**t,int start,int end){
    if(start>end){
        return;
    }
    int mid=start+(end-start)/2;
    insert(t,arr[mid]);
    sert(arr,t,start,mid-1);
    sert(arr,t,mid+1,end);
}



struct queue {
        struct avl* elts[MAX_QUEUE_SIZE];
        size_t head;
        size_t tail;
        size_t len;
};

void init_queue(struct queue *q)
{
        q->head = 0;
        q->tail = 0;
        q->len = 0;
}

void enqueue(struct queue *q, struct avl* elt)
{
        assert(q->len < MAX_QUEUE_SIZE);
        q->elts[q->tail] = elt;
        q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
        ++(q->len);
}

struct avl* dequeue(struct queue *q)
{
        assert(q->len > 0);
        struct avl* u = q->elts[q->head];
        q->head = (q->head + 1) % MAX_QUEUE_SIZE;
        --(q->len);
        return u;
}

bool is_bfs_empty(struct queue *q)
{
        return q->len == 0;
}



int main()
{
	struct avl *t;
	empty(&t);
    int n;
    scanf("%d",&n);
    int arr[n];
    for(int i=0;i<n;i++){
    scanf("%d",&arr[i]);
    }
     int maxi=arr[0];
        for(int i=1;i<n;i++){
            if(maxi<arr[i]){maxi=arr[i];}
        }
    
    
    sert(arr,&t,0,n-1);
    struct queue q;
    init_queue(&q);
    enqueue(&q,t);
    while(!(is_bfs_empty(&q))){
        struct avl *x = dequeue(&q);
        
        if (x) { 
            
            printf("%d ", x->key); 
            if(x->key==maxi){return 0;}
            
            enqueue(&q, x->left);
            enqueue(&q, x->right);
        }
        else{printf("null ");}
       
    }

	return 0;
}
