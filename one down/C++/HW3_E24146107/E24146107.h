#ifndef E24146107_H
#define E24146107_H

class Complex {
private:
    double real;
    double imag;
    bool is_error; // 用來標記除以零的錯誤狀態

public:
    // 建構子新增 err 參數，預設為 false
    Complex(double r = 0.0, double i = 0.0, bool err = false);

    Complex add(const Complex& other) const;
    Complex subtract(const Complex& other) const;
    Complex multiply(const Complex& other) const;
    Complex divide(const Complex& other) const;
    Complex conjugate() const;
    
    void print() const;
};

#endif