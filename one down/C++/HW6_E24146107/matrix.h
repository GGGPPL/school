#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
using namespace std;

class Matrix
{
private:
    double **data;        // store all the values according to number of rows and columes
    int row;              // number of rows
    int col;              // number of columns
    static int count;     // store number of matrix objects
    void setUp(int, int); // set up number of rows and columes

public:
    Matrix();                        // default constructor, build a 2*2 matrix with all values 0
    Matrix(int, int);                // set up number of rows and columes with all values 0
    Matrix(int, int, double[], int); // convert the array into a matrix according to number of rows and columns
    Matrix(const Matrix &);          // copy constructor
    ~Matrix();                       // destructor

    void setData(int, int, double); // set up the value at the position, (row, col)

    int getRow() const;             // get the number of rows
    int getCol() const;             // get the number of columns
    double getData(int, int) const; // get the value at the position, (row, col)
    static int getCount();          // get the number of objects now

    void displayData()
    { // display data
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << data[i][j] << "\t";
            }
            cout << endl;
        }
    }

    // ====== HW6 新增的 Operator Overloading 宣告 ======
    
    // 1. << : Matrix 輸出 (設定為 friend 才能存取 private data)
    friend ostream& operator<<(ostream& os, const Matrix& m);
    
    // 2. >> : Matrix 輸入 (設定為 friend 才能存取 private data)
    friend istream& operator>>(istream& is, Matrix& m);
    
    // 3. = : Assignment operator (指派運算子，需回傳參考以支援連續指派 a=b=c)
    Matrix& operator=(const Matrix& other);
    
    // 4. + : Matrix + Matrix (矩陣相加)
    Matrix operator+(const Matrix& other) const;
    
    // 5. + : Matrix + double (矩陣所有數值加上該 double)
    Matrix operator+(double val) const;
    
    // 6. += : Matrix += Matrix
    Matrix& operator+=(const Matrix& other);
    
    // 7. ++ : 後置遞增 (例如 m5++)，注意參數需放 int 作為後置的識別
    Matrix operator++(int);
    
    // 8. * : Matrix * Matrix (矩陣相乘)
    Matrix operator*(const Matrix& other) const;
    
    // 9. () : 實作轉置矩陣 (Transpose) 功能
    Matrix operator()() const;
    
    // 10. == : 測試兩個矩陣是否包含相同數值
    bool operator==(const Matrix& other) const;
};

#endif