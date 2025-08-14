#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define N 9

void printGrid(int grid[N][N]) {
    for (int i = 0; i < N; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("---------------------\n");
        }
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

bool isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow + i][startCol + j] == num) return false;
        }
    }
    return true;
}

bool certaintySolve(int grid[N][N]) {
    bool changed = false;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                int count = 0, lastNum = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(grid, row, col, num)) {
                        count++;
                        lastNum = num;
                    }
                }
                if (count == 1) {
                    grid[row][col] = lastNum;
                    changed = true;
                }
            }
        }
    }
    return changed;
}

bool findEmptyCell(int grid[N][N], int *row, int *col) {
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (grid[*row][*col] == 0)
                return true;
        }
    }
    return false;
}

bool backtrackSolve(int grid[N][N]) {
    int row, col;
    if (!findEmptyCell(grid, &row, &col)) return true;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (backtrackSolve(grid)) return true;
            grid[row][col] = 0;
        }
    }
    return false;
}

void solveSudoku(int grid[N][N]) {
    bool backtrackingUsed = false;

    // Phase 1: Certainty method
    while (certaintySolve(grid));

    // Check if still incomplete
    int row, col;
    if (findEmptyCell(grid, &row, &col)) {
        backtrackingUsed = true;
        backtrackSolve(grid);
    }

    printGrid(grid);
    if (backtrackingUsed)
        printf("\nSolved using certainty + backtracking.\n");
    else
        printf("\nSolved using certainty only.\n");
}

int main() {
    int grid[N][N] = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},

        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},

        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    clock_t start = clock();
    solveSudoku(grid);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_taken);


    int grid2[N][N] = {
    {0,0,0,2,6,0,7,0,1},
    {6,8,0,0,7,0,0,9,0},
    {1,9,0,0,0,4,5,0,0},
    {8,2,0,1,0,0,0,4,0},
    {0,0,4,6,0,2,9,0,0},
    {0,5,0,0,0,3,0,2,8},
    {0,0,9,3,0,0,0,7,4},
    {0,4,0,0,5,0,0,3,6},
    {7,0,3,0,1,8,0,0,0}
};

    printf("Original Puzzle:\n");
    printGrid(grid2);
    

    clock_t start2 = clock();
    solveSudoku(grid);
    clock_t end2 = clock();
    double time_taken2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_taken2);
    return 0;
    
}
