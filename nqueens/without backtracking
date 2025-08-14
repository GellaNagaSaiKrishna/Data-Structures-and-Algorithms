#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h> 

void swap(size_t *a, size_t *b) {
    size_t temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(size_t *arr, size_t start, size_t end) {
    while (start < end) {
        swap(&arr[start], &arr[end]);
        ++start;
        --end;
    }
}

void first(size_t n, size_t *queens) {
    for (size_t i = 0; i < n; ++i)
        queens[i] = i;
}

bool next(size_t n, size_t *queens) {
    ssize_t i = n - 2;
    while (i >= 0 && queens[i] >= queens[i + 1])
        --i;
    if (i < 0)
        return false;

    size_t j = n - 1;
    while (queens[j] <= queens[i])
        --j;
    swap(&queens[i], &queens[j]);
    reverse(queens, i + 1, n - 1);
    return true;
}

bool ok(size_t n, size_t *queens) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            size_t d = (queens[i] > queens[j]) ? queens[i] - queens[j] : queens[j] - queens[i];
            if (d == j - i)  // Same diagonal
                return false;
        }
    }
    return true;
}

size_t nqueens(size_t n) {
    size_t c = 0;
    size_t *queens = malloc(n * sizeof(size_t));
    assert(queens);
    first(n, queens);
    while (true) {
        if (ok(n, queens))
            ++c;
        if (!next(n, queens))
            break;
    }
    free(queens);
    return c;
}

int main() {
    size_t n;
    printf("Enter value of n: ");
    scanf("%zu", &n);

    clock_t start = clock();

    size_t solutions = nqueens(n);


    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Number of solutions for %zu-Queens: %zu\n", n, solutions);
    printf("Time taken: %.6f seconds\n", time_spent);

    return 0;
}
