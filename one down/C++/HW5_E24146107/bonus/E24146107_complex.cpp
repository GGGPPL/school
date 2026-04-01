// 【備註：這是「加分題 (Bonus)」版本 - 處理 Complex】
#include "E24146107_complex.h"
#include <iostream>

using namespace std;

Complex::Complex() {
    real = 1.0;
    imag = 2.0;
}

Complex::Complex(double r, double i) {
    real = r;
    imag = i;
}

Complex Complex::add(Complex other) {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::multiply(Complex other) {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
}

double Complex::getReal() const {
    return real;
}

double Complex::getImag() const {
    return imag;
}

void Complex::print() const {
    cout << real;
    if (imag >= 0) {
        cout << "+" << imag << "i";
    } else {
        cout << imag << "i";
    }
}