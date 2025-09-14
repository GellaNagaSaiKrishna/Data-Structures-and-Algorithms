#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A structure to hold the permutation and auxiliary data for the Gray ordering.
typedef struct {
    size_t *p;   // permutation array
    int *d;      // direction array: -1 for left, +1 for right
    size_t n;
    bool has_next;
} GrayPermutation;

// Initializes the Gray ordering generator with the first permutation.
void first_gray(GrayPermutation *gp, size_t n) {
    gp->n = n;
    gp->p = malloc(n * sizeof(size_t));
    gp->d = malloc(n * sizeof(int));

    if (n > 0 && (!gp->p || !gp->d)) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < n; ++i) {
        gp->p[i] = i;
        gp->d[i] = -1; // Initial direction is left
    }
    gp->has_next = (n > 0);
}

// Generates the next Gray permutation.
// Returns true if a next permutation exists.
bool next_gray(GrayPermutation *gp) {
    if (!gp->has_next) return false;

    // 1. Find the largest mobile element
    long mobile_index = -1;
    size_t mobile_value = 0;

    for (long i = 0; i < (long)gp->n; ++i) {
        long adj = i + gp->d[i];
        if (adj >= 0 && adj < (long)gp->n) {
            if (gp->p[i] > gp->p[adj] && gp->p[i] > mobile_value) {
                mobile_value = gp->p[i];
                mobile_index = i;
            }
        }
    }

    // 2. If no mobile element, we are done
    if (mobile_index == -1) {
        gp->has_next = false;
        return false;
    }

    // 3. Swap mobile with adjacent in its direction
    long swap_index = mobile_index + gp->d[mobile_index];

    size_t temp_val = gp->p[mobile_index];
    gp->p[mobile_index] = gp->p[swap_index];
    gp->p[swap_index] = temp_val;

    int temp_dir = gp->d[mobile_index];
    gp->d[mobile_index] = gp->d[swap_index];
    gp->d[swap_index] = temp_dir;

    // 4. Reverse direction of all elements larger than the mobile
    for (long i = 0; i < (long)gp->n; ++i) {
        if (gp->p[i] > mobile_value) {
            gp->d[i] *= -1;
        }
    }

    return true;
}

// Print a permutation
void print_permutation(size_t *p, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        printf("%zu ", p[i]);
    }
    printf("\n");
}

int main() {
    size_t n;
    printf("Enter value of n: ");
    scanf("%zu", &n);

    GrayPermutation gp;
    first_gray(&gp, n);

    // Print all permutations
    if (gp.has_next) {
        print_permutation(gp.p, n);
        while (next_gray(&gp)) {
            print_permutation(gp.p, n);
        }
    }

    free(gp.p);
    free(gp.d);

    return 0;
}
