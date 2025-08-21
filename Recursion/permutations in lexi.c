#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print(int *arr, size_t k) {
    for (size_t i = 0; i < k; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sort(int *arr, size_t n) {
    for (size_t i = 0; i < n-1; i++) {
        for (size_t j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void do_permutations_lex(
    int *a, size_t n,
    int *b, size_t k,
    size_t k1
) {
    if (k1 == k) {
        print(b, k);
        return;
    }

    int *sorted_a = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        sorted_a[i] = a[i];
    }
    sort(sorted_a, n);
    
    for (size_t i = 0; i < n; i++) {
        size_t pos = 0;
        while (pos < n && a[pos] != sorted_a[i]) {
            pos++;
        }
        

        if (i > 0 && sorted_a[i] == sorted_a[i-1]) {
            continue;
        }
        
        swap(&a[0], &a[pos]);
        b[k1] = a[0];
        do_permutations_lex(a+1, n-1, b, k, k1+1);
        swap(&a[0], &a[pos]);
    }
    
    free(sorted_a);
}

void permutations_lex(
    int *a, size_t n,
    int *b, size_t k
) {

    sort(a, n);
    do_permutations_lex(a, n, b, k, 0);
}


int main() {
    int a[] = {3, 1, 2};
    size_t n = sizeof(a) / sizeof(a[0]);
    int b[n];
    printf("Lexicographical permutations:\n");
    permutations_lex(a, n, b, n);
    
    return 0;
}