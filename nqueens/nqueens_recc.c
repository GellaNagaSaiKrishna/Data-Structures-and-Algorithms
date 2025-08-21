#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

bool ok(size_t *queens, size_t k) {
    if (k <= 1) return true;
    
    size_t last_row = k - 1;
    size_t last_col = queens[last_row];
    
    for (size_t i = 0; i < last_row; ++i) {
        size_t row = i;
        size_t col = queens[i];
        
        if (col == last_col) {
            return false;
        }
        
        if (abs((int)last_row - (int)row) == abs((int)last_col - (int)col)) {
            return false;
        }
    }
    
    return true;
}

size_t do_nqueens(size_t n, size_t *queens, size_t k) {
    if (k == n && ok(queens, k))
        return 1;
    
    if (k > 0 && !ok(queens, k))
        return 0;
    
    bool *used = malloc(n * sizeof(bool));
    assert(used);
    
    for (size_t i = 0; i < n; ++i)
        used[i] = false;
    
    for (size_t i = 0; i < k; ++i)
        used[queens[i]] = true;
    
    size_t count = 0;
    for (size_t i = 0; i < n; ++i) {
        if (!used[i]) {
            queens[k] = i;
            count += do_nqueens(n, queens, k + 1);
        }
    }
    
    free(used);
    return count;
}

size_t nqueens(size_t n) {
    if (n == 0) return 0;
    
    size_t *queens = malloc(n * sizeof(size_t));
    assert(queens);
    size_t count = do_nqueens(n, queens, 0);
    free(queens);
    return count;
}

int main() {
    size_t n;
    printf("Enter the board size (n): ");
    scanf("%zu", &n);
    
    size_t solutions = nqueens(n);
    printf("Number of solutions for %zu-queens: %zu\n", n, solutions);
    
    return 0;
}