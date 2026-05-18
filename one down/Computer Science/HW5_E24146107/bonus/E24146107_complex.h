// 【備註：這是「加分題 (Bonus)」版本 - 處理 Complex】
#ifndef E24146107_COMPLEX_H
#define E24146107_COMPLEX_H

class Complex {
public:
    Complex();  // default constructor 存入 1+2i
    Complex(double, double);
    Complex add(Complex);
    Complex multiply(Complex);
    double getReal() const; 
    double getImag() const; 
    void print() const;     

private:
    double real; 
    double imag; 
};

#endif