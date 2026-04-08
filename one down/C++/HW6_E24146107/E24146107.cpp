#include "matrix.h"
#include <iostream>

using namespace std;

// 初始化靜態變數 count
int Matrix::count = 0;

// 設定基礎大小並動態配置記憶體
void Matrix::setUp(int r, int c) {
    row = r;
    col = c;
    data = new double*[row];
    for (int i = 0; i < row; ++i) {
        data[i] = new double[col];
    }
}

// default constructor, 建立 2x2 全為 0 的矩陣
Matrix::Matrix() {
    setUp(2, 2);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = 0.0;
        }
    }
    count++; // 物件數量加一
}

// constructor, 依照傳入的大小建立全為 0 的矩陣
Matrix::Matrix(int r, int c) {
    setUp(r, c);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = 0.0;
        }
    }
    count++;
}

// constructor, 將一維陣列轉為二維陣列
Matrix::Matrix(int r, int c, double arr[], int len) {
    setUp(r, c);
    int k = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (k < len) {
                data[i][j] = arr[k++];
            } else {
                data[i][j] = 0.0; // 若超出陣列長度，補 0
            }
        }
    }
    count++;
}

// copy constructor (深拷貝，避免指標共享同一塊記憶體)
Matrix::Matrix(const Matrix &other) {
    setUp(other.row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
    count++;
}

// destructor, 釋放動態配置的二維陣列記憶體
Matrix::~Matrix() {
    for (int i = 0; i < row; ++i) {
        delete[] data[i];
    }
    delete[] data;
    count--; // 物件數量減一
}

// 取得目前的物件總數
int Matrix::getCount() {
    return count;
}

// 設定特定位置的值
void Matrix::setData(int r, int c, double val) {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        data[r][c] = val;
    }
}

// 取得 row 的數量
int Matrix::getRow() const {
    return row;
}

// 取得 col 的數量
int Matrix::getCol() const {
    return col;
}

// 取得特定位置的值
double Matrix::getData(int r, int c) const {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        return data[r][c];
    }
    return 0.0;
}

// ====== HW6 Operator Overloading 實作 ======

// 1. << : Matrix 輸出 (排版需支援連續輸出 cout << m1 << m2)
ostream& operator<<(ostream& os, const Matrix& m) {
    os << "\n";
    for (int i = 0; i < m.row; i++) {
        for (int j = 0; j < m.col; j++) {
            os << m.data[i][j] << "\t";
        }
        os << "\n";
    }
    return os;
}

// 2. >> : Matrix 輸入 (支援連續輸入 cin >> m1 >> m2)
istream& operator>>(istream& is, Matrix& m) {
    for (int i = 0; i < m.row; i++) {
        for (int j = 0; j < m.col; j++) {
            is >> m.data[i][j];
        }
    }
    return is;
}

// 3. = : Assignment operator (指派運算子，必須處理自我指派並進行深拷貝)
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this; // 防止自我指派 (如 m1 = m1) 造成資料遺失
    }
    
    // 清除舊有的記憶體
    for (int i = 0; i < row; i++) {
        delete[] data[i];
    }
    delete[] data;
    
    // 重新配置符合 other 大小的記憶體並複製資料
    setUp(other.row, other.col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

// 4. + : Matrix + Matrix (矩陣相加)
Matrix Matrix::operator+(const Matrix& other) const {
    if (row != other.row || col != other.col) {
        cout << "Error: Matrix dimensions must agree for addition." << endl;
        return Matrix(row, col); // 維度不符回傳全為 0 的矩陣
    }
    Matrix result(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

// 5. + : Matrix + double (矩陣內所有數值加上該 double)
Matrix Matrix::operator+(double val) const {
    Matrix result(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[i][j] = data[i][j] + val;
        }
    }
    return result;
}

// 6. += : Matrix += Matrix
Matrix& Matrix::operator+=(const Matrix& other) {
    if (row == other.row && col == other.col) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[i][j] += other.data[i][j];
            }
        }
    } else {
        cout << "Error: Matrix dimensions must agree for +=" << endl;
    }
    return *this;
}

// 7. ++ : 後置遞增 (m5++)，所有數值加 1.0
Matrix Matrix::operator++(int) {
    Matrix temp(*this); // 拷貝遞增前的狀態，準備回傳
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] += 1.0;
        }
    }
    return temp; // 回傳遞增前的矩陣
}

// 8. * : Matrix * Matrix (矩陣相乘)
Matrix Matrix::operator*(const Matrix& other) const {
    if (col != other.row) {
        cout << "Error: Matrix dimensions must agree for multiplication." << endl;
        return Matrix(row, other.col); // 維度不合，回傳全零矩陣
    }
    Matrix result(row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < other.col; ++j) {
            result.data[i][j] = 0.0;
            for (int k = 0; k < col; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

// 9. () : 轉置矩陣 (Transpose)，回傳自身的轉置
Matrix Matrix::operator()() const {
    Matrix result(col, row); // 行列長度互換
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}

// 10. == : 測試兩個矩陣是否完全相同
bool Matrix::operator==(const Matrix& other) const {
    if (row != other.row || col != other.col) {
        return false;
    }
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}