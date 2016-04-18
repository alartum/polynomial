#ifndef FACTORSTACK_H
#define FACTORSTACK_H

#include <QStack>
#include <QObject>
#include "valuetable.h"
#include "polynomial.h"
#include "divisors.h"
#include "divisorstable.h"
#include <QBitArray>
#include <QVector>

class FactorStack : public QObject, public QStack <Polynomial>
{
    Q_OBJECT
public:
    FactorStack();
    // Создаём новый стак факторизации с полиномом внутри.
    FactorStack(Polynomial P);

    // Устанавливаем полином для разложения.
    void setPolynomial(Polynomial P);
    // Вывод
    void print() const;
private:
    int mVariants;
    int mVariantsDone;
    // Находим НОД всех коэффициентов полинома
    int findGCD (const Polynomial &P);
    // Подготавливаем верхний многочлен для разложения: чистим его линейные множители. Возвращает true, если что-то убрали
    void removeTopRoots();
    // Обрабатываем верхний полином.
    void manageTop ();
    // Проверка, является ли полином, построенный по таблице, делителем вернего в стэке.
    bool checkFactor (const ValueTable &table);

    // Поиск делителя идёт так: checkValueCombinations() -> checkDivisorsCombinations() ->
    // -> checkMinusLocation() -> checkFactor()

    // Поиск всех возможных комбинаций значений из таблицы значений.
    bool checkValueCombinations (const ValueTable &table);
    //поиск всех возможных комбинаций делителей для данной таблицы значений
    bool checkDivisorsCombinations (const DivisorsTable &table);
    //поиск всех возможных положений знака "-" в таблице значений
    bool checkMinusLocation (const ValueTable &table);
public slots:
    void factorize();
signals:
    void factorFound (Polynomial P);
    void constantFound (int constant);
    void factorized ();
    void progressDone (int percents);
};

#endif // FACTORSTACK_H
