# Optimal Connect 4 (5x4)

This program implements optimal Connect-4 on a 5-column by 4-row board. The game is played via standard input and output as follows:

- The first line is either `Y` (for Yellow, first player) or `R` (for Red, second player), indicating which piece the computer should play.
- Moves are alternated between the program and the user, with each move on a new line in the format:  
  `X n`  
  where `X` is the color (`Y` or `R`) making the move and `n` (1..5) is the column to drop the piece.
- When the game is over, the program prints either  
  `X won`  
  for the winner's color, or  
  `draw`  
  and then exits.

**Optimality:**  
- The first player cannot win if the second is optimal.
- The first player, if optimal, cannot lose.
- The code always plays optimally.

### Example Usage

