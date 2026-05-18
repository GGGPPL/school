// 【備註：這是「加分題 (Bonus)」版本 - 處理 Complex】
#include "E24146107_matrix.h"

void Matrix::setUp(int r, int c) {
    row = r;
    col = c;
    data = new Complex*[row];
    for (int i = 0; i < row; ++i) {
        data[i] = new Complex[col]; 
    }
}

Matrix::Matrix() {
    setUp(2, 2);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = Complex(0, 0);
        }
    }
}

Matrix::Matrix(int r, int c) {
    setUp(r, c);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = Complex(0, 0);
        }
    }
}

Matrix::Matrix(int r, int c, Complex arr[], int len) {
    setUp(r, c);
    int k = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (k < len) {
                data[i][j] = arr[k++];
            } else {
                data[i][j] = Complex(0, 0);
            }
        }
    }
}

Matrix::Matrix(const Matrix &other) {
    setUp(other.row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < row; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::setData(int r, int c, const Complex &val) {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        data[r][c] = val;
    }
}

int Matrix::getRow() const {
    return row;
}

int Matrix::getCol() const {
    return col;
}

Complex Matrix::getData(int r, int c) const {
    if (r >= 0 && r < row && c >= 0 && c < col) {
        return data[r][c];
    }
    return Complex(0, 0);
}

Matrix Matrix::add(const Matrix &other) {
    if (row != other.row || col != other.col) {
        cout << "Error: Matrix dimensions must agree for addition." << endl;
        return Matrix(row, col);
    }
    Matrix result(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[i][j] = data[i][j].add(other.data[i][j]);
        }
    }
    return result;
}

Matrix Matrix::multiply(const Matrix &other) {
    if (col != other.row) {
        cout << "Error: Matrix dimensions must agree for multiplication." << endl;
        return Matrix(row, other.col);
    }
    Matrix result(row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < other.col; ++j) {
            result.data[i][j] = Complex(0, 0); 
            for (int k = 0; k < col; ++k) {
                result.data[i][j] = result.data[i][j].add(data[i][k].multiply(other.data[k][j]));
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(col, row);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}