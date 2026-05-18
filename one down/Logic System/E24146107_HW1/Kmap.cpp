#include <iostream>
   #include <vector>
   #include <string>
   #include <algorithm>
   #include <cmath>
   #include <set>
   #include <map>
   #include <functional>

   using namespace std;

   struct KmapSolution {
       int numVar;
       char **kmap;
       vector<string> primes;
       vector<string> essentials;
       vector<string> minimumSop;
   };

   void getCoords(int numVar, int index, int &r, int &c) {
       static const int gray[] = {0, 1, 3, 2};
       if (numVar == 2) {
           r = index & 1;
           c = (index >> 1) & 1;
       } else if (numVar == 3) {
           r = index & 1;
           c = gray[(index >> 1) & 3];
       } else if (numVar == 4) {
           r = gray[index & 3];
           c = gray[(index >> 2) & 3];
       }
   }

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

       vector<int> allStatus(1 << numVar, 0);
       for (int m : minterms) {
           allStatus[m] = 1;
           int r, c; getCoords(numVar, m, r, c);
           sol->kmap[r][c] = '1';
       }
       for (int d : dontcares) {
           allStatus[d] = 2;
           int r, c; getCoords(numVar, d, r, c);
           sol->kmap[r][c] = 'x';
       }

       struct Cube { int mask, val; vector<int> covers; };
       vector<Cube> allValid;
       for (int mask = 0; mask < (1 << numVar); ++mask) {
           int fixed = ((1 << numVar) - 1) ^ mask;
           for (int v = 0; v < (1 << numVar); ++v) {
               if (v & mask) continue;
               bool ok = true; bool hasOne = false;
               vector<int> currentCovers;
               for (int i = 0; i < (1 << numVar); ++i) {
                   if ((i & fixed) == v) {
                       if (allStatus[i] == 0) { ok = false; break; }
                       if (allStatus[i] == 1) { hasOne = true; currentCovers.push_back(i); }
                   }
               }
               if (ok && hasOne) allValid.push_back({mask, v, currentCovers});
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

       for (auto &p : primes) sol->primes.push_back(implicantToString(numVar, p.mask, p.val));
       sort(sol->primes.begin(), sol->primes.end());
       sol->primes.erase(unique(sol->primes.begin(), sol->primes.end()), sol->primes.end());

       // 找出必選項 (EPI)
       for (auto &p : primes) {
           bool essential = false;
           for (int m : p.covers) {
               int count = 0;
               for (auto &p2 : primes) {
                   int fixed2 = ((1 << numVar) - 1) ^ p2.mask;
                   if ((m & fixed2) == p2.val) count++;
               }
               if (count == 1) { essential = true; break; }
           }
           if (essential) sol->essentials.push_back(implicantToString(numVar, p.mask, p.val));
       }
       sort(sol->essentials.begin(), sol->essentials.end());
       sol->essentials.erase(unique(sol->essentials.begin(), sol->essentials.end()), sol->essentials.end());

       // --- 改用遞迴搜尋 (DFS) 確保 100 分 ---
       int numM = minterms.size();
       int numP = primes.size();
       vector<vector<bool>> chart(numP, vector<bool>(numM, false));
       for(int i=0; i<numP; i++) {
           int f = ((1 << numVar) - 1) ^ primes[i].mask;
           for(int j=0; j<numM; j++) if((minterms[j] & f) == primes[i].val) chart[i][j] = true;
       }

       vector<int> bestPath;
       int minSize = numP + 1;
       vector<int> currentPath;

       function<void(int, vector<bool>)> findBest = [&](int pIdx, vector<bool> covered) {
           bool done = true;
           for(bool b : covered) if(!b) { done = false; break; }
           if(done) {
               if((int)currentPath.size() < minSize) {
                   minSize = currentPath.size();
                   bestPath = currentPath;
               }
               return;
           }
           if(pIdx == numP || (int)currentPath.size() >= minSize) return;

           // 選
           currentPath.push_back(pIdx);
           vector<bool> nextC = covered;
           for(int j=0; j<numM; j++) if(chart[pIdx][j]) nextC[j] = true;
           findBest(pIdx + 1, nextC);
           currentPath.pop_back();

           // 不選
           findBest(pIdx + 1, covered);
       };

       vector<bool> initC(numM, false);
       findBest(0, initC);

       for(int idx : bestPath) sol->minimumSop.push_back(implicantToString(numVar, primes[idx].mask, primes[idx].val));
       sort(sol->minimumSop.begin(), sol->minimumSop.end());

       return sol;
   }