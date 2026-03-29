#include "E24146107.h"
#include <iostream>
#include <iomanip>

using namespace std;

// 建構子
Complex::Complex(double r, double i, bool err) : real(r), imag(i), is_error(err) {}

Complex Complex::add(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::subtract(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::multiply(const Complex& other) const {
    return Complex(real * other.real - imag * other.imag, 
                   real * other.imag + imag * other.real);
}

Complex Complex::divide(const Complex& other) const {
    double denominator = other.real * other.real + other.imag * other.imag;
    if (denominator == 0) {
        // 分母為 0，標記為 Error 狀態
        return Complex(0.0, 0.0, true); 
    }
    double r = (real * other.real + imag * other.imag) / denominator;
    double i = (imag * other.real - real * other.imag) / denominator;
    return Complex(r, i);
}

Complex Complex::conjugate() const {
    return Complex(real, -imag);
}

void Complex::print() const {
    // 規定 7：除以零輸出 Error
    if (is_error) {
        cout << "Error" << endl;
        return;
    }

    // 規定 8：全為 0 時輸出 0
    if (real == 0 && imag == 0) {
        cout << "0" << endl;
        return;
    }

    // 內部輔助函式：如果是整數就不印小數點，如果是小數就印兩位
    auto printNum = [](double num) {
        if (num == (int)num) cout << (int)num;
        else cout << fixed << setprecision(2) << num;
    };

    // 規定 5, 6, 1~4 的各式排列組合
    if (real == 0) {
        printNum(imag);
        cout << "i";
    } else if (imag == 0) {
        printNum(real);
    } else {
        printNum(real);
        if (imag > 0) {
            cout << "+";
            printNum(imag);
            cout << "i";
        } else {
            printNum(imag); // 負數自帶負號
            cout << "i";
        }
    }
    cout << endl;
}