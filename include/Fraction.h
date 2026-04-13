#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <gmpxx.h>
#include <string>

using namespace std;

class Fraction
{
private:
    mpz_class numerator;
    mpz_class denominator;

public:
    // Конструктор по умолчанию
    Fraction();

    // Конструктор с параметрами
    Fraction(string num, string den);

    // Установка значений числителю и знаменателю
    void Set(string num, string den);

    // Сокращение дроби
    void reduce();

    // Перевод в десятичную дробь
    string toDecimal(int maxDigits, bool points) const;

    // Оператор присваивания
    Fraction& operator=(const Fraction& other);

    // Перегрузки арифметических операций
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    // Перегрузки операторов сравнения
    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;

    // Перегрузка вывода
    friend ostream& operator<<(ostream& out, const Fraction& f);

    // Деструктор
    ~Fraction();
};

#endif
