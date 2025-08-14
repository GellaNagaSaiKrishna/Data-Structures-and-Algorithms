#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define N 9
void printGrid(int grid[N][N]){
    for(int i=0;i<N;i++){
        if(i%3==0 && i!=0){
            printf("---------------------\n");
        }        
        for(int j=0;j<N;j++){
            if(j%3==0 && j!=0){
                printf("| ");
            }
            printf("%d ",grid[i][j]);
        }
        printf("\n");
    }
}

bool issafe(int grid[N][N],int row,int col,int num){
    for(int i=0;i<N;i++){
        if(grid[row][i]==num) return false; // Row check
        if(grid[i][col]==num) return false; // Column check
    }
    for(int i=0;i<3;i++){
        for(int j = 0;j<3;j++){
            int x = row - row%3+i;
            int y = col - col%3+j;
            if(grid[x][y]==num) return false; // 3x3 box check
        }
    }
    return true;
}

bool fillcertaincells(int grid[N][N]){
    bool filled = false;
    for(int row=0;row<N;row++){
        for(int col=0;col<N;col++){
            if(grid[row][col]==0){
                int count = 0;
                int candidate = 0;
                for(int num=1;num<=9;num++){
                    if(issafe(grid,row,col,num)){
                        count++;
                        candidate = num;
                        if(count > 1) break; // More than one candidate
                    }
                }
                if(count == 1){ // Exactly one candidate
                    grid[row][col] = candidate;
                    filled = true;
                }
            }
        }
    }
    return filled;
}

bool solveSudokuCertainOnly(int grid[N][N]) {
    while (fillcertaincells(grid));
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                return false; // If any cell is still empty, return false
            }
        }
    }
    return true; // All cells filled
}

int main() {
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    printf("Initial Sudoku:\n");
    printGrid(grid);
    clock_t start = clock();
    if (solveSudokuCertainOnly(grid)) {
        printf("\nSolved (only certain moves):\n");
        printGrid(grid);
    } else {
        printf("\nCannot solve completely with only certain moves.\n");
        printGrid(grid);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC; 
    printf("Time taken: %f seconds\n", time_spent);
    return 0;
}
