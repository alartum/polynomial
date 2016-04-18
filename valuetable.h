#ifndef VALUETABLE_H
#define VALUETABLE_H

#include <QList>
#include <QPair>
#include "polynomial.h"

// Контейнер для таблицы значений.
// first = x
// second = y
class ValueTable : public QList< QPair<int, int> >
{
public:
    ValueTable();
    ValueTable(Polynomial P);

    void print() const;
    Polynomial interpolate() const;
private:
    // Счёт произведения всех возможных пар чисел в массиве вида (x[i]-x[j]), в которых нет элемента x[without]
    int calcPartPair (int without) const;
    // Счёт произведения всех возможных пар чисел в массиве вида (x[i]-x[j])
    int calcFullPair () const;
};

#endif // VALUETABLE_H
