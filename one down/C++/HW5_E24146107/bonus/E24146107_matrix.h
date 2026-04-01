// 【備註：這是「加分題 (Bonus)」版本 - 處理 Complex】
#ifndef E24146107_MATRIX_H
#define E24146107_MATRIX_H

#include "E24146107_complex.h"
#include <iostream>

using namespace std;

class Matrix {
private:
    Complex ** data;    
    int row;            
    int col;            
    void setUp(int, int);   

public:
    Matrix();
    Matrix(int, int);
    Matrix(int, int, Complex[], int);
    Matrix(const Matrix &);
    ~Matrix();

    void setData(int, int, const Complex &);
    int getRow() const;     
    int getCol() const;     
    Complex getData(int, int) const;    
    Matrix add(const Matrix &); 
    Matrix multiply(const Matrix &);    
    Matrix transpose();

    void displayData() { 
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                data[i][j].print();
                cout << "\t";
            }
            cout << "" << endl;
        }
    }
};

#endif