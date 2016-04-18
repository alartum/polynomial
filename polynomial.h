#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <QList>
#include <QString>

class Polynomial : public QList <int>
{
public:
    // Конструктор нулевого многочлена.
    Polynomial ();
    // Конструктор многочлена заданной степени.
    Polynomial (int degree);
    // Копирующий конструктор.
    Polynomial (const Polynomial &other);

    // Операторы.
    /// Возможно, использование возврата по ссылке некорректно (т.к. передаются локальные переменные)
    bool operator < (const Polynomial &other) const;
    bool operator == (const Polynomial &other) const;
    const Polynomial operator + (const Polynomial &summand) const;
    const Polynomial operator - (const Polynomial &subtrahend) const;
    const Polynomial operator * (const Polynomial &multiplier) const;
    const Polynomial operator * (int n) const;
    // Нахождение остатка одного многочлена при делении на другой.
    const Polynomial operator % (const Polynomial &divider) const;
    // Нахождение частного от деления многочлена на многочлен.
    const Polynomial operator / (const Polynomial &divider) const;
    const Polynomial operator / (int n) const;
    void divide (const Polynomial &divider, Polynomial &quotient, Polynomial &modulo) const;
    // Вычисление значения полинома в данной точке
    int value (int x);
    // Вывод полинома в строку.
    QString toString() const;
    // Копирование полинома.
    void copy (const Polynomial &other);
    // Удаляем старшие нулевые коэффициенты, если они есть.
    void removeZeros();
    void setIsValid(bool isValid);
    bool isValid() const;
    int  degree() const;

private:
    // Является ли многочлен многочленом над полем целых чисел.
    bool mIsValid;
};

#endif // POLYNOMIAL_H
