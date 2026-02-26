#ifndef E24146107_H  
#define E24146107_H  

#include <vector>
#include <iostream>

using namespace std;

// 檢查該欄位是否合法 (未滿)
bool isColValid(const vector<vector<char> >& board, int col) {
    if (col < 0 || col > 6) return false;
    return board[0][col] == '\0'; // Source.cpp 使用 '\0' 代表空
}

// 檢查連線狀態 (輔助函式)
bool checkConnect(const vector<vector<char> >& board, int r, int c, char disc) {
    // 方向：水平、垂直、右斜、左斜
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};

    for (int d = 0; d < 4; d++) {
        int count = 1;
        // 正向檢查
        for (int k = 1; k < 4; k++) {
            int nr = r + dr[d] * k;
            int nc = c + dc[d] * k;
            if (nr < 0 || nr >= 6 || nc < 0 || nc >= 7 || board[nr][nc] != disc) break;
            count++;
        }
        // 反向檢查
        for (int k = 1; k < 4; k++) {
            int nr = r - dr[d] * k;
            int nc = c - dc[d] * k;
            if (nr < 0 || nr >= 6 || nc < 0 || nc >= 7 || board[nr][nc] != disc) break;
            count++;
        }
        if (count >= 4) return true;
    }
    return false;
}

// 尋找是否有致勝或需要防守的一步
int findCriticalMove(const vector<vector<char> >& board, char disc) {
    for (int c = 0; c < 7; c++) {
        if (!isColValid(board, c)) continue;
        
        // 模擬落子位置
        int r = 5;
        while (r >= 0 && board[r][c] != '\0') r--;
        
        // 檢查如果下這裡是否會連成 4 線
        if (checkConnect(board, r, c, disc)) return c;
    }
    return -1;
}


// 參數 map: 目前棋盤, me: 我的棋子符號, opp: 對手棋子符號
int E24146107(const vector<vector<char> >& map, char me, char opp) {
    
    // 1. 進攻：檢查我是否能贏
    int winMove = findCriticalMove(map, me);
    if (winMove != -1) return winMove;

    // 2. 防守：檢查對手是否快贏了
    int blockMove = findCriticalMove(map, opp);
    if (blockMove != -1) return blockMove;

    // 3. 策略：優先佔領中間 (3 -> 2,4 -> 1,5 -> 0,6)
    int priorityCols[] = {3, 2, 4, 1, 5, 0, 6};
    for (int i = 0; i < 7; i++) {
        int col = priorityCols[i];
        if (isColValid(map, col)) {
            return col;
        }
    }
    
    // 4. Fallback (理論上不會執行到這裡)
    return 0;
}

#endif