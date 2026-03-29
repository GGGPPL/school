#include <iostream>
#include <string>

// Include your self-defined complex number
#include "E24146107.h"
using namespace std;

int main()
{
    // Initialize two self-defined complex numbers
    Complex test1 = Complex(1, 2);
    Complex test2 = Complex(3, 4);
    cout << "test1: ";
    test1.print();
    cout << endl;
    cout << "test2: ";
    test2.print();
    cout << endl;

    Complex addition = test1.add(test2);
    cout << "Add: ";
    addition.print();
    cout << endl;

    Complex multiplication = test1.multiply(test2);
    cout << "Multiply: ";
    multiplication.print();
    cout << endl;

    Complex division = test1.divide(test2);
    cout << "Divide: ";
    division.print();
    cout << endl;

    Complex conjugate_value = test1.conjugate();
    cout << "Complex conjugate: ";
    conjugate_value.print();
    cout << endl;

    return 0;
}