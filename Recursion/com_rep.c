#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort
int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void do_combinations(int *a, size_t n, int *b, size_t k, size_t k1) {
    if (n < k - k1) {
        return;
    }
    if (k1 == k) {
        for (size_t i = 0; i < k; i++) {
            printf("%d ", b[i]);
        }
        printf("\n");
        return;
    }

    for (size_t i = 0; i < n; i++) {
        // Skip duplicates
        if (i > 0 && a[i] == a[i-1]) {
            continue;
        }
        b[k1] = a[i];
        do_combinations(a + i + 1, n - i - 1, b, k, k1 + 1);
    }
}

void combinations(int *a, size_t n, int *b, size_t k) {
    // Sort first to group duplicates
    qsort(a, n, sizeof(int), cmpfunc);
    do_combinations(a, n, b, k, 0);
}

int main() {
    int a[] = {1, 2, 3, 3};
    size_t n = sizeof(a) / sizeof(a[0]);
    size_t k = 2;
    int *b = (int *)malloc(k * sizeof(int));

    printf("Unique combinations of size %zu:\n", k);
    combinations(a, n, b, k);

    free(b);
    return 0;
}
