#include "Field.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Added for internal boundary checking
int m_w = 0, m_h = 0;

Field::Field(int** data, int x, int y, int vw, int vh) 
    : map_data(data), current_position_x(x), current_position_y(y), vision_width(vw), vision_height(vh) {
    map_name = "Manual Map";
}

Field::Field(const char* filename, int x, int y, int vw, int vh)
    : current_position_x(x), current_position_y(y), vision_width(vw), vision_height(vh) {
    loadFromMazeFile(filename);
}

void Field::loadFromMazeFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string sizeLine;
    getline(file, sizeLine);
    for(size_t i=0; i<sizeLine.length(); i++) if(sizeLine[i] == ',') sizeLine[i] = ' ';
    
    stringstream ss(sizeLine);
    ss >> m_w >> m_h;

    map_data = new int*[m_h];
    for (int i = 0; i < m_h; i++) {
        map_data[i] = new int[m_w];
        for (int j = 0; j < m_w; j++) {
            file >> map_data[i][j];
            if (file.peek() == ',') {
                char comma;
                file >> comma;
            }
        }
    }
    map_name = filename;
    file.close();
}

Field::~Field() {
    if (map_data) {
        for (int i = 0; i < m_h; i++) delete[] map_data[i];
        delete[] map_data;
    }
}

bool Field::move(char dir) {
    switch (dir) {
        case 'w': case 'W': return moveUp();
        case 's': case 'S': return moveDown();
        case 'a': case 'A': return moveLeft();
        case 'd': case 'D': return moveRight();
        default: return false;
    }
}

bool Field::moveUp() {
    if (current_position_y > 0 && map_data[current_position_y - 1][current_position_x] != 1) {
        current_position_y--;
        return true;
    }
    return false;
}

bool Field::moveDown() {
    if (current_position_y < m_h - 1 && map_data[current_position_y + 1][current_position_x] != 1) {
        current_position_y++;
        return true;
    }
    return false;
}

bool Field::moveLeft() {
    if (current_position_x > 0 && map_data[current_position_y][current_position_x - 1] != 1) {
        current_position_x--;
        return true;
    }
    return false;
}

bool Field::moveRight() {
    if (current_position_x < m_w - 1 && map_data[current_position_y][current_position_x + 1] != 1) {
        current_position_x++;
        return true;
    }
    return false;
}

int Field::getCurrentPositionX() const { return current_position_x; }
int Field::getCurrentPositionY() const { return current_position_y; }
int Field::getVisionWidth() const { return vision_width; }
int Field::getVisionHeight() const { return vision_height; }
string Field::getMapName() const { return map_name; }

int Field::getMapSymbol(int x, int y) {
    if (x >= 0 && x < m_w && y >= 0 && y < m_h) return map_data[y][x];
    return 1;
}

void Field::setPosition(int x, int y) {
    current_position_x = x;
    current_position_y = y;
}

void Field::setMapSymbol(int symbol, int x, int y) {
    if (x >= 0 && x < m_w && y >= 0 && y < m_h) map_data[y][x] = symbol;
}

void Field::setVisionSize(int w, int h) {
    vision_width = w;
    vision_height = h;
}

void Field::display() const {
    cout << "\nMap: " << map_name << " (" << current_position_x << ", " << current_position_y << ")" << endl;
    
    int start_x = current_position_x - vision_width / 2;
    int start_y = current_position_y - vision_height / 2;

    for (int i = 0; i < vision_height; i++) {
        for (int j = 0; j < vision_width; j++) {
            int target_y = start_y + i;
            int target_x = start_x + j;

            if (target_x == current_position_x && target_y == current_position_y) {
                cout << "P ";
            } else if (target_x < 0 || target_x >= m_w || target_y < 0 || target_y >= m_h) {
                cout << "  ";
            } else {
                int symbol = map_data[target_y][target_x];
                if (symbol == 1) cout << "# ";
                else if (symbol == 3) cout << "M ";
                else cout << ". ";
            }
        }
        cout << endl;
    }
}
