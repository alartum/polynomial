#ifndef DIVISORSTABLE_H
#define DIVISORSTABLE_H

#include "divisors.h"
#include "valuetable.h"
#include <QList>
#include <QPair>

//Контенер для таблицы с делителями значений
// first = x (integer)
// second = делители y (Divisors)
class DivisorsTable : public QList < QPair <int, Divisors> >
{
public:
    DivisorsTable(ValueTable valueTable);

    void print() const;
};

#endif // DIVISORSTABLE_H
