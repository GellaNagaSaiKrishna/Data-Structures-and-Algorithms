#include <stdio.h>
#include <stdlib.h>

void print(int *arr, size_t k) {
    for (size_t i = 0; i < k; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void do_permutations_dup_correct(
    int *a, size_t n,
    int *b, size_t k,
    size_t k1,
    int *used
) {
    if (k1 == k) {
        print(b, k);
        return;
    }
    
    for (size_t i = 0; i < n; ++i) {
        if (used[i]) {
            continue;
        }
        
        // Skip duplicates: if same value appeared earlier and wasn't used
        int duplicate = 0;
        for (size_t j = 0; j < i; j++) {
            if (a[j] == a[i] && !used[j]) {
                duplicate = 1;
                break;
            }
        }
        if (duplicate) {
           continue;
        }
        
        used[i] = 1;
        b[k1] = a[i];
        do_permutations_dup_correct(a, n, b, k, k1+1, used);
        used[i] = 0;
    }
}

void permutations_dup_correct(
    int *a, size_t n,
    int *b, size_t k
) {
    int *used = calloc(n, sizeof(int));
    do_permutations_dup_correct(a, n, b, k, 0, used);
    free(used);
}

int main() {
    int a[] = {1, 2, 2};
    size_t n = sizeof(a) / sizeof(a[0]);
    int b[n];
    
    printf("Unique permutations of [1, 2, 2]:\n");
    permutations_dup_correct(a, n, b, n);
    
    return 0;
}