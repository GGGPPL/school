// 【備註：這是「必寫題 (Requirement)」版本 - 處理 double】
#include "E24146107_matrix.h"

// 設定基礎大小並動態配置記憶體
void Matrix::setUp(int r, int c) {
    row = r;
    col = c;
    data = new double*[row];
    for (int i = 0; i < row; ++i) {
        data[i] = new double[col];
    }
}

// default constructor, 2x2 皆為 0
Matrix::Matrix() {
    setUp(2, 2);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = 0.0;
        }
    }
}

// constructor, 設定大小, 預設為0
Matrix::Matrix(int r, int c) {
    setUp(r, c);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = 0.0;
        }
    }
}

// constructor, 一維陣列轉二維
Matrix::Matrix(int r, int c, double arr[], int len) {
    setUp(r, c);
    int k = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (k < len) {
                data[i][j] = arr[k++];
            } else {
                data[i][j] = 0.0; // 超出陣列長度補 0
            }
        }
    }
}

// copy constructor (深拷貝，避免兩個物件指標指到同一塊記憶體)
Matrix::Matrix(const Matrix &other) {
    setUp(other.row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// destructor (釋放記憶體，避免 Memory Leak)
Matrix::~Matrix() {
    for (int i = 0; i < row; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

// 設定特定位置的值 (包含邊界防呆)
void Matrix::setData(int r, int c, double val) {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        data[r][c] = val;
    }
}

// 取得 row
int Matrix::getRow() const {
    return row;
}

// 取得 col
int Matrix::getCol() const {
    return col;
}

// 取得特定位置的值 (包含邊界防呆)
double Matrix::getData(int r, int c) const {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        return data[r][c];
    }
    return 0.0; 
}

// 矩陣相加
Matrix Matrix::add(const Matrix &other) {
    if (row != other.row || col != other.col) {
        cout << "Error: Matrix dimensions must agree for addition." << endl;
        return Matrix(row, col); 
    }
    Matrix result(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

// 矩陣相乘
Matrix Matrix::multiply(const Matrix &other) {
    if (col != other.row) {
        cout << "Error: Matrix dimensions must agree for multiplication." << endl;
        return Matrix(row, other.col);
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

// 矩陣轉置
Matrix Matrix::transpose() {
    Matrix result(col, row); 
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}