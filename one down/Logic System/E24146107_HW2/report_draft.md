# Logic System HW2 - Quine-McCluskey Method Report

## 1. AI Interaction & Review Evidence

### Stage 1: Algorithm Design (Recursive DFS vs Petrick's Method)
*   **Prompt**: "I need to implement QM method for 8-10 variables. Should I use Petrick's Method or Recursive DFS for finding minimum SOPs?"
*   **AI Response**: Recommended Recursive DFS (Branch and Bound) for efficiency in 8-10 variables to avoid exponential expansion issues of Petrick's Method.
*   **User Review**: Accepted Recursive DFS because it's better for online judge (CASOJ) performance limits.

### Stage 2: Step-by-Step Learning & Verification
*   **Prompt**: "Explain Step 1 (Grouping) and Step 2 (Merging) using analogies and quiz me."
*   **AI Response**: Provided "Course Enrollment" and "Shopping List" analogies.
*   **User Review**: Found analogies confusing and requested direct technical explanation. Shifted focus to binary string logic (`0100` + `0101` = `010-`).

---

## 2. Program Explanation & Flowchart

### Algorithm: Quine-McCluskey with Branch and Bound
1.  **Grouping**: Group all minterms and dont-cares by Hamming weight (number of 1s).
2.  **Iterative Merging**: Compare terms in adjacent groups. If they differ by exactly 1 bit and have '-' in the same positions, merge them and mark as 'used'.
3.  **PI Identification**: Any term not marked 'used' after all merging iterations is a Prime Implicant (PI).
4.  **PI Chart**: Create a mapping of PIs to required Minterms (excluding dont-cares).
5.  **Recursive DFS (Branch and Bound)**:
    *   Start with an empty selection and an empty coverage set.
    *   For each PI, decide: "Select" or "Skip".
    *   **Pruning**: If `current_selection.size() >= current_best_min_size`, abort the current branch.
    *   If all required minterms are covered, update the global minimum SOP list.

### Flowchart Description
`Start -> Group Column 1 -> While(can merge) { Create Next Column } -> Collect Unused as PIs -> Build PI Chart -> Start DFS(PI_0, covered_set) -> [Base Case: All covered? -> Save Result] -> [Branch: Include PI_i?] -> [Branch: Exclude PI_i?] -> End`

---

## 3. Test Scenarios & Evidence

### Test Case 1: Assignment Example
*   **Input**: `numVar=8`, `minterms={0, 2, 5, 6, 7, 8, 10, 12, 13, 14, 15}`, `dontcares={255}`
*   **Result**: Correctly identified 4 valid minimum SOPs each containing 4 PIs.
*   **Evidence**: 
    ```
    Column 3: 0000-0-0: 0 2 8 10, 0000--10: 2 6 10 14, etc.
    Final Results: { a'b'c'd'f'h' a'b'c'd'gh' a'b'c'd'eh' a'b'c'd'fh }, etc.
    ```

---

## 4. Advanced C++ Techniques Used
1.  **Lambda with `std::function` for Recursion**: Used `auto dfs = [&](auto self, ...)` to define a recursive search locally within `solveQm`.
2.  **`std::set` for Minterm Coverage**: Utilized `std::set::count` for fast checking if a PI covers a specific minterm.
3.  **`std::map` for Grouping**: Leveraged `std::map<int, vector<Term>>` to automatically sort and group terms by their Hamming weight.
4.  **`std::unique` & `std::erase` Idiom**: Used for efficient deduplication of the initial `all_terms` vector.
5.  **Pass-by-Reference & `const` Correctness**: Optimized performance by passing large objects (like `Term`) via `const Term&` to avoid unnecessary copying.
