#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

using namespace std;

struct QmSolution {
    int numVar;
    vector<string> primes;
    vector<vector<string>> minimumSops;
};

struct Term {
    string bits;
    set<int> covered_minterms;
    bool used = false;

    int countOnes() const {
        int count = 0;
        for (char c : bits) if (c == '1') count++;
        return count;
    }
};

string toBinary(int n, int numVar) {
    string res = "";
    for (int i = numVar - 1; i >= 0; i--) {
        res += ((n >> i) & 1) ? '1' : '0';
    }
    return res;
}

// --- 新增：檢查兩項是否可以合併 ---
// 兩項必須只有一個位元不同，且 '-' 的位置必須完全一樣
bool canMerge(const Term& t1, const Term& t2, int& diffIdx) {
    int diffCount = 0;
    diffIdx = -1;
    for (int i = 0; i < t1.bits.length(); i++) {
        if (t1.bits[i] != t2.bits[i]) {
            if (t1.bits[i] == '-' || t2.bits[i] == '-') return false; // '-' 位置不同不能合併
            diffCount++;
            diffIdx = i;
        }
    }
    return diffCount == 1;
}

QmSolution* solveQm(int numVar, vector<int> minterms, vector<int> dontcares, bool verbose) {
    QmSolution* sol = new QmSolution;
    sol->numVar = numVar;

    map<int, vector<Term>> current_column;
    
    // 初始化 Column 1
    vector<int> all_terms = minterms;
    all_terms.insert(all_terms.end(), dontcares.begin(), dontcares.end());
    sort(all_terms.begin(), all_terms.end());
    all_terms.erase(unique(all_terms.begin(), all_terms.end()), all_terms.end());

    for (int m : all_terms) {
        Term t;
        t.bits = toBinary(m, numVar);
        t.covered_minterms.insert(m);
        current_column[t.countOnes()].push_back(t);
    }

    vector<Term> all_primes;
    int colIdx = 1;

    // --- 步驟 2：不斷合併直到不能再合併為止 ---
    while (!current_column.empty()) {
        if (verbose) {
            cout << "=====================" << endl;
            cout << "Column " << colIdx << endl;
            cout << "=====================" << endl;
        }

        map<int, vector<Term>> next_column;
        set<string> added_to_next; // 防止重複加入相同的合併項

        // 比較相鄰的群組
        for (auto it = current_column.begin(); it != current_column.end(); ++it) {
            int current_ones = it->first;
            if (current_column.count(current_ones + 1)) {
                for (auto& t1 : it->second) {
                    for (auto& t2 : current_column[current_ones + 1]) {
                        int diffIdx;
                        if (canMerge(t1, t2, diffIdx)) {
                            t1.used = t2.used = true;
                            Term combined = t1;
                            combined.bits[diffIdx] = '-';
                            combined.used = false;
                            combined.covered_minterms.insert(t2.covered_minterms.begin(), t2.covered_minterms.end());
                            
                            if (added_to_next.find(combined.bits) == added_to_next.end()) {
                                next_column[combined.countOnes()].push_back(combined);
                                added_to_next.insert(combined.bits);
                            }
                        }
                    }
                }
            }
        }

        // 把沒被合併過的項收集起來，它們就是 Prime Implicants
        for (auto const& [count, terms] : current_column) {
            for (const auto& t : terms) {
                if (verbose) {
                    cout << (t.used ? "v " : "  ") << t.bits << ": ";
                    for (int m : t.covered_minterms) cout << m << " ";
                    cout << endl;
                }
                if (!t.used) {
                    all_primes.push_back(t);
                }
            }
            if (verbose) cout << "---------------------" << endl;
        }

        current_column = next_column;
        colIdx++;
    }

    for (const auto& p : all_primes) {
        sol->primes.push_back(p.bits);
    }

    // --- 步驟 3：建立 PI Chart ---
    // 橫軸只有真正的 minterms (排除 dontcares)
    int numMinterms = minterms.size();
    int numPIs = all_primes.size();
    vector<vector<bool>> chart(numPIs, vector<bool>(numMinterms, false));

    for (int i = 0; i < numPIs; i++) {
        for (int j = 0; j < numMinterms; j++) {
            if (all_primes[i].covered_minterms.count(minterms[j])) {
                chart[i][j] = true;
            }
        }
    }

    if (verbose) {
        cout << "=====================" << endl;
        cout << "Prime Implicant Chart" << endl;
        cout << "=====================" << endl;
        for (int j = 0; j < numMinterms; j++) cout << "\t" << minterms[j];
        cout << endl;
        for (int i = 0; i < numPIs; i++) {
            cout << all_primes[i].bits;
            for (int j = 0; j < numMinterms; j++) {
                cout << "\t" << (chart[i][j] ? "X" : " ");
            }
            cout << endl;
        }
    }

    // --- 步驟 4：DFS 尋找最小覆蓋 ---
    vector<int> current_path;
    vector<vector<int>> all_min_paths;
    int min_size = numPIs + 1;

    // 定義遞迴搜尋函式
    auto dfs = [&](auto self, int p_idx, vector<bool> covered) -> void {
        bool all_covered = true;
        for (bool b : covered) if (!b) { all_covered = false; break; }

        if (all_covered) {
            if (current_path.size() < min_size) {
                min_size = current_path.size();
                all_min_paths.clear();
                all_min_paths.push_back(current_path);
            } else if (current_path.size() == min_size) {
                all_min_paths.push_back(current_path);
            }
            return;
        }

        if (p_idx == numPIs || current_path.size() >= min_size) return;

        // 選擇目前的 PI
        current_path.push_back(p_idx);
        vector<bool> next_covered = covered;
        for (int j = 0; j < numMinterms; j++) if (chart[p_idx][j]) next_covered[j] = true;
        self(self, p_idx + 1, next_covered);
        current_path.pop_back();

        // 不選目前的 PI
        self(self, p_idx + 1, covered);
    };

    vector<bool> initial_covered(numMinterms, false);
    dfs(dfs, 0, initial_covered);

    // 輔助函式：將 bit 字串轉為 a'b 格式
    auto bitsToExpr = [&](string bits) {
        string expr = "";
        for (int i = 0; i < numVar; i++) {
            if (bits[i] == '0') {
                expr += (char)('a' + i);
                expr += "'";
            } else if (bits[i] == '1') {
                expr += (char)('a' + i);
            }
        }
        return expr;
    };

    // 儲存找到的 Prime Implicants (轉換格式)
    sol->primes.clear();
    for (const auto& p : all_primes) {
        sol->primes.push_back(bitsToExpr(p.bits));
    }

    // 格式化輸出結果 (轉換格式)
    for (const auto& path : all_min_paths) {
        vector<string> sop;
        for (int pi_idx : path) {
            sop.push_back(bitsToExpr(all_primes[pi_idx].bits));
        }
        sol->minimumSops.push_back(sop);
    }

    return sol;
}

int main() {
    // Test Case: f(a,b,c,d) = Σm(4,8,10,11,12,15) + Σd(9,14)
    int numVar = 4;
    vector<int> minterms = {4, 8, 10, 11, 12, 15};
    vector<int> dontcares = {9, 14};

    cout << "Solving QM for: Sum m(4,8,10,11,12,15) + d(9,14)" << endl;
    QmSolution* sol = solveQm(numVar, minterms, dontcares, true);

    cout << "\nFinal Minimum SOPs:" << endl;
    for (const auto& sop : sol->minimumSops) {
        for (int i = 0; i < sop.size(); i++) {
            cout << sop[i] << (i == sop.size() - 1 ? "" : " + ");
        }
        cout << endl;
    }

    delete sol;
    return 0;
}
