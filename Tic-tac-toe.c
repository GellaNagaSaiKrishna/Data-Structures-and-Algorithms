#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE (4)

enum { LOSS = 1, DRAW = 2, WIN = 4 };

size_t power(size_t a, size_t n) {
    size_t p = 1;
    while (n-- > 0)
        p *= a;
    return p;
}

int mark(uint32_t board, size_t r, size_t c) {
    size_t n = BOARD_SIZE * r + c;
    return (board / power(3, n)) % 3;
}

uint32_t place(uint32_t board, size_t r, size_t c, int p) {
    size_t n = BOARD_SIZE * r + c;
    return board + p * power(3, n);
}

bool is_full(uint32_t board) {
    for (size_t r = 0; r < BOARD_SIZE; ++r)
        for (size_t c = 0; c < BOARD_SIZE; ++c)
            if (!mark(board, r, c))
                return false;
    return true;
}

bool has_won(uint32_t board, int m) {
    for (size_t r = 0; r < BOARD_SIZE; ++r) {
        for (size_t c = 0; c < BOARD_SIZE; ++c) {
            if (mark(board, r, c) != m)
                goto next_row;
        }
        return true;
    next_row:;
    }
    for (size_t c = 0; c < BOARD_SIZE; ++c) {
        for (size_t r = 0; r < BOARD_SIZE; ++r) {
            if (mark(board, r, c) != m)
                goto next_col;
        }
        return true;
    next_col:;
    }
    for (size_t r = 0; r < BOARD_SIZE; ++r)
        if (mark(board, r, r) != m)
            goto other_diagonal;
    return true;
other_diagonal:
    for (size_t r = 0; r < BOARD_SIZE; ++r)
        if (mark(board, r, BOARD_SIZE - r - 1) != m)
            return false;
    return true;
}

bool is_done(uint32_t board) {
    return (
        is_full(board)
        || has_won(board, 1)
        || has_won(board, 2)
    );
}

uint8_t move(uint8_t result, size_t r, size_t c) {
    return (result << 4) | (r << 2) | c;
}

bool is_loss(uint8_t move) {
    return move & 0x10;
}

bool is_draw(uint8_t move) {
    return move & 0x20;
}

bool is_win(uint8_t move) {
    return move & 0x40;
}

int player(uint32_t board) {
    size_t marks[3] = {0};
    for (size_t r = 0; r < BOARD_SIZE; ++r)
        for (size_t c = 0; c < BOARD_SIZE; ++c)
            marks[mark(board, r, c)]++;
    return (marks[1] <= marks[2]) ? 1 : 2;
}

uint8_t ttt(uint32_t board, uint8_t *best) {
    assert(!is_done(board));

    if (best[board])
        return best[board];

    int p = player(board);
    uint8_t b = 0;

    for (size_t r = 0; r < BOARD_SIZE; ++r)
        for (size_t c = 0; c < BOARD_SIZE; ++c)
            if (!mark(board, r, c)) {
                uint32_t new_board = place(board, r, c, p);
                if (has_won(new_board, p))
                    return best[board] = move(WIN, r, c);
                if (is_done(new_board))
                    return best[board] = move(DRAW, r, c);
                uint8_t f = ttt(new_board, best);
                if (is_loss(f))
                    return best[board] = move(WIN, r, c);
                if (is_draw(f))
                    b = move(DRAW, r, c);
                else if (!b)
                    b = move(LOSS, r, c);
            }
    assert(b);
    return best[board] = b;
}

void print_board(uint32_t board) {
    printf("\nCurrent Board:\n");
    for (size_t r = 0; r < BOARD_SIZE; ++r) {
        for (size_t c = 0; c < BOARD_SIZE; ++c) {
            int m = mark(board, r, c);
            if (m == 0) printf(". ");
            else if (m == 1) printf("X ");
            else printf("O ");
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    uint8_t *best = calloc(power(3, 16), sizeof(uint8_t));
    uint32_t board = 0;
    int player_first;

    printf("Do you want to play first? (0 = Yes, 1 = No): ");
    scanf("%d", &player_first);

    print_board(board);

    while (!is_done(board)) {
        int current_player = player(board);

        if ((player_first && current_player == 2) || (!player_first && current_player == 1)) {
            
            size_t r, c;
            printf("Your move (row col): ");
            scanf("%zu %zu", &r, &c);
            if (mark(board, r, c) != 0) {
                printf("Invalid move! Cell already occupied. Try again.\n");
                continue;
            }
            board = place(board, r, c, current_player);
        } else {
           
            uint8_t ai_move = ttt(board, best);
            size_t r = (ai_move >> 2) & 3;
            size_t c = ai_move & 3;
            printf("Computer plays: %zu %zu\n", r, c);
            board = place(board, r, c, current_player);
        }

        print_board(board);
    }

   
    if (has_won(board, 1))
        printf("Player X (usually AI) wins!\n");
    else if (has_won(board, 2))
        printf("Player O (usually Human) wins!\n");
    else
        printf("It's a draw!\n");

    free(best);
    return 0;
}
