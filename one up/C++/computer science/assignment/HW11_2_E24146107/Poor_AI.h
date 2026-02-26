#ifndef POOR_AI_H
#define POOR_AI_H

#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// 簡單的隨機 AI，符合 Source.cpp 的函式介面
int poorAI(const vector<vector<char> >& map, char me, char opp) {
    int attempts = 0;
    // 嘗試隨機下子
    while (attempts < 50) {
        int col = rand() % 7;
        if (map[0][col] == '\0') {
            return col;
        }
        attempts++;
    }
    // 若隨機失敗，循序找空位
    for (int i = 0; i < 7; i++) {
        if (map[0][i] == '\0') return i;
    }
    return 0;
}

#endif