#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct position {
    int row;
    int col;
};

struct move {
    struct position pos;
    int onward_moves;
};

int n, m, r, c;
int board[8][8];

struct position knight_moves[8] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
};

bool is_valid(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < m && board[x][y] == 0);
}

int count_forward_moves(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + knight_moves[i].row;
        int ny = y + knight_moves[i].col;
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && board[nx][ny] == 0) {
            count++;
        }
    }
    return count;
}

int compare_moves(const void *a, const void *b) {
    return ((struct move *)a)->onward_moves - ((struct move *)b)->onward_moves;
}

bool solve(int x, int y, int move_count) {
    board[x][y] = move_count;

    if (move_count == n * m) {
        for (int i = 0; i < 8; i++) {
            int nx = x + knight_moves[i].row;
            int ny = y + knight_moves[i].col;
            if (nx == r && ny == c) {
                return true;
            }
        }
        board[x][y] = 0;
        return false;
    }

    struct move valid_moves[8];
    int num_valid_moves = 0;

    for (int i = 0; i < 8; i++) {
        int nx = x + knight_moves[i].row;
        int ny = y + knight_moves[i].col;
        if (is_valid(nx, ny)) {
            valid_moves[num_valid_moves].pos.row = nx;
            valid_moves[num_valid_moves].pos.col = ny;
            valid_moves[num_valid_moves].onward_moves = count_forward_moves(nx, ny);
            num_valid_moves++;
        }
    }

    qsort(valid_moves, num_valid_moves, sizeof(struct move), compare_moves);

    for (int i = 0; i < num_valid_moves; i++) {
        if (solve(valid_moves[i].pos.row, valid_moves[i].pos.col, move_count + 1)) {
            return true;
        }
    }

    board[x][y] = 0;
    return false;
}

int main() {
    printf("Enter number of rows (n): ");
    scanf("%d", &n);
    printf("Enter number of columns (m): ");
    scanf("%d", &m);
    printf("Enter starting row index (r): ");
    scanf("%d", &r);
    printf("Enter starting column index (c): ");
    scanf("%d", &c);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board[i][j] = 0;
        }
    }

    if (solve(r, c, 1)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%d ", board[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("-1\n");
    }

    return 0;
}
