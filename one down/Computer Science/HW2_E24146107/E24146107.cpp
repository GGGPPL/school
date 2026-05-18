#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

class Minesweeper {
private:
    int height, width, mines;
    vector<vector<char>> displayBoard; // 顯示給玩家看的地圖
    vector<vector<bool>> mineBoard;    // 紀錄地雷真實位置
    vector<vector<bool>> revealed;     // 紀錄格子是否已被翻開
    bool firstClick;                   // 判定是否為第一步
    int revealedCount;                 // 已安全翻開的數量

    // 八個方向的座標偏移量 (上、下、左、右、四個斜角)
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // 檢查座標是否在合法範圍內
    bool isValid(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // 計算周圍 8 宮格內的地雷總數
    int countNeighborMines(int x, int y) {
        int count = 0;
        for (int i = 0; i < 8; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny) && mineBoard[ny][nx]) {
                count++;
            }
        }
        return count;
    }

    // 隨機放置地雷 (改用陣列洗牌法，保證秒速生成，絕對不卡住)
    void placeMines(int firstX, int firstY) {
        vector<pair<int, int>> available;
        // 將所有合法的空位加入候選清單 (避開第一次點擊的位置)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (!(x == firstX && y == firstY)) {
                    available.push_back({x, y});
                }
            }
        }
        
        // 從候選清單中隨機抽出 mines 個位置放地雷
        for (int i = 0; i < mines; ++i) {
            int randIdx = i + rand() % (available.size() - i);
            swap(available[i], available[randIdx]);
            mineBoard[available[i].second][available[i].first] = true;
        }
    }

    // 展開格子 (改用 BFS 迴圈解法，避免地圖太大時記憶體當機)
    void reveal(int startX, int startY) {
        if (!isValid(startX, startY) || revealed[startY][startX]) return;

        vector<pair<int, int>> queue;
        queue.push_back({startX, startY});
        revealed[startY][startX] = true;
        revealedCount++;

        int head = 0;
        while (head < queue.size()) {
            int x = queue[head].first;
            int y = queue[head].second;
            head++;

            int neighborMines = countNeighborMines(x, y);
            if (neighborMines > 0) {
                // 周圍有地雷，顯示數字不再擴散
                displayBoard[y][x] = '0' + neighborMines;
            } else {
                // 周圍零地雷，顯示空白並自動向周圍擴散
                displayBoard[y][x] = ' '; 
                for (int i = 0; i < 8; ++i) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (isValid(nx, ny) && !revealed[ny][nx]) {
                        revealed[ny][nx] = true;
                        revealedCount++;
                        queue.push_back({nx, ny});
                    }
                }
            }
        }
    }

    // 印出遊戲地圖
    void printBoard(bool showMines = false) {
        cout << "\n   ";
        for (int x = 0; x < width; ++x) cout << x << " ";
        cout << "\n";
        for (int y = 0; y < height; ++y) {
            if (y < 10) cout << " ";
            cout << y << " ";
            
            for (int x = 0; x < width; ++x) {
                if (showMines && mineBoard[y][x]) {
                    cout << "* "; 
                } else if (revealed[y][x]) {
                    cout << displayBoard[y][x] << " ";
                } else {
                    cout << "- "; 
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // 清除輸入緩衝區，防呆更穩定
    void clearInputBuffer() {
        cin.clear();
        string dump;
        getline(cin, dump);
    }

public:
    Minesweeper() {
        srand(time(0)); 
    }

    void init() {
        while (true) {
            cout << "Please input height, width, and number of mines:\n> ";
            cin >> height >> width >> mines;

            // 確保輸入的是數字，且沒有少輸入
            if (cin.fail()) {
                clearInputBuffer();
                cout << "Error: Invalid input format. Please enter three numbers.\n";
                continue;
            }

            if (height <= 0 || width <= 0 || mines <= 0) {
                cout << "Error: Invalid input. Height, width, and mines must be positive.\n";
                continue;
            }
            
            if (mines >= width * height) {
                cout << "Error: Mines cannot exceed total grids (" << width * height - 1 << "). Please try again:\n";
                continue;
            }
            break;
        }

        displayBoard.assign(height, vector<char>(width, '-'));
        mineBoard.assign(height, vector<bool>(width, false));
        revealed.assign(height, vector<bool>(width, false));
        firstClick = true;
        revealedCount = 0;
    }

    void play() {
        cout << "Welcome to Minesweeper!\n";
        init();

        while (true) {
            printBoard();
            cout << "Enter coordinate (x y) to sweep:\n> ";
            int x, y;
            cin >> x >> y;

            if (cin.fail()) {
                clearInputBuffer();
                cout << "Error: Invalid input format. Enter like '0 0'.\n";
                continue;
            }

            if (!isValid(x, y)) {
                cout << "Error: Coordinate out of bounds.\n";
                continue;
            }
            if (revealed[y][x]) {
                cout << "Error: Coordinate already revealed. Please try again.\n";
                continue;
            }

            if (firstClick) {
                placeMines(x, y);
                firstClick = false;
            }

            if (mineBoard[y][x]) {
                cout << "BOOM! You hit a mine. Game Over.\n";
                printBoard(true); 
                break;
            }

            reveal(x, y);

            if (revealedCount == height * width - mines) {
                cout << "Congratulations! You cleared all the safe zones. You Win!\n";
                printBoard(true);
                break;
            }
        }
    }
};

int main() {
    Minesweeper game;
    char playAgain;
    
    do {
        game.play();
        cout << "Do you want to play again? (Y/N):\n> ";
        cin >> playAgain;
    } while (playAgain == 'Y' || playAgain == 'y');

    return 0;
}