#include <iostream>
#include <fstream>
#include "../include/Matrix.h"

#include <ctime>

using namespace std;

int main()
{
    clock_t t;
    int size = 100;

    auto timee = [&]() {return double(clock() - t) / CLOCKS_PER_SEC;};

    Matrix A(size, size);
    Matrix B(size, size);
    Matrix C(size, size);
    Fraction* vector = new Fraction[size];

    // Якобы вектор из случайных дробей
    for(int i = 0; i < size; i++)
        vector[i].Set(to_string((i + 20)/2 * 30 + 36 * (size - i*22) / 13 ), to_string((i + 70) / 7 * 23 - 6 * (size - i/22) * 13 + size));    

    ifstream F("mpz_tDATA1.txt");
    F >> A;

    ifstream F2("mpz_tDATA2.txt");
    F2 >> B;

    t = clock();
    A.multiplyByVector(vector, size);
    cout << "Умножение матрицы на вектор:" << timee() << " секунд\n";

    t = clock();
    A.multiplyClassic(B);
    cout << "Умножение матриц классическим методом: " << timee() << " секунд\n";

    t = clock();
    A.multiplyBlock(B);
    cout << "Умножение матриц блочным методом: " << timee() << " секунд\n";

    t = clock();
    A.multiplyStrassen(B);
    cout << "Умножение матриц методом Штрассена: " << timee() << " секунд\n";

    t = clock();
    A.solveGauss(vector, size);
    cout << "Решение СЛАУ методом Гаусса: " << timee() << " секунд\n";

    t = clock();
    A.solveJordanGauss(vector, size);
    cout << "Решение СЛАУ методом Жордана-Гаусса: " << timee() << " секунд\n";

    t = clock();
    A.inverseGauss();
    cout << "Нахождение обратной матрицы методом Гаусса: " << timee() << " секунд\n";

    t = clock();
    A.inverseJordanGauss();
    cout << "Нахождение обратной матрицы методом Жордана-Гаусса: " << timee() << " секунд\n";
}

// что скажешь насчёт времени размер 40
// Умножение матрицы на вектор:0.002741 секунд
// Умножение матриц классическим методом: 0.142781 секунд
// Умножение матриц блочным методом прошло за 0.121902 секунд
// Умножение матриц методом Штрассена: 0.94443 секунд
// Решение СЛАУ методом Гаусса: 1.26241 секунд
// Решение СЛАУ методом Жордана-Гаусса: 2.51057 секунд
// Нахождение обратной матрицы методом Гаусса: 49.6346 секунд
// Нахождение обратной матрицы методом Жордана-Гаусса: 98.3429 секунд
