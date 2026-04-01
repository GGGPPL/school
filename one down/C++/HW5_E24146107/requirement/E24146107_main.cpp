// 【備註：這是「必寫題 (Requirement)」版本 - 處理 double】
#include "E24146107_matrix.h"
#include <iostream>

using namespace std;

int main() {
    Matrix m1;
    double ds[9] = { 1,2,3,4,5,6,7,8,9 };
    Matrix m2(3, 3, ds, 9);
    Matrix m3(m2);

    cout << "\nMatrix m1 is : \n";
    m1.displayData();
    cout << "\nMatrix m2 is : \n";
    m2.displayData();
    cout << "\nMatrix m3 is : \n";
    m3.displayData();

    int r = 1, c = 2; 
    m3.setData(r, c, 99);
    cout << "\n( " << r << "," << c << " ) of Matrix m3 is :  " << m3.getData(r, c) << "\n";
    cout << "\nMatrix m3 after setup is : \n";
    m3.displayData();
    cout << "\ntranspose of Matrix m3 is :  \n";
    m3.transpose().displayData();

    Matrix m4(3, 3);
    cout << "\nrow of Matrix m4 is :  " << m4.getRow();
    cout << "\ncol of Matrix m4 is :  " << m4.getCol() << "\n";

    cout << "\n\n m2 + m3 is : \n";
    m2.add(m3).displayData();

    cout << "\n\n m2 * m3 is : \n";
    m2.multiply(m3).displayData();

    return 0;
}