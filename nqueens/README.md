# ♛ N-Queens Problem

The **N-Queens** problem is a classic combinatorial challenge:  
Place **N queens** on an `N × N` chessboard such that **no two queens attack each other**.  
That means:
- No two queens can be in the same row.
- No two queens can be in the same column.
- No two queens can be on the same diagonal.

---

## 📌 Problem Statement
Given an integer `N`, find all possible arrangements of `N` queens on the chessboard that satisfy the above conditions.

Example for `N = 4`:

Here, `Q` denotes a queen and `.` denotes an empty square.

---

## 🛠 Approaches Implemented

### 1️⃣ Without Backtracking (Brute Force)
- **Idea:**  
  - Generate **all possible permutations** of queen placements (one per row).
  - Check if the arrangement is valid by verifying column and diagonal constraints.
- **Steps:**  
  1. Place queens row by row in every possible column.
  2. For each complete board, check if it’s safe (no attacks).
- **Advantages:**  
  - Simple to understand and implement.
- **Limitations:**  
  - **Extremely slow** — time complexity is `O(N!)` just to generate permutations, plus additional checks.
  - Not practical for larger `N` (works only for small boards like N ≤ 8).

---

### 2️⃣ With Backtracking
- **Idea:**  
  - Place queens **one by one** in different rows.
  - At each step, check if the current placement is safe **before** moving to the next row.
  - If no valid position is found in a row, **backtrack** to the previous row and try a new position.
- **Steps:**  
  1. Start from the first row, try placing a queen in each column.
  2. If placing the queen is safe, move to the next row.
  3. If no safe column exists, backtrack to the previous row.
  4. Continue until all queens are placed or all possibilities are exhausted.
- **Advantages:**  
  - **Much faster** than brute force because it prunes invalid branches early.
  - Handles larger boards efficiently.
- **Limitations:**  
  - Still exponential in the worst case, but significantly reduced search space.

---

## ⚖️ Comparison

| Method            | Time Complexity | Works For Large N? | Notes |
|-------------------|----------------|--------------------|-------|
| Brute Force       | O(N! × N)      | ❌                 | Very slow, only for small N |
| Backtracking      | O(N!) worst-case but faster in practice | ✅ | Efficient pruning |

---

## 📝 Conclusion
- **Brute Force** is educational for understanding the problem but not efficient.
- **Backtracking** is the standard approach for solving N-Queens efficiently.
