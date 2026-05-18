#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// 迷宮節點座標結構
struct Point {
    int x, y;
};

// 迷宮類別封裝
class MazeSolver {
private:
    int col, row;
    vector<vector<int>> maze;
    vector<vector<bool>> visited;
    vector<Point> path;
    Point startPt, endPt;

    // 深度優先搜尋 (DFS) 尋找從起點到終點的路徑
    bool dfs(int x, int y) {
        // 1. 邊界與防呆檢查
        if (x < 0 || x >= col || y < 0 || y >= row) return false;
        if (maze[y][x] == 1) return false; // 遇到牆壁 (1)
        if (visited[y][x]) return false;   // 避免無窮迴圈，走過不重複走

        // 2. 標記為已走過並加入當前路徑
        visited[y][x] = true;
        path.push_back({x, y});

        // 3. 抵達終點 (201)
        if (x == endPt.x && y == endPt.y) return true;

        // 4. 探索四個方向：右、下、左、上
        if (dfs(x + 1, y)) return true;
        if (dfs(x, y + 1)) return true;
        if (dfs(x - 1, y)) return true;
        if (dfs(x, y - 1)) return true;

        // 5. 若此路不通，則回溯 (Backtracking)
        path.pop_back();
        return false;
    }

public:
    void solve() {
        string filename;
        // 規定：讓使用者輸入檔名，且不輸出干擾腳本的提示字元
        if (!(cin >> filename)) return; 

        // 規定：使用相對路徑讀取檔案
        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "Error opening file.\n";
            return;
        }

        char comma;
        // 讀取第一行的寬度 (col) 與高度 (row)，格式為 "col,row"
        if (!(fin >> col >> comma >> row)) return;

        // 初始化陣列大小
        maze.assign(row, vector<int>(col));
        visited.assign(row, vector<bool>(col, false));

        // 讀取迷宮結構陣列
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                fin >> maze[i][j];
                
                // 記錄起點與終點座標
                if (maze[i][j] == 200) {
                    startPt = {j, i};
                } else if (maze[i][j] == 201) {
                    endPt = {j, i};
                }
                
                // 忽略數值之間的逗號 (最後一欄之後沒有逗號)
                if (j != col - 1) fin >> comma;
            }
        }
        fin.close();

        // 執行路徑搜尋
        bool found = dfs(startPt.x, startPt.y);

        // 輸出最終迷宮與路徑結果
        printMaze(found);
    }

    void printMaze(bool found) {
        vector<vector<char>> display(row, vector<char>(col, ' '));
        
        // 建立初始迷宮視覺化字元
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (maze[i][j] == 1) display[i][j] = '#';         // 牆壁
                else if (maze[i][j] == 200) display[i][j] = 'S';  // 起點 (Start)
                else if (maze[i][j] == 201) display[i][j] = 'E';  // 終點 (End)
                else if (maze[i][j] >= 202) display[i][j] = '*';  // 額外獎勵點 (Bonus)
            }
        }

        // 規定：以 "X" 標記找出的路徑 (保留起點 S 與終點 E 顯示)
        if (found) {
            for (size_t k = 1; k < path.size() - 1; ++k) {
                display[path[k].y][path[k].x] = 'X';
            }
        }

        // 印出結果 (排版加上空格讓視覺更像正方形網格)
        cout << "\n";
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                cout << display[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

int main() {
    MazeSolver solver;
    solver.solve();
    return 0;
}