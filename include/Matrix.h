#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include "Fraction.h"

using namespace std;

class Matrix
{
private:
    int rows;           // Количество строк
    int cols;           // Количество столбцов
    Fraction** data;    // Двумерный динамический массив элементов матрицы

    // Выделение памяти под матрицу rows x cols
    void allocateMemory();

    // Освобождение памяти
    void freeMemory();

    // Копирование данных из другой матрицы
    void copyFrom(const Matrix& other);

    // Проверка, что матрица квадратная
    void checkSquare() const;

    // Проверка, что размеры матриц совпадают
    void checkSameSize(const Matrix& other) const;

    // Проверка, что матрицы можно умножить
    void checkMultiplicationSize(const Matrix& other) const;

    // Создание единичной матрицы порядка n
    static Matrix identity(int n);

    // Разбиение матрицы на блок: подматрица [rowStart..rowEnd] x [colStart..colEnd]
    Matrix subMatrix(int rowStart, int rowEnd, int colStart, int colEnd) const;

    // Вставка блока block в текущую матрицу с позиции [rowStart][colStart]
    void insertBlock(const Matrix& block, int rowStart, int colStart);

public:
    // Конструктор по умолчанию
    Matrix();

    // Конструктор матрицы заданного размера
    Matrix(int rows, int cols);

    // Конструктор копирования
    Matrix(const Matrix& other);

    // Оператор присваивания
    Matrix& operator=(const Matrix& other);

    // Деструктор
    ~Matrix();

    // Получение количества строк
    int getRows() const;

    // Получение количества столбцов
    int getCols() const;

    Fraction* operator[](int i);
    const Fraction* operator[](int i) const;

    // Ввод матрицы с клавиатуры
    friend istream& operator>>(istream& in, Matrix& m);

    // Перегрузка << для вывода матрицы
    friend ostream& operator<<(ostream& out, const Matrix& m);

    // Сложение матриц
    Matrix operator+(const Matrix& other) const;

    // Вычитание матриц
    Matrix operator-(const Matrix& other) const;

    // Классический алгоритм умножения матриц
    Matrix multiplyClassic(const Matrix& other) const;

    // Блочный алгоритм умножения матриц
    Matrix multiplyBlock(const Matrix& other, int blockSize = 2) const;

    // Алгоритм Штрассена для умножения матриц
    Matrix multiplyStrassen(const Matrix& other, int bound = 0) const;

    // Умножение матрицы на вектор
    // vec - одномерный динамический массив длины size
    // Функция возвращает новый динамический массив длины rows
    Fraction* multiplyByVector(const Fraction* vec, int size) const;

    // Решение СЛАУ методом Гаусса
    // b - столбец свободных членов длины rows
    // Возвращает новый динамический массив решений длины cols
    Fraction* solveGauss(const Fraction* b, int size) const;

    // Решение СЛАУ методом Жордана-Гаусса
    // b - столбец свободных членов длины rows
    // Возвращает новый динамический массив решений длины cols
    Fraction* solveJordanGauss(const Fraction* b, int size) const;

    // Нахождение обратной матрицы методом Гаусса
    Matrix inverseGauss() const;

    // Нахождение обратной матрицы методом Жордана-Гаусса
    Matrix inverseJordanGauss() const;
};

#endif