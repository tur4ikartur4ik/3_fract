#include "../include/Matrix.h"
#include <stdexcept>
#include <algorithm>
#include <functional>

using namespace std;

// Конструктор по умолчанию
Matrix::Matrix()
    : rows(0), cols(0), data(nullptr)
{}

// Конструктор матрицы заданного размера
Matrix::Matrix(int rows, int cols)
    : rows(rows), cols(cols), data(nullptr)
{
    if (rows <= 0 || cols <= 0)
        throw invalid_argument("Размеры матрицы должны быть положительными.");

    allocateMemory();
}

// Конструктор копирования
Matrix::Matrix(const Matrix& other)
    : rows(0), cols(0), data(nullptr)
{
    copyFrom(other);
}

// Оператор присваивания
Matrix& Matrix::operator=(const Matrix& other)
{
    if (this != &other)
    {
        freeMemory();
        copyFrom(other);
    }

    return *this;
}

// Деструктор
Matrix::~Matrix()
{
    freeMemory();
}

// Выделение памяти под матрицу rows x cols
void Matrix::allocateMemory()
{
    data = new Fraction*[rows];

    for (int i = 0; i < rows; i++)
    {
        data[i] = new Fraction[cols];

        for (int j = 0; j < cols; j++)
            data[i][j] = Fraction("0", "1");
    }
}

// Освобождение памяти
void Matrix::freeMemory()
{
    if (data != nullptr)
    {
        for (int i = 0; i < rows; i++)
            delete[] data[i];

        delete[] data;
        data = nullptr;
    }

    rows = 0;
    cols = 0;
}

// Копирование данных из другой матрицы
void Matrix::copyFrom(const Matrix& other)
{
    rows = other.rows;
    cols = other.cols;

    allocateMemory();

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            data[i][j] = other.data[i][j];
}

// Проверка, что матрица квадратная
void Matrix::checkSquare() const
{
    if (rows != cols)
        throw invalid_argument("Матрица должна быть квадратной.");
}

// Проверка, что размеры матриц совпадают
void Matrix::checkSameSize(const Matrix& other) const
{
    if (rows != other.rows || cols != other.cols)
        throw invalid_argument("Размеры матриц должны совпадать.");
}

// Проверка, что матрицы можно умножить
void Matrix::checkMultiplicationSize(const Matrix& other) const
{
    if (cols != other.rows)
        throw invalid_argument("Количество столбцов первой матрицы должно быть равно количеству строк второй матрицы.");
}

// Получение количества строк
int Matrix::getRows() const
{
    return rows;
}

// Получение количества столбцов
int Matrix::getCols() const
{
    return cols;
}

// Обращение к элементам матрицы через [][]
Fraction* Matrix::operator[](int i)
{
    if (i < 0 || i >= rows)
        throw out_of_range("Индекс строки выходит за границы матрицы.");

    return data[i];
}

const Fraction* Matrix::operator[](int i) const
{
    if (i < 0 || i >= rows)
        throw out_of_range("Индекс строки выходит за границы матрицы.");

    return data[i];
}

// Перегрузка >> для ввода матрицы
istream& operator>>(istream& in, Matrix& m)
{
    string num, den;

    for (int i = 0; i < m.rows; i++)
        for (int j = 0; j < m.cols; j++)
        {
            in >> num;
            in >> den;

            m.data[i][j] = Fraction(num, den);
        }

    return in;
}

// Перегрузка << для вывода матрицы
ostream& operator<<(ostream& out, const Matrix& m)
{
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
            out << m.data[i][j] << "\t";

        out << "\n";
    }

    return out;
}

// Сложение матриц
Matrix Matrix::operator+(const Matrix& other) const
{
    checkSameSize(other);

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] + other.data[i][j];

    return result;
}

// Вычитание матриц
Matrix Matrix::operator-(const Matrix& other) const
{
    checkSameSize(other);

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] - other.data[i][j];

    return result;
}

// Классический алгоритм умножения матриц
Matrix Matrix::multiplyClassic(const Matrix& other) const
{
    checkMultiplicationSize(other);

    Matrix result(rows, other.cols);

    for (int i = 0; i < rows; i++)
    {
        for (int k = 0; k < cols; k++)
        {
            Fraction a = data[i][k];

            for (int j = 0; j < other.cols; j++)
                result.data[i][j] = result.data[i][j] + a * other.data[k][j];
        }
    }

    return result;
}

// Умножение матрицы на вектор
Fraction* Matrix::multiplyByVector(const Fraction* vec, int size) const
{
    if (vec == nullptr)
        throw invalid_argument("Вектор не должен быть пустым указателем.");

    if (cols != size)
        throw invalid_argument("Количество столбцов матрицы должно быть равно размеру вектора.");

    Fraction* result = new Fraction[rows];

    for (int i = 0; i < rows; i++)
    {
        result[i] = Fraction("0", "1");

        for (int j = 0; j < cols; j++)
            result[i] = result[i] + data[i][j] * vec[j];
    }

    return result;
}

// Решение СЛАУ методом Гаусса
Fraction* Matrix::solveGauss(const Fraction* b, int size) const
{
    checkSquare();

    if (b == nullptr)
        throw invalid_argument("Столбец свободных членов не должен быть пустым указателем.");

    if (size != rows)
        throw invalid_argument("Размер столбца свободных членов должен совпадать с числом строк матрицы.");

    Fraction zero("0", "1");

    // Создаём копию матрицы A
    Matrix A(*this);

    // Создаём копию столбца свободных членов
    Fraction* rhs = new Fraction[rows];
    for (int i = 0; i < rows; i++)
        rhs[i] = b[i];

    // Прямой ход метода Гаусса
    for (int k = 0; k < rows; k++)
    {
        // Если на диагонали ноль, ищем ниже строку с ненулевым элементом
        if (A.data[k][k] == zero)
        {
            int swapRow = -1;

            for (int i = k + 1; i < rows; i++)
                if (A.data[i][k] != zero)
                {
                    swapRow = i;
                    break;
                }

            if (swapRow == -1)
            {
                delete[] rhs;
                throw runtime_error("Система не имеет единственного решения.");
            }

            // Меняем строки местами в матрице
            swap(A.data[k], A.data[swapRow]);

            // Меняем местами элементы в столбце свободных членов
            swap(rhs[k], rhs[swapRow]);
        }

        // Обнуляем элементы ниже ведущего
        for (int i = k + 1; i < rows; i++)
        {
            Fraction factor = A.data[i][k] / A.data[k][k];

            for (int j = k; j < cols; j++)
                A.data[i][j] = A.data[i][j] - factor * A.data[k][j];

            rhs[i] = rhs[i] - factor * rhs[k];
        }
    }

    // Обратная подстановка
    Fraction* x = new Fraction[rows];

    for (int i = rows - 1; i >= 0; i--)
    {
        Fraction sum("0", "1");

        for (int j = i + 1; j < cols; j++)
            sum = sum + A.data[i][j] * x[j];

        if (A.data[i][i] == zero)
        {
            delete[] rhs;
            delete[] x;
            throw runtime_error("Система не имеет единственного решения.");
        }

        x[i] = (rhs[i] - sum) / A.data[i][i];
    }

    delete[] rhs;
    return x;
}

// Решение СЛАУ методом Жордана-Гаусса
Fraction* Matrix::solveJordanGauss(const Fraction* b, int size) const
{
    checkSquare();

    if (b == nullptr)
        throw invalid_argument("Столбец свободных членов не должен быть пустым указателем.");

    if (size != rows)
        throw invalid_argument("Размер столбца свободных членов должен совпадать с числом строк матрицы.");

    Fraction zero("0", "1");

    // Копия матрицы коэффициентов
    Matrix A(*this);

    // Копия столбца свободных членов
    Fraction* rhs = new Fraction[rows];
    for (int i = 0; i < rows; i++)
        rhs[i] = b[i];

    // Прямой + обратный ход сразу
    for (int k = 0; k < rows; k++)
    {
        // Если ведущий элемент равен 0, ищем строку ниже
        if (A.data[k][k] == zero)
        {
            int swapRow = -1;

            for (int i = k + 1; i < rows; i++)
                if (A.data[i][k] != zero)
                {
                    swapRow = i;
                    break;
                }

            if (swapRow == -1)
            {
                delete[] rhs;
                throw runtime_error("Система не имеет единственного решения.");
            }

            // Меняем строки местами
            swap(A.data[k], A.data[swapRow]);
            swap(rhs[k], rhs[swapRow]);
        }

        // Нормируем ведущую строку: делаем ведущий элемент равным 1
        Fraction pivot = A.data[k][k];

        for (int j = 0; j < cols; j++)
            A.data[k][j] = A.data[k][j] / pivot;

        rhs[k] = rhs[k] / pivot;

        // Обнуляем все остальные элементы в столбце k
        for (int i = 0; i < rows; i++)
        {
            if (i == k)
                continue;

            Fraction factor = A.data[i][k];

            if (factor != zero)
            {
                for (int j = 0; j < cols; j++)
                    A.data[i][j] = A.data[i][j] - factor * A.data[k][j];

                rhs[i] = rhs[i] - factor * rhs[k];
            }
        }
    }

    // После метода Жордана-Гаусса решение уже находится в rhs
    Fraction* x = new Fraction[rows];
    for (int i = 0; i < rows; i++)
        x[i] = rhs[i];

    delete[] rhs;
    return x;
}

// Создание единичной матрицы порядка n
Matrix Matrix::identity(int n)
{
    if (n <= 0)
        throw invalid_argument("Размер единичной матрицы должен быть положительным.");

    Matrix I(n, n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            I.data[i][j] = (i == j) ? Fraction("1", "1") : Fraction("0", "1");

    return I;
}

/// Нахождение обратной матрицы методом Гаусса
Matrix Matrix::inverseGauss() const
{
    checkSquare();

    Fraction zero("0", "1");
    Fraction one("1", "1");

    // Расширенная матрица размера rows x (2 * cols)
    Matrix ext(rows, 2 * cols);;

    // Слева записываем исходную матрицу, справа — единичную
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            ext.data[i][j] = data[i][j];
            ext.data[i][j + cols] = (i == j) ? one : zero;
        }

    // Прямой ход метода Гаусса
    for (int k = 0; k < rows; k++)
    {
        // Если ведущий элемент равен 0, ищем строку ниже
        if (ext.data[k][k] == zero)
        {
            int swapRow = -1;

            for (int i = k + 1; i < rows; i++)
                if (ext.data[i][k] != zero)
                {
                    swapRow = i;
                    break;
                }

            if (swapRow == -1)
                throw runtime_error("Матрица не имеет обратной.");

            swap(ext.data[k], ext.data[swapRow]);
        }

        // Обнуляем элементы ниже ведущего
        for (int i = k + 1; i < rows; i++)
        {
            Fraction factor = ext.data[i][k] / ext.data[k][k];

            for (int j = 0; j < 2 * cols; j++)
                ext.data[i][j] = ext.data[i][j] - factor * ext.data[k][j];
        }
    }

    // Обратный ход
    for (int k = rows - 1; k >= 0; k--)
    {
        if (ext.data[k][k] == zero)
            throw runtime_error("Матрица не имеет обратной.");

        // Нормируем ведущую строку
        Fraction pivot = ext.data[k][k];

        for (int j = 0; j < 2 * cols; j++)
            ext.data[k][j] = ext.data[k][j] / pivot;

        // Обнуляем элементы выше ведущего
        for (int i = k - 1; i >= 0; i--)
        {
            Fraction factor = ext.data[i][k];

            if (factor != zero)
            {
                for (int j = 0; j < 2 * cols; j++)
                    ext.data[i][j] = ext.data[i][j] - factor * ext.data[k][j];
            }
        }
    }

    // Из правой половины расширенной матрицы собираем обратную
    Matrix inverse(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            inverse.data[i][j] = ext.data[i][j + cols];

    return inverse;
}

// Нахождение обратной матрицы методом Жордана-Гаусса
Matrix Matrix::inverseJordanGauss() const
{
    checkSquare();

    Fraction zero("0", "1");
    Fraction one("1", "1");

    // Расширенная матрица размера rows x (2 * cols)
    Matrix ext(rows, 2 * cols);

    // Слева записываем исходную матрицу, справа — единичную
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            ext.data[i][j] = data[i][j];
            ext.data[i][j + cols] = (i == j) ? one : zero;
        }

    // Метод Жордана-Гаусса
    for (int k = 0; k < rows; k++)
    {
        // Если ведущий элемент равен 0, ищем строку ниже
        if (ext.data[k][k] == zero)
        {
            int swapRow = -1;

            for (int i = k + 1; i < rows; i++)
                if (ext.data[i][k] != zero)
                {
                    swapRow = i;
                    break;
                }

            if (swapRow == -1)
                throw runtime_error("Матрица не имеет обратной.");

            swap(ext.data[k], ext.data[swapRow]);
        }

        // Нормируем ведущую строку
        Fraction pivot = ext.data[k][k];

        for (int j = 0; j < 2 * cols; j++)
            ext.data[k][j] = ext.data[k][j] / pivot;

        // Обнуляем все остальные элементы в столбце k
        for (int i = 0; i < rows; i++)
        {
            if (i == k)
                continue;

            Fraction factor = ext.data[i][k];

            if (factor != zero)
            {
                for (int j = 0; j < 2 * cols; j++)
                    ext.data[i][j] = ext.data[i][j] - factor * ext.data[k][j];
            }
        }
    }

    // Из правой половины расширенной матрицы собираем обратную
    Matrix inverse(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            inverse.data[i][j] = ext.data[i][j + cols];

    return inverse;
}

// Разбиение матрицы на блок: подматрица [rowStart..rowEnd] x [colStart..colEnd]
Matrix Matrix::subMatrix(int rowStart, int rowEnd, int colStart, int colEnd) const
{
    if (rowStart < 0 || rowEnd >= rows || colStart < 0 || colEnd >= cols || rowStart > rowEnd || colStart > colEnd)
        throw out_of_range("Некорректные границы подматрицы.");

    Matrix block(rowEnd - rowStart + 1, colEnd - colStart + 1);

    for (int i = rowStart; i <= rowEnd; i++)
        for (int j = colStart; j <= colEnd; j++)
            block.data[i - rowStart][j - colStart] = data[i][j];

    return block;
}

// Вставка блока block в текущую матрицу с позиции [rowStart][colStart]
void Matrix::insertBlock(const Matrix& block, int rowStart, int colStart)
{
    if (rowStart < 0 || colStart < 0 ||
        rowStart + block.rows > rows ||
        colStart + block.cols > cols)
        throw out_of_range("Блок не помещается в матрицу.");

    for (int i = 0; i < block.rows; i++)
        for (int j = 0; j < block.cols; j++)
            data[rowStart + i][colStart + j] = block.data[i][j];
}

// Блочный алгоритм умножения матриц
Matrix Matrix::multiplyBlock(const Matrix& other, int blockSize) const
{
    checkMultiplicationSize(other);

    if (blockSize <= 0)
        throw invalid_argument("Размер блока должен быть положительным.");

    Matrix result(rows, other.cols);

    for (int ii = 0; ii < rows; ii += blockSize)
    {
        int iEnd = (ii + blockSize - 1 < rows) ? (ii + blockSize - 1) : (rows - 1);

        for (int jj = 0; jj < other.cols; jj += blockSize)
        {
            int jEnd = (jj + blockSize - 1 < other.cols) ? (jj + blockSize - 1) : (other.cols - 1);

            // Блок результата C(ii, jj)
            Matrix Cblock(iEnd - ii + 1, jEnd - jj + 1);

            for (int kk = 0; kk < cols; kk += blockSize)
            {
                int kEnd = (kk + blockSize - 1 < cols) ? (kk + blockSize - 1) : (cols - 1);

                Matrix Ablock = subMatrix(ii, iEnd, kk, kEnd);
                Matrix Bblock = other.subMatrix(kk, kEnd, jj, jEnd);

                Cblock = Cblock + Ablock.multiplyClassic(Bblock);
            }

            result.insertBlock(Cblock, ii, jj);
        }
    }

    return result;
}

// Алгоритм Штрассена для умножения матриц
Matrix Matrix::multiplyStrassen(const Matrix& other, int bound) const
{
    checkMultiplicationSize(other);

    // Размер квадратной матрицы, до которого будем дополнять нулями
    int size = 1;
    while (size < max(rows, max(cols, other.cols))) size *= 2;

    // Если порог не задан, выбираем автоматически
    if (bound == 0)
    {
        // size = 2^k
        // берём примерно половину степени двойки
        int tmp = size;
        int power = 0;

        while (tmp > 1)
        {
            tmp /= 2;
            power++;
        }

        bound = 1;
        for (int i = 0; i < power / 2; i++)
            bound *= 2;

        // Чтобы порог не был слишком маленьким
        if (bound < 2)
            bound = 2;
    }

    // Расширенные квадратные матрицы size x size, заполненные нулями
    Matrix Aext(size, size);
    Matrix Bext(size, size);

    // Копируем текущую матрицу в левый верхний угол Aext
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            Aext.data[i][j] = data[i][j];

    // Копируем other в левый верхний угол Bext
    for (int i = 0; i < other.rows; i++)
        for (int j = 0; j < other.cols; j++)
            Bext.data[i][j] = other.data[i][j];

    function<Matrix(const Matrix&, const Matrix&)> strassen =
        [&](const Matrix& A, const Matrix& B) -> Matrix
    {
        int n = A.rows;

        if (n <= 32)
            return A.multiplyClassic(B);

        int mid = n / 2;

        Matrix A11 = A.subMatrix(0,   mid - 1, 0,   mid - 1);
        Matrix A12 = A.subMatrix(0,   mid - 1, mid, n - 1);
        Matrix A21 = A.subMatrix(mid, n - 1,   0,   mid - 1);
        Matrix A22 = A.subMatrix(mid, n - 1,   mid, n - 1);

        Matrix B11 = B.subMatrix(0,   mid - 1, 0,   mid - 1);
        Matrix B12 = B.subMatrix(0,   mid - 1, mid, n - 1);
        Matrix B21 = B.subMatrix(mid, n - 1,   0,   mid - 1);
        Matrix B22 = B.subMatrix(mid, n - 1,   mid, n - 1);

        Matrix M1 = strassen(A11 + A22, B11 + B22);
        Matrix M2 = strassen(A21 + A22, B11);
        Matrix M3 = strassen(A11,       B12 - B22);
        Matrix M4 = strassen(A22,       B21 - B11);
        Matrix M5 = strassen(A11 + A12, B22);
        Matrix M6 = strassen(A21 - A11, B11 + B12);
        Matrix M7 = strassen(A12 - A22, B21 + B22);

        Matrix C11 = M1 + M4 - M5 + M7;
        Matrix C12 = M3 + M5;
        Matrix C21 = M2 + M4;
        Matrix C22 = M1 - M2 + M3 + M6;

        Matrix C(n, n);

        C.insertBlock(C11, 0, 0);
        C.insertBlock(C12, 0, mid);
        C.insertBlock(C21, mid, 0);
        C.insertBlock(C22, mid, mid);

        return C;
    };

    Matrix Cext = strassen(Aext, Bext);

    Matrix result(rows, other.cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < other.cols; j++)
            result.data[i][j] = Cext.data[i][j];

    return result;
}
