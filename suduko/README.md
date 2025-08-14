# 🧩 Sudoku Solver

This folder contains multiple approaches to solving the Sudoku puzzle.  
We explore **certainty-based solving**, **backtracking**, and a **hybrid method** that combines both.

---

## 📌 Methods Implemented

### 1️⃣ Certainty Method
- Place a number in a cell **only when we are 100% sure** it is the only valid choice for that position.
- Works by repeatedly scanning the board for cells with a **single possible number**.
- **Advantages:**  
  - Extremely fast — nearly **100× faster** than pure backtracking.
  - Solves many easy and medium-level Sudokus instantly.
- **Limitations:**  
  - Cannot solve all Sudoku puzzles (especially harder ones with no immediate certainty moves).

---

### 2️⃣ Backtracking
- Try placing **every possible number** in each empty cell.
- If the partial solution fails, backtrack to the previous step and try another option.
- **Advantages:**  
  - Can solve **any valid Sudoku puzzle**.
- **Limitations:**  
  - Much slower compared to certainty method.

---

### 3️⃣ Hybrid Method
- Start with the **certainty method** to fill in as many cells as possible quickly.
- When certainty-based solving is no longer possible, switch to **backtracking**.
- **Benefits:**  
  - Reduces overall solving time.
  - Maintains the ability to solve **all** Sudoku puzzles.
  - Offers the best of both worlds — speed and accuracy.

---

## 📝 Conclusion
- **Certainty method** → Lightning-fast but limited coverage.  
- **Backtracking** → Perfect coverage but slower.  
- **Hybrid method** → Optimized speed **and** guaranteed solution.

