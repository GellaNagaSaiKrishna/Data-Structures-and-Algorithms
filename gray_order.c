#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A structure to hold the permutation and auxiliary data for the Gray ordering.
typedef struct {
    size_t *p;
    int *d; // Direction array: 1 for right, -1 for left
    size_t n;
    bool has_next;
} GrayPermutation;

// Initializes the Gray ordering generator with the first permutation.
// The first permutation is [0, 1, 2, ..., n-1].
void first_gray(GrayPermutation *gp, size_t n) {
    gp->n = n;
    gp->p = malloc(n * sizeof(size_t));
    gp->d = malloc(n * sizeof(int));

    if (!gp->p || !gp->d) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < n; ++i) {
        gp->p[i] = i;
        gp->d[i] = -1; // Initial direction is left for all elements
    }
    gp->has_next = true;
}

// Generates the next Gray permutation.
// Returns true if a next permutation was found, false otherwise.
bool next_gray(GrayPermutation *gp) {
    if (!gp->has_next) {
        return false;
    }

    // 1. Find the largest mobile element k. A mobile element is one whose direction arrow points to a smaller adjacent element.
    ssize_t mobile_index = -1;
    size_t mobile_value = 0;

    for (ssize_t i = 0; i < gp->n; ++i) {
        ssize_t adjacent_index = i + gp->d[i];
        if (adjacent_index >= 0 && adjacent_index < gp->n) {
            if (gp->p[i] > gp->p[adjacent_index] && gp->p[i] > mobile_value) {
                mobile_value = gp->p[i];
                mobile_index = i;
            }
        }
    }

    // 2. If no mobile element exists, we are at the last permutation.
    if (mobile_index == -1) {
        gp->has_next = false;
        return false;
    }

    // 3. Swap the mobile element k with its adjacent element in the direction of its arrow.
    ssize_t swap_index = mobile_index + gp->d[mobile_index];
    size_t temp_val = gp->p[mobile_index];
    gp->p[mobile_index] = gp->p[swap_index];
    gp->p[swap_index] = temp_val;
    
    int temp_dir = gp->d[mobile_index];
    gp->d[mobile_index] = gp->d[swap_index];
    gp->d[swap_index] = temp_dir;

    // 4. Reverse the direction of all elements larger than the mobile element.
    for (ssize_t i = 0; i < gp->n; ++i) {
        if (gp->p[i] > mobile_value) {
            gp->d[i] *= -1;
        }
    }

    return true;
}

// Prints a permutation.
void print_permutation(size_t *p, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        printf("%zu", p[i]);
    }
    printf("\n");
}

int main() {
    size_t n;
    printf("Enter value of n: ");
    scanf("%zu", &n);

    GrayPermutation gp;
    first_gray(&gp, n);

    // Print all permutations in the Gray ordering.
    print_permutation(gp.p, n);
    while (next_gray(&gp)) {
        print_permutation(gp.p, n);
    }
    
    // Cleanup allocated memory.
    free(gp.p);
    free(gp.d);

    return 0;
}
