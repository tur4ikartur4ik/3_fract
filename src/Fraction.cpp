#include "../include/Fraction.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

// Конструктор по умолчанию
Fraction::Fraction()
{}

// Конструктор с параметрами
Fraction::Fraction(string num, string den)
{
    Set(num, den);
}

// Установка значений числителю и знаменателю
void Fraction::Set(string num, string den)
{
    auto removeSpaces = [](string s)
    {
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        return s;
    };

    num = removeSpaces(num);
    den = removeSpaces(den);

    // Проверка на пустые строки
    if (num.empty() || den.empty())
        throw invalid_argument("Некорректный ввод данных.");

    numerator = num;
    denominator = den;

    // Проверка знаменателя на 0
    if (denominator == 0)
        throw invalid_argument("Знаменатель не может быть равен 0.");

    // Если знаменатель отрицательный, переносим минус в числитель
    if (denominator < 0)
    {
        numerator = -numerator;
        denominator = -denominator;
    }

    reduce();
}

// Сокращение дроби
void Fraction::reduce()
{
    mpz_class d = gcd(numerator, denominator);
    numerator /= d;
    denominator /= d;
}

// Формирование десятичной дроби
string Fraction::toDecimal(int maxDigits = 26, bool points = false) const
{
    // Если числитель равен 0, то дробь равна 0
    if (numerator == 0)
        return "0";

    mpz_class numAbs = abs(numerator);        // Модуль числителя
    mpz_class denAbs = abs(denominator);      // Модуль знаменателя
    mpz_class integerPart = numAbs / denAbs;  // Целая часть
    mpz_class remainder = numAbs % denAbs;    // Остаток

    string result;      // Сюда собираем весь ответ
    string fractional;  // Сюда собираем цифры после запятой

    // Если дробь отрицательная, добавим знак "-"
    if (sgn(numerator) < 0)
        result += "-";

    // Добавляем целую часть
    result += integerPart.get_str();

    // Если остаток равен 0, дробь целая
    if (remainder == 0)
        return result;

    // Добавляем десятичную точку
    result += ".";

    // Пока остаток не стал нулём и не превысили лимит цифр
    while (remainder != 0 && fractional.size() < maxDigits)
    {
        // Умножаем остаток на 10
        remainder *= 10;

        // Находим очередную цифру после запятой
        mpz_class digit = remainder / denAbs;

        // Обновляем остаток
        remainder %= denAbs;

        // Добавляем цифру в строку
        fractional += char('0' + digit.get_ui());
    }

    result += fractional;

    // Если дробь не закончилась, добавим многоточие
    if (remainder != 0 && points)
        result += "...";

    return result;
}

// Перегрузка << для вывода на экран и в файл
ostream& operator<<(ostream& out, const Fraction& f)
{
    out << f.numerator << " / " << f.denominator;
    return out;
}

// Оператор присваивания
Fraction& Fraction::operator=(const Fraction& other)
{
    if (this != &other)
    {
        numerator = other.numerator;
        denominator = other.denominator;
    }

    return *this;
}

// Перегрузка + : a/b + c/d = (ad + bc) / (bd)
Fraction Fraction::operator+(const Fraction& other) const
{
    Fraction result("0", "1");

    result.denominator = denominator * other.denominator;
    result.numerator = numerator * other.denominator + other.numerator * denominator;

    result.reduce();
    return result;
}

// Перегрузка - : a/b - c/d = (ad - bc) / (bd)
Fraction Fraction::operator-(const Fraction& other) const
{
    Fraction result("0", "1");

    result.denominator = denominator * other.denominator;
    result.numerator = numerator * other.denominator - other.numerator * denominator;

    result.reduce();
    return result;
}

// Перегрузка * : a/b * c/d = (ac) / (bd)
Fraction Fraction::operator*(const Fraction& other) const
{
    Fraction result("0", "1");

    result.numerator = numerator * other.numerator;
    result.denominator = denominator * other.denominator;

    result.reduce();
    return result;
}

// Перегрузка / : (a/b) / (c/d) = (a/b) * (d/c)
Fraction Fraction::operator/(const Fraction& other) const
{
    // Если числитель дроби справа равен 0, деление невозможно
    if (other.numerator == 0)
        throw invalid_argument("Деление на ноль невозможно.");

    Fraction result("0", "1");

    result.numerator = numerator * other.denominator;
    result.denominator = denominator * other.numerator;

    // Если знаменатель отрицательный, переносим минус в числитель
    if (result.denominator < 0)
    {
        result.numerator = -result.numerator;
        result.denominator = -result.denominator;
    }

    result.reduce();
    return result;
}

// Перегрузка == : дроби равны, если равны числители и знаменатели
bool Fraction::operator==(const Fraction& other) const
{
    return numerator == other.numerator && denominator == other.denominator;
}

// Перегрузка != : дроби не равны, если == ложно
bool Fraction::operator!=(const Fraction& other) const
{
    return !(*this == other);
}

// Деструктор
Fraction::~Fraction()
{}