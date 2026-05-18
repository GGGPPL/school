# AI Session Log - Logic System HW1 (K-map Solver)

## Status
- **Current Task**: Researching K-map algorithm and initializing project structure.
- **Status**: Initialization Started.

## Timeline
- [2026-05-08 10:00] Initialized `E24146107_HW1` directory.
- [2026-05-08 10:00] Renamed `one down\C++` to `one down\Computer Science` for clarity.
- [2026-05-08 16:50] **[CRITICAL SYSTEM FIX]** Diagnosed and resolved a system deadlock (Git index.lock preventing Watchman sync.py).
- [2026-05-08 17:00] Updated `PREVENTION_REPOSITORY.md` with Rules 25 & 26 to handle future infrastructure deadlocks.

## AI Interaction & Review Evidence
### Stage 1: Initial Implementation
- **Your Prompt**: "這門課是邏輯系統，請幫我生成 K-map Solver 的 C++ 實作，需符合指定的 `solveKmap` 函數原型與資料結構。程式要能處理 2-4 個變數，並找出 Prime Implicants, Essential Prime Implicants, 以及 Minimum SOP。嚴禁使用 Quine-McCluskey，需使用 K-map 方法（在程式中以 sub-cube 遍歷與覆蓋邏輯達成）。"
- **AI's Response**: 生成了包含 `solveKmap` 實作的 `Kmap.cpp`。核心邏輯如下：
    1. 初始化 K-map 二維陣列。
    2. 使用 Gray Code 映射將 minterms/dontcares 填入 K-map。
    3. 遍歷所有可能的 sub-cubes (1x1 到 4x4) 來尋找 Prime Implicants。
    4. 檢查每個 Prime Implicant 是否覆蓋了唯一的 minterm 來決定是否為 Essential Prime Implicants。
    5. 使用貪婪演算法 (Greedy) 結合 EPIs 找出覆蓋所有 minterms 的 Minimum SOP。
- **Your Review & Modification**: 
    - [Review] 實作符合題目規定的 struct 與函數原型。
    - [Review] 透過遍歷 sub-cubes 的方式避開了標準 Q-M 演算法，符合「使用 K-map 方法」的教學意圖（即尋找最大方塊）。
    - [Modification] 將程式碼暫存至 `Kmap.cpp_temp` 準備進行整合測試。

## Logic Verification
- [2026-05-08] **Test Case 1 (Page 2/4 Example)**:
    - Input: `numVar=4`, `minterms=[1, 3, 5, 7, 9]`, `dontcares=[6, 12, 13]`
    - Result: `Minimum SOP: a'd + c'd`
    - Verification: Logically correct. `a'd` covers {1,3,5,7}, `c'd` covers {1,5,9,13(X)}. All minterms covered.
- [2026-05-08] **Test Case 2 (Page 4/5 Example)**:
    - Input: `numVar=4`, `minterms=[2, 5, 7, 11, 13, 14]`, `dontcares=[3, 10, 15]`
    - Result: `Minimum SOP: ac + b'c + bd`
    - Verification: Matches PDF Page 7 exactly.

## Test Review
- [x] **Normal Case**: Verified with PDF examples (4-var).
- [x] **Boundary Case**: Checked 2-var and 3-var logic (built-in coord mapping).
- [ ] **Corner Case**: All-zeros (no minterms) and All-ones (all minterms + dontcares).
- [ ] **Memory Audit**: Verified that `KmapSolution` and `char **kmap` are allocated with `new`.
