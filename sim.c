#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

// Triangle definitions (triplets of line indices that form triangles)
const int triangles[20][3] = {
    {0, 1, 5},    // Triangle 1-2-3
    {0, 2, 6},    // Triangle 1-2-4
    {0, 3, 7},    // Triangle 1-2-5
    {0, 4, 8},    // Triangle 1-2-6
    {1, 2, 9},    // Triangle 1-3-4
    {1, 3, 10},   // Triangle 1-3-5
    {1, 4, 11},   // Triangle 1-3-6
    {2, 3, 12},   // Triangle 1-4-5
    {2, 4, 13},   // Triangle 1-4-6
    {3, 4, 14},   // Triangle 1-5-6
    {5, 6, 9},    // Triangle 2-3-4
    {5, 7, 10},   // Triangle 2-3-5
    {5, 8, 11},   // Triangle 2-3-6
    {6, 7, 12},   // Triangle 2-4-5
    {6, 8, 13},   // Triangle 2-4-6
    {7, 8, 14},   // Triangle 2-5-6
    {9, 10, 12},  // Triangle 3-4-5
    {9, 11, 13},  // Triangle 3-4-6
    {10, 11, 14}, // Triangle 3-5-6
    {12, 13, 14}  // Triangle 4-5-6
};

// Map line numbers to point connections
const char* line_connections[15] = {
    "1-2", "1-3", "1-4", "1-5", "1-6",  // Lines 0-4
    "2-3", "2-4", "2-5", "2-6",         // Lines 5-8
    "3-4", "3-5", "3-6",                // Lines 9-11
    "4-5", "4-6",                       // Lines 12-13
    "5-6"                               // Line 14
};

int has_won(board_t board, player_t player)
{
    // Check if the player has completed any triangle
    for (int i = 0; i < 20; i++) {
        if (board[triangles[i][0]] == player &&
            board[triangles[i][1]] == player &&
            board[triangles[i][2]] == player) {
            return 1; // Player has lost (completed a triangle)
        }
    }
    return 0; // Player hasn't lost yet
}

int is_full(board_t board)
{
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            return 0; // Board is not full
        }
    }
    return 1; // Board is full
}

player_t other_player(player_t player)
{
    return (player == RED) ? BLUE : RED;
}

int evaluate(board_t board, player_t player)
{
    if (has_won(board, player)) {
        return -1; // Current player loses
    }
    if (has_won(board, other_player(player))) {
        return 1; // Opponent loses (current player wins)
    }
    if (is_full(board)) {
        return 0; // Draw
    }
    return 0; // Game continues
}

move_t minimax(board_t board, player_t player, int depth, int alpha, int beta)
{
    move_t best_move;
    best_move.line = -1;
    best_move.score = -1000; // Initialize with worst possible score
    
    // If game is over, return evaluation
    int eval = evaluate(board, player);
    if (eval != 0 || is_full(board)) {
        best_move.score = eval;
        return best_move;
    }
    
    // Try all possible moves
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) { // If the line is empty
            // Make the move
            board[i] = player;
            
            // Recursively evaluate the move
            move_t current_move = minimax(board, other_player(player), depth + 1, -beta, -alpha);
            current_move.score = -current_move.score; // Negamax: opponent's score is negative for us
            current_move.line = i;
            
            // Undo the move
            board[i] = NO;
            
            // Update best move
            if (current_move.score > best_move.score || best_move.line == -1) {
                best_move = current_move;
            }
            
            // Alpha-beta pruning
            if (current_move.score > alpha) {
                alpha = current_move.score;
            }
            if (alpha >= beta) {
                break; // Beta cutoff
            }
        }
    }
    
    return best_move;
}

move_t best_move(board_t board, player_t player)
{
    return minimax(board, player, 0, INT_MIN, INT_MAX);
}

void print_graphical_board(board_t board)
{
    printf("\n   Graphical Representation:\n");
    printf("       1\n");
    printf("       /\\\n");
    printf("      /  \\\n");
    printf("     /    \\\n");
    printf("    /      \\\n");
    printf("   /        \\\n");
    printf(" 2 ———————— 3\n");
    printf("  \\        / \\\n");
    printf("   \\      /   \\\n");
    printf("    \\    /     \\\n");
    printf("     \\  /       \\\n");
    printf("      \\/         \\\n");
    printf("      4 ————————— 5\n");
    printf("       \\         /\n");
    printf("        \\       /\n");
    printf("         \\     /\n");
    printf("          \\   /\n");
    printf("           \\ /\n");
    printf("            6\n\n");
    
    printf("Lines and their connections:\n");
    for (int i = 0; i < 15; i++) {
        char color;
        switch (board[i]) {
            case RED: color = 'R'; break;
            case BLUE: color = 'B'; break;
            default: color = '.'; break;
        }
        printf("Line %2d (%s): %c\n", i, line_connections[i], color);
    }
}

void print_available_moves(board_t board)
{
    printf("\nAvailable moves:\n");
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            printf("  %2d: %s\n", i, line_connections[i]);
        }
    }
}

void check_triangles(board_t board, player_t player)
{
    printf("\nChecking triangles for player %c:\n", (player == RED) ? 'R' : 'B');
    int danger_count = 0;
    
    for (int i = 0; i < 20; i++) {
        int count = 0;
        int missing_line = -1;
        
        for (int j = 0; j < 3; j++) {
            if (board[triangles[i][j]] == player) {
                count++;
            } else if (board[triangles[i][j]] == NO) {
                missing_line = triangles[i][j];
            }
        }
        
        if (count == 2 && missing_line != -1) {
            printf("  WARNING: Triangle %d (lines %d,%d,%d) has 2 %c lines! ",
                   i, triangles[i][0], triangles[i][1], triangles[i][2], 
                   (player == RED) ? 'R' : 'B');
            printf("Don't draw line %d (%s) or you lose!\n", 
                   missing_line, line_connections[missing_line]);
            danger_count++;
        }
    }
    
    if (danger_count == 0) {
        printf("  No immediate danger - no triangles with 2 of your lines\n");
    }
}

int main()
{
    board_t board;
    player_t human, computer;
    char color_choice, turn_choice;
    
    // Initialize empty board
    for (int i = 0; i < 15; i++) {
        board[i] = NO;
    }
    
    // Let user choose color
    printf("Welcome to Sim Game!\n");
    printf("Choose your color (R for Red, B for Blue): ");
    scanf(" %c", &color_choice);
    
    if (color_choice == 'R' || color_choice == 'r') {
        human = RED;
        computer = BLUE;
        printf("You are Red, computer is Blue.\n");
    } else {
        human = BLUE;
        computer = RED;
        printf("You are Blue, computer is Red.\n");
    }
    
    // Let user choose turn order
    printf("\nDo you want to go first? (Y for Yes, N for No): ");
    scanf(" %c", &turn_choice);
    
    player_t current_player;
    if (turn_choice == 'Y' || turn_choice == 'y') {
        current_player = human;
        printf("You will go first!\n");
    } else {
        current_player = computer;
        printf("Computer will go first.\n");
    }
    
    printf("\nGame starting...\n");
    printf("Red always goes first in Sim. Remember: You LOSE if you complete a triangle of your own color!\n");
    
    while (1) {
        printf("\n" "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "=\n");
        
        // Display graphical representation
        print_graphical_board(board);
        
        // Check if game is over
        if (has_won(board, RED)) {
            printf("\nRED completed a triangle! BLUE wins!\n");
            break;
        }
        if (has_won(board, BLUE)) {
            printf("\nBLUE completed a triangle! RED wins!\n");
            break;
        }
        if (is_full(board)) {
            printf("\nIt's a draw! No triangles completed.\n");
            break;
        }
        
        if (current_player == human) {
            // Human's turn
            printf("\nYour turn (%c):\n", (human == RED) ? 'R' : 'B');
            check_triangles(board, human);
            print_available_moves(board);
            
            int move;
            printf("\nEnter line number to draw: ");
            scanf("%d", &move);
            
            if (move < 0 || move >= 15 || board[move] != NO) {
                printf("Invalid move! Try again.\n");
                continue;
            }
            
            board[move] = human;
            printf("You drew line %d (%s)\n", move, line_connections[move]);
        } else {
            // Computer's turn
            printf("\nComputer's turn (%c):\n", (computer == RED) ? 'R' : 'B');
            check_triangles(board, computer);
            move_t best = best_move(board, computer);
            printf("Computer chooses line %d (%s)\n", best.line, line_connections[best.line]);
            board[best.line] = computer;
        }
        
        // Switch turns
        current_player = other_player(current_player);
    }
    
    printf("\nFinal board:\n");
    print_graphical_board(board);
    printf("Game over!\n");
    
    return 0;
}
