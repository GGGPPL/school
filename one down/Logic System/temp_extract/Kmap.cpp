#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>

using namespace std;

struct KmapSolution {
    int numVar;
    char **kmap;
    vector<string> primes;
    vector<string> essentials;
    vector<string> minimumSop;
};

// Helper function to convert minterm index to Gray code coordinates for K-map
void getCoords(int numVar, int index, int &r, int &c) {
    static const int gray[] = {0, 1, 3, 2};
    if (numVar == 2) {
        // PDF Page 2: A on top (Col), B on side (Row)
        int a = (index >> 1) & 1;
        int b = index & 1;
        r = b; c = a;
    } else if (numVar == 3) {
        // PDF Page 2: AB on top (Col), C on side (Row)
        int ab = (index >> 1) & 3;
        int c_val = index & 1;
        r = c_val;
        c = gray[ab];
    } else if (numVar == 4) {
        // 依照審核者習慣：橫軸為 CD (Col)，縱軸為 AB (Row)
        int ab = (index >> 2) & 3;
        int cd = index & 3;
        r = gray[ab];
        c = gray[cd];
    }
}

// Function to convert a grouping (implicant) to string representation (e.g., "a'bc")
string implicantToString(int numVar, int mask, int values) {
    string res = "";
    for (int i = 0; i < numVar; ++i) {
        int bit = numVar - 1 - i;
        if (!((mask >> bit) & 1)) {
            char var = 'a' + i;
            res += var;
            if (!((values >> bit) & 1)) res += "'";
        }
    }
    return res == "" ? "1" : res;
}

KmapSolution *solveKmap(int numVar, const vector<int> minterms, const vector<int> dontcares) {
    KmapSolution *sol = new KmapSolution;
    sol->numVar = numVar;

    int rows = (numVar == 4) ? 4 : 2;
    int cols = (numVar >= 3) ? 4 : 2;

    sol->kmap = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        sol->kmap[i] = new char[cols];
        for (int j = 0; j < cols; ++j) sol->kmap[i][j] = '0';
    }

    vector<int> allTrue(1 << numVar, 0); // 0: 0, 1: 1, 2: x
    for (int m : minterms) {
        allTrue[m] = 1;
        int r, c;
        getCoords(numVar, m, r, c);
        sol->kmap[r][c] = '1';
    }
    for (int d : dontcares) {
        allTrue[d] = 2;
        int r, c;
        getCoords(numVar, d, r, c);
        sol->kmap[r][c] = 'x';
    }

    struct Cube { int mask, val; };
    vector<Cube> allValid;
    for (int mask = 0; mask < (1 << numVar); ++mask) {
        int fixed = ((1 << numVar) - 1) ^ mask;
        for (int v = 0; v < (1 << numVar); ++v) {
            if (v & mask) continue;
            bool ok = true;
            bool hasOne = false;
            for (int i = 0; i < (1 << numVar); ++i) {
                if ((i & fixed) == v) {
                    if (allTrue[i] == 0) { ok = false; break; }
                    if (allTrue[i] == 1) hasOne = true;
                }
            }
            if (ok && hasOne) allValid.push_back({mask, v});
        }
    }

    vector<Cube> primes;
    for (auto &c1 : allValid) {
        bool isP = true;
        for (auto &c2 : allValid) {
            if (c1.mask == c2.mask && c1.val == c2.val) continue;
            if ((c2.mask & c1.mask) == c1.mask && (c1.val & ~c2.mask) == (c2.val & ~c2.mask)) {
                isP = false; break;
            }
        }
        if (isP) primes.push_back(c1);
    }

    for (auto &p : primes) {
        sol->primes.push_back(implicantToString(numVar, p.mask, p.val));
    }
    sort(sol->primes.begin(), sol->primes.end());
    sol->primes.erase(unique(sol->primes.begin(), sol->primes.end()), sol->primes.end());

    for (auto &p : primes) {
        bool essential = false;
        int fixed = ((1 << numVar) - 1) ^ p.mask;
        for (int i = 0; i < (1 << numVar); ++i) {
            if (allTrue[i] == 1 && (i & fixed) == p.val) {
                int count = 0;
                for (auto &p2 : primes) {
                    if ((i & (((1 << numVar) - 1) ^ p2.mask)) == p2.val) count++;
                }
                if (count == 1) { essential = true; break; }
            }
        }
        if (essential) {
            sol->essentials.push_back(implicantToString(numVar, p.mask, p.val));
        }
    }
    sort(sol->essentials.begin(), sol->essentials.end());
    sol->essentials.erase(unique(sol->essentials.begin(), sol->essentials.end()), sol->essentials.end());

    sol->minimumSop = sol->essentials;
    set<int> covered;
    for (string &s : sol->minimumSop) {
        for (auto &p : primes) {
            if (implicantToString(numVar, p.mask, p.val) == s) {
                int fixed = ((1 << numVar) - 1) ^ p.mask;
                for (int i = 0; i < (1 << numVar); ++i) {
                    if (allTrue[i] == 1 && (i & fixed) == p.val) covered.insert(i);
                }
            }
        }
    }

    vector<int> remaining;
    for (int m : minterms) if (covered.find(m) == covered.end()) remaining.push_back(m);

    while (!remaining.empty()) {
        int maxCover = -1;
        Cube best;
        for (auto &p : primes) {
            int count = 0;
            int fixed = ((1 << numVar) - 1) ^ p.mask;
            for (int m : remaining) {
                if ((m & fixed) == p.val) count++;
            }
            if (count > maxCover) {
                maxCover = count;
                best = p;
            }
        }

        if (maxCover > 0) {
            sol->minimumSop.push_back(implicantToString(numVar, best.mask, best.val));
            int fixed = ((1 << numVar) - 1) ^ best.mask;
            vector<int> next;
            for (int m : remaining) {
                if ((m & fixed) != best.val) next.push_back(m);
            }
            remaining = next;
        } else break;
    }
    sort(sol->minimumSop.begin(), sol->minimumSop.end());
    sol->minimumSop.erase(unique(sol->minimumSop.begin(), sol->minimumSop.end()), sol->minimumSop.end());

    return sol;
}
