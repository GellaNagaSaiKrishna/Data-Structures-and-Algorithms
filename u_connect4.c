#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define win  0x80
#define draw 0x40
#define loss 0x20

static size_t power(size_t base, size_t exponent) {
    size_t result = 1;
    for (size_t idx = 0; idx < exponent; ++idx) result *= base;
    return result;
}

static bool has_winning_line(uint64_t bits) {
    if ((bits & (bits >> 1) & (bits >> 2) & (bits >> 3)) != 0) return true;
    if ((bits & (bits >> 5) & (bits >> 10) & (bits >> 15)) != 0) return true;
    if ((bits & (bits >> 6) & (bits >> 12) & (bits >> 18)) != 0) return true;
    if ((bits & (bits >> 4) & (bits >> 8) & (bits >> 12)) != 0) return true;
    return false;
}

static void display_board(size_t grid[4][5]) {
    for (int row = 3; row >= 0; --row) {
        for (int col = 0; col < 5; ++col) {
            printf("%zu ", grid[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

typedef struct {
    size_t encoded_state;
    size_t computer_player;
    size_t grid[4][5];
    size_t turn_count;
    size_t state_per_column[5];
    size_t tokens_in_column[5];
    uint64_t mask_computer;
    uint64_t mask_user;
} GameState;

uint8_t solve_game(size_t encoded_state, size_t* tokens_in_column, size_t* state_per_column, uint8_t* visited_states,
    size_t computer_player, size_t playing_player, uint64_t mask_current, uint64_t mask_opponent, size_t moves_played) {

    if (has_winning_line(mask_opponent)) {
        visited_states[encoded_state] = loss;
        return loss;
    }
    if (moves_played == 20) {
        visited_states[encoded_state] = draw;
        return draw;
    }
    if (visited_states[encoded_state]) return visited_states[encoded_state];

    size_t count_losses = 0, count_draws = 0;
    uint8_t draw_move = 0, loss_move = 0;

    for (size_t col = 0; col < 5; ++col) {
        if (tokens_in_column[col] < 4) {
            size_t move_state_in_col = playing_player + state_per_column[col];
            state_per_column[col] += move_state_in_col;
            size_t updated_state = encoded_state + move_state_in_col * power(31, col);
            tokens_in_column[col]++;
            size_t board_pos = 5 * col + tokens_in_column[col] - 1;
            uint64_t updated_mask = mask_current | (1ULL << board_pos);

            if (!visited_states[updated_state])
                solve_game(updated_state, tokens_in_column, state_per_column, visited_states, computer_player,
                           3 - playing_player, mask_opponent, updated_mask, moves_played + 1);

            state_per_column[col] -= move_state_in_col;
            tokens_in_column[col]--;

            if (visited_states[updated_state] & loss) {
                visited_states[encoded_state] = win | (col + 1);
                return visited_states[encoded_state];
            }
            if (visited_states[updated_state] & draw) {
                count_draws++;
                draw_move = col + 1;
            } else {
                count_losses++;
                loss_move = col + 1;
            }
        }
    }
    if (count_draws > 0) {
        visited_states[encoded_state] = draw | draw_move;
        return visited_states[encoded_state];
    }
    visited_states[encoded_state] = loss | loss_move;
    return visited_states[encoded_state];
}

static size_t select_move(GameState* game, uint8_t* visited_states) {
    size_t tokens_in_column[5], state_per_column[5];
    for (size_t col = 0; col < 5; ++col) {
        state_per_column[col] = game->state_per_column[col];
        tokens_in_column[col] = game->tokens_in_column[col];
    }
    if (!visited_states[game->encoded_state]) {
        solve_game(game->encoded_state, tokens_in_column, state_per_column, visited_states,
            game->computer_player, game->computer_player, game->mask_computer, game->mask_user, game->turn_count);
    }
    if (visited_states[game->encoded_state] & win)
        return visited_states[game->encoded_state] ^ win;
    if (visited_states[game->encoded_state] & draw)
        return visited_states[game->encoded_state] ^ draw;
    return visited_states[game->encoded_state] ^ loss;
}

static void play_game(GameState* game, uint8_t* visited_states) {
    while (true) {
        if (has_winning_line(game->mask_computer)) {
            printf("You Lose!\n");
            break;
        }
        if (has_winning_line(game->mask_user)) {
            printf("You Win!\n");
            break;
        }
        if (game->turn_count == 20) {
            printf("It's a Tie!\n");
            break;
        }

        // Computer's turn: computer_player=2 means human starts first
        int computer_turn = ((game->turn_count % 2) != (3 - game->computer_player) % 2);
        if (computer_turn) {
            size_t computer_play = select_move(game, visited_states);
            size_t move_col = computer_play - 1;
            size_t move_row = game->tokens_in_column[move_col];
            game->grid[move_row][move_col] = game->computer_player;
            game->tokens_in_column[move_col]++;
            size_t move_col_state = game->computer_player + game->state_per_column[move_col];
            game->state_per_column[move_col] += move_col_state;
            game->encoded_state += move_col_state * power(31, move_col);
            size_t mask_pos = 5 * move_col + move_row;
            game->mask_computer |= (1ULL << mask_pos);
            printf("Computer played column %zu\n", computer_play);
            display_board(game->grid);
        } else {
            char player_token;
            size_t player_column;
            printf("Enter colour (single char) and column (1-5): ");
            if (scanf(" %c %zu", &player_token, &player_column) != 2) {
                fprintf(stderr, "Invalid input. Exiting.\n");
                return;
            }
            if (player_column < 1 || player_column > 5) {
                printf("Invalid column. Try again.\n");
                continue;
            }
            size_t col_idx = player_column - 1;
            if (game->tokens_in_column[col_idx] >= 4) {
                printf("Column full. Pick another column.\n");
                continue;
            }
            size_t user_player = (game->computer_player == 1) ? 2 : 1;
            size_t row_idx = game->tokens_in_column[col_idx];
            game->grid[row_idx][col_idx] = user_player;
            game->tokens_in_column[col_idx]++;
            size_t move_col_state = user_player + game->state_per_column[col_idx];
            game->state_per_column[col_idx] += move_col_state;
            game->encoded_state += move_col_state * power(31, col_idx);
            size_t mask_pos = 5 * col_idx + row_idx;
            game->mask_user |= (1ULL << mask_pos);
            display_board(game->grid);
        }
        game->turn_count += 1;
    }
}

void connect_four(void) {
    char choice;
    size_t computer_player = 2;
    printf("Do you want to play first? (Y for 'Yes' and N for 'No'): ");
    if (scanf(" %c", &choice) != 1) return;
    if (choice == 'Y') computer_player = 1; // user is player 1 if Y; computer is player 2
    else if(choice =='N') computer_player = 2;  
    else {printf("Invalid input"); 
    return;}// computer is player 1 otherwise

    GameState game = {0};
    game.computer_player = computer_player;
    game.turn_count = 0;
    game.encoded_state = 0;
    game.mask_computer = 0;
    game.mask_user = 0;

    for (size_t r = 0; r < 4; ++r)
        for (size_t c = 0; c < 5; ++c)
            game.grid[r][c] = 0;
    for (size_t c = 0; c < 5; ++c) {
        game.tokens_in_column[c] = 0;
        game.state_per_column[c] = 0;
    }
    uint8_t* visited_states = (uint8_t*)calloc(power(31, 5), sizeof(uint8_t));
    assert(visited_states);
    play_game(&game, visited_states);
    free(visited_states);
}

int main(void) {
    connect_four();
    return 0;
}
