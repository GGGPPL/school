/*
 * 作業名稱: Find all Longest Common Sequences (LCS)
 * 功能描述:
 * 1. 讀取使用者輸入的 txt 檔案路徑。
 * 2. 讀取檔案中每一行，以逗號分隔兩個字串。
 * 3. 使用動態規劃 (DP) 計算 LCS 長度。
 * 4. 使用回溯法 (Backtracking) 找出"所有" LCS 組合。
 * 5. 輸出格式: 長度, lcs1,lcs2... (依字母排序)
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

// 使用 map 進行 Memoization (備忘錄)，避免回溯時重複計算相同路徑
// Key: (i, j) 座標, Value: 該位置能產生的所有 LCS 後綴集合
map<pair<int, int>, set<string>> memo;

/**
 * 函式: get_lcs_strings
 * 目的: 透過回溯 DP 表格，找出所有的 LCS 字串
 * 參數:
 * i, j: 當前在 s1 和 s2 的索引位置 (從 1 開始)
 * s1, s2: 原始字串
 * dp: 已計算好的 DP 表格
 * 回傳: 一個包含所有 LCS 字串的 set (自動排序且不重複)
 */
set<string> get_lcs_strings(int i, int j, const string& s1, const string& s2, const vector<vector<int>>& dp) {
    // 邊界條件：如果索引歸零，代表字串空了，回傳包含空字串的集合
    if (i == 0 || j == 0) {
        return {""};
    }

    // 檢查備忘錄，如果算過就直接回傳
    if (memo.count({i, j})) {
        return memo[{i, j}];
    }

    set<string> result;

    // 情況 1: 字元相同，這是 LCS 的一部分
    if (s1[i - 1] == s2[j - 1]) {
        // 往左上角遞迴
        set<string> prev_lcs = get_lcs_strings(i - 1, j - 1, s1, s2, dp);
        // 將當前字元加到所有回傳的子序列後面
        for (const string& s : prev_lcs) {
            result.insert(s + s1[i - 1]);
        }
    } 
    // 情況 2: 字元不同，需判斷往哪個方向回溯 (上或左)
    else {
        // 如果上方格子的長度等於當前長度，代表可以往上走
        if (dp[i - 1][j] >= dp[i][j - 1]) {
            set<string> temp = get_lcs_strings(i - 1, j, s1, s2, dp);
            result.insert(temp.begin(), temp.end());
        }
        // 如果左方格子的長度等於當前長度，代表可以往左走
        if (dp[i][j - 1] >= dp[i - 1][j]) {
            set<string> temp = get_lcs_strings(i, j - 1, s1, s2, dp);
            result.insert(temp.begin(), temp.end());
        }
    }

    // 存入備忘錄並回傳
    return memo[{i, j}] = result;
}

/**
 * 函式: solve_lcs
 * 目的: 處理單一行的兩個字串，計算並印出結果
 */
void solve_lcs(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();

    // 建立 DP 表格，初始化為 0
    // dp[i][j] 代表 s1 前 i 個字元與 s2 前 j 個字元的 LCS 長度
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 填寫 DP 表格
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int lcs_len = dp[m][n];

    // 清空全域的 memo，準備進行新的回溯
    memo.clear();
    
    // 取得所有 LCS 組合
    set<string> all_lcs = get_lcs_strings(m, n, s1, s2, dp);

    // 輸出結果
    // 格式: 長度, lcs1,lcs2...
    cout << lcs_len << ", ";
    
    int count = 0;
    for (const string& s : all_lcs) {
        cout << s;
        count++;
        // 如果不是最後一個元素，印出逗號
        if (count < all_lcs.size()) {
            cout << ",";
        }
    }
    cout << endl;
}

int main() {
    // 題目要求: 詢問使用者 txt 檔案的輸入路徑
    string filepath;
    cout << "請輸入txt檔案的輸入路徑 (ex: C:\\Users\\yourname\\Desktop\\lcs.txt): ";
    // 使用 getline 讀取整行，避免路徑中有空白造成錯誤
    getline(cin, filepath);

    // 去除路徑可能包含的引號 (Windows 複製路徑時常有)
    if (!filepath.empty() && filepath.front() == '"') filepath.erase(0, 1);
    if (!filepath.empty() && filepath.back() == '"') filepath.pop_back();

    ifstream infile(filepath);

    if (!infile.is_open()) {
        cerr << "錯誤: 無法開啟檔案 '" << filepath << "'" << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        // 去除行尾可能的 \r (Windows 換行字元處理)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) continue;

        // 尋找逗號位置
        size_t comma_pos = line.find(',');
        if (comma_pos != string::npos) {
            string s1 = line.substr(0, comma_pos);
            string s2 = line.substr(comma_pos + 1);

            // 去除字串前後的空白
            // Lambda 函式: 去除頭尾空白
            auto trim = [](string &s) {
                if (s.empty()) return;
                s.erase(0, s.find_first_not_of(" \t"));
                s.erase(s.find_last_not_of(" \t") + 1);
            };

            trim(s1);
            trim(s2);

            solve_lcs(s1, s2);
        }
    }

    infile.close();
    
    // 暫停畫面以便查看結果 (Windows 適用)
    system("pause"); 
    
    return 0;
}