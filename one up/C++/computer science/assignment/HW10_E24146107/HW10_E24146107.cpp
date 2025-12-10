#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>

using namespace std;

// 用來儲存找到的所有 LCS (使用 set 自動排序且不重複)
set<string> distinct_lcs;

// 建立 DP Table 的函式
vector<vector<int>> buildDPTable(const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();
    // 初始化 (m+1) x (n+1) 的二維陣列，全為 0
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp;
}

// 使用回溯法 (Backtracking) 從 DP Table 找出所有 LCS
void findAllLCS(int i, int j, const string& s1, const string& s2, const vector<vector<int>>& dp, string current_lcs) {
    // Base Case: 走到表格邊界，結束遞迴
    if (i == 0 || j == 0) {
        // 因為是從後往前找，所以要反轉字串
        reverse(current_lcs.begin(), current_lcs.end());
        distinct_lcs.insert(current_lcs);
        return;
    }

    // 情況 1: 字元相同，這個字元屬於 LCS
    if (s1[i - 1] == s2[j - 1]) {
        findAllLCS(i - 1, j - 1, s1, s2, dp, current_lcs + s1[i - 1]);
    } 
    // 情況 2: 字元不同，往 DP 值大的方向走
    else {
        // 如果上方的值跟當前一樣，代表可以往上走
        if (dp[i - 1][j] == dp[i][j]) {
            findAllLCS(i - 1, j, s1, s2, dp, current_lcs);
        }
        // 如果左方的值跟當前一樣，代表可以往左走
        if (dp[i][j - 1] == dp[i][j]) {
            findAllLCS(i, j - 1, s1, s2, dp, current_lcs);
        }
    }
}

// 去除字串前後空白的工具函式 (避免輸入檔有額外空白)
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int main() {
    string filePath;
    cout << "請輸入txt檔案的路徑 (ex: C:\\Users\\name\\Desktop\\lcs.txt): ";
    getline(cin, filePath); // 讀取整行路徑

    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "無法開啟檔案，請確認路徑是否正確。" << endl;
        return 1;
    }

    string line;
    // 逐行讀取檔案
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string segment;
        vector<string> pairStr;

        // 以逗號分割字串
        while (getline(ss, segment, ',')) {
            pairStr.push_back(trim(segment));
        }

        if (pairStr.size() < 2) continue; // 確保有兩個字串

        string s1 = pairStr[0];
        string s2 = pairStr[1];

        // 1. 建立 DP 表格
        vector<vector<int>> dp = buildDPTable(s1, s2);
        int lcs_len = dp[s1.length()][s2.length()];

        // 2. 清空上次的結果並開始回溯找字串
        distinct_lcs.clear();
        findAllLCS(s1.length(), s2.length(), s1, s2, dp, "");

        // 3. 輸出結果
        // 格式: 長度, lcs1, lcs2...
        cout << lcs_len;
        for (const string& s : distinct_lcs) {
            cout << ", " << s;
        }
        cout << endl;
    }

    inputFile.close();
    
    // 暫停一下讓視窗不會馬上關閉 (在 Windows 環境常用)
    cout << "\n執行完畢，按 Enter 鍵離開...";
    cin.get();

    return 0;
}