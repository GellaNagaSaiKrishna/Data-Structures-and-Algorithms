#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define FLAG_WIN  0x80  // 1<<7
#define FLAG_DRAW 0x40  // 1<<6
#define FLAG_LOSS 0x20  // 1<<5

static size_t pow_size_t(size_t base, size_t exponent)
{
    size_t pow = 1;
    for (size_t i = 0; i < exponent; i++) pow *= base;
    return pow;
}

static bool player_win(uint64_t m)
{
    if ((m & (m >> 1) & (m >> 2) & (m >> 3)) != 0) return true;  // horizontal
    if ((m & (m >> 5) & (m >> 10) & (m >> 15)) != 0) return true; // vertical
    if ((m & (m >> 6) & (m >> 12) & (m >> 18)) != 0) return true; // diag
    if ((m & (m >> 4) & (m >> 8) & (m >> 12)) != 0) return true;  // other diag
    return false;
}

static void print_board(size_t board[4][5])
{
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%zu ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Game state struct: bundle all the previously separate parameters */
typedef struct {
    size_t state;
    size_t player_computer;      // 1 or 2
    size_t board[4][5];         // board[row][col], row=0 is bottom
    size_t move;                // number of moves made so far
    size_t col_state[5];        // whatever meaning you used (keeps packed column info)
    size_t columns_filled[5];   // number of tokens in each column (0..4)
    uint64_t playercomp;        // bitmask for computer pieces
    uint64_t playeruser;        // bitmask for user pieces
    // visited omitted for now (see note)
} Game;

uint8_t explore(size_t state, size_t* columns_filled, size_t* col_state, uint8_t *visited,
    size_t player_computer, size_t player, uint64_t player_mask, uint64_t opp_mask, size_t moves)
{
    if (player_win(opp_mask))
    {
        visited[state] = FLAG_LOSS;
        return FLAG_LOSS;
    }
    if (moves == 20)
    {
        visited[state] = FLAG_DRAW;
        return FLAG_DRAW;
    }
    if (visited[state]) return visited[state];
    size_t losses = 0;
    size_t draws = 0;
    uint8_t draw_col = 0;
    uint8_t loss_col = 0;
    for (size_t i = 0; i < 5; i++)
    {
        if (columns_filled[i] < 4)
        {
            size_t change_col_state = player + col_state[i];
            col_state[i] += change_col_state;
            size_t board_state = state + change_col_state * pow_size_t(31, i);
            columns_filled[i]++;
            size_t pos = 5 * i + columns_filled[i] - 1;
            uint64_t updated_player_mask = player_mask | (1ULL << pos);
            if (!visited[board_state])
                explore(board_state, columns_filled, col_state, visited, player_computer, 3-player,
                opp_mask, updated_player_mask, moves+1);
            col_state[i] -= change_col_state;
            columns_filled[i]--;
            if (visited[board_state] & FLAG_LOSS)
            {
                visited[state] = FLAG_WIN | (i+1);
                return visited[state];
            }
            if (visited[board_state] & FLAG_DRAW)
            {
                draws++;
                draw_col = i+1;
            }
            else
            {
                losses++;
                loss_col = i+1;
            }
        }
    }
    if (draws > 0)
    {
        visited[state] = FLAG_DRAW | draw_col;
        return visited[state];
    }
    visited[state] = FLAG_LOSS | loss_col;
    return visited[state];
}

static size_t play_move(Game *g, uint8_t *visited)
{
    size_t columns_filled[5];
    size_t col_state[5];
    for (size_t i = 0; i < 5; i++)
    {
        col_state[i] = g->col_state[i];
        columns_filled[i] = g->columns_filled[i];
    }
    if (!visited[g->state])
    {
        explore(g->state, columns_filled, col_state, visited, g->player_computer, g->player_computer,
        g->playercomp, g->playeruser, g->move);
    }
    if (visited[g->state] & FLAG_WIN)
        return visited[g->state] ^ FLAG_WIN;
    if (visited[g->state] & FLAG_DRAW)
        return visited[g->state] ^ FLAG_DRAW;
    return visited[g->state] ^ FLAG_LOSS;
}

/* main game loop using Game* instead of many args */
static void play_connect4(Game *g, uint8_t *visited)
{
    while (true)
    {
        if (player_win(g->playercomp))
        {
            printf("You Lose!\n");
            return;
        }
        if (player_win(g->playeruser))
        {
            printf("You Win!\n");
            return;
        }
        if (g->move == 20) // 4*5 = 20 max moves => full board
        {
            printf("It's a Tie!\n");
            return;
        }
        if ((g->move % 2) != (g->player_computer % 2))
        {
            /* computer's turn */
            size_t move_column = play_move(g, visited);
            size_t col = move_column - 1;
            size_t row = g->columns_filled[col];

            g->board[row][col] = g->player_computer;
            g->columns_filled[col] += 1;
            size_t change_col_state = g->player_computer + g->col_state[col];
            g->col_state[col] += change_col_state;
            g->state += change_col_state * pow_size_t(31, col);
            size_t pos = 5 * col + row;
            g->playercomp |= (1ULL << pos);

            printf("Computer played column %zu\n", move_column);
            print_board(g->board);
        }
        else
        {
            /* user's turn */
            char user_colour;
            size_t move_column;
            printf("Enter colour (single char) and column (1-5): ");
            if (scanf(" %c %zu", &user_colour, &move_column) != 2) {
                fprintf(stderr, "Invalid input. Exiting.\n");
                return;
            }
            if (move_column < 1 || move_column > 5) {
                printf("Invalid column. Try again.\n");
                continue;
            }
            size_t col = move_column - 1;
            if (g->columns_filled[col] >= 4) {
                printf("Column full. Pick another column.\n");
                continue;
            }
            size_t player_user = (g->player_computer == 1) ? 2 : 1;
            size_t row = g->columns_filled[col];
            g->board[row][col] = player_user;
            g->columns_filled[col] += 1;
            size_t change_col_state = player_user + g->col_state[col];
            g->col_state[col] += change_col_state;
            g->state += change_col_state * pow_size_t(31, col);
            size_t pos = 5 * col + row;
            g->playeruser |= (1ULL << pos);

            print_board(g->board);
        }
        g->move += 1;
    }
}

void connect4()
{
    char player = ' ';
    size_t player_computer = 2;
    printf("Do you want to play first? (Y to be the computer?) ");
    if (scanf(" %c", &player) != 1) return;
    if (player == 'Y') player_computer = 1;

    Game g = {0};
    g.player_computer = player_computer;
    g.playercomp = 0;
    g.playeruser = 0;
    g.move = 0;
    g.state = 0;
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 5; ++j)
            g.board[i][j] = 0;
    for (size_t c = 0; c < 5; ++c) { g.columns_filled[c] = 0; g.col_state[c] = 0; }
    uint8_t *visited = calloc(pow_size_t(31, 5), sizeof(uint8_t));
    assert(visited);

    play_connect4(&g, visited);
    free(visited);
}

int main(void)
{
    connect4();
    return 0;
}
