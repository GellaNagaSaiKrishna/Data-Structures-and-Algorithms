#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#ifdef _WIN32
typedef long ssize_t;
#endif

struct node {
    size_t n;
    size_t *queens; // queens[row] = column index
    size_t k;       // current row
    ssize_t last;   // last tried column in this row
};

void root(size_t n, struct node *p) {
    p->n = n;
    p->queens = malloc(n * sizeof(size_t));
    assert(p->queens);
    p->k = 0;
    p->last = -1;
}

bool leaf(struct node *p) {
    return p->k == p->n;
}

bool ok(struct node *p) {
    size_t row = p->k - 1;     // last placed queen
    size_t col = p->queens[row];
    for (size_t i = 0; i < row; i++) {
        size_t qcol = p->queens[i];
        if (qcol == col) return false;                            // same column
        if ((ssize_t)qcol - (ssize_t)i == (ssize_t)col - (ssize_t)row) return false; // ↘ diagonal
        if ((ssize_t)qcol + (ssize_t)i == (ssize_t)col + (ssize_t)row) return false; // ↗ diagonal
    }
    return true;
}

bool up(struct node *p) {
    if (p->k == 0) return false;
    p->k--;
    p->last = p->queens[p->k];
    return true;
}

bool down(struct node *p) {
    for (size_t i = p->last + 1; i < p->n; i++) {
        p->queens[p->k] = i;
        p->last = i;
        p->k++;
        if (ok(p)) {  // check immediately after placing
            p->last = -1;
            return true;
        }
        p->k--; // undo if not valid
    }
    return false;
}

size_t nqueens(size_t n) {
    struct node p;
    root(n, &p);
    size_t count = 0;

    while (true) {
        if (leaf(&p)) {
            count++; // full solution found
            if (!up(&p)) break;
        } else {
            if (!down(&p)) {
                if (!up(&p)) break;
            }
        }
    }

    free(p.queens);
    return count;
}

int main() {
    size_t n;
    printf("Enter the value of n (number of queens): ");
    scanf("%zu", &n);
    size_t result = nqueens(n);
    printf("Number of solutions for %zu queens: %zu\n", n, result);
    return 0;
}
