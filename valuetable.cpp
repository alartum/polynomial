#include "valuetable.h"
#include <QDebug>

ValueTable::ValueTable(): QList< QPair<int, int> >()
{
}

ValueTable::ValueTable(Polynomial P) : QList< QPair<int, int> >()
{
    int start;// Самый маленький x в таблице.
    int end;// Самый большой.
    int counter = 0;
    int degree = P.size() - 1;// Степень полинома.

    if (degree % 2 == 0){
        start = - degree / 2;
        end = degree / 2;
    }
    else{
        start = - (degree + 1) / 2;
        end = (degree - 1) / 2;
    }
    for (int i = start; i <= end; ++ i, ++ counter){
        QPair <int, int> xyPair;
        xyPair.first = i;
        xyPair.second = P.value(i);
        this->append(xyPair);
    }
}

void ValueTable::print() const
{
    qDebug() << "ValueTable::print()";

    for (int i = 0; i < this->size(); ++ i)
        qDebug() << "(" << this->at(i).first << ";" << this->at(i).second << ")";
}

Polynomial ValueTable::interpolate() const
{
    Polynomial numerator;// Числитель дроби.

    // Находим числитель интерполяционного полинома
    for (int i = 0; i < this->size(); ++ i)
    {
        Polynomial L;// Числитель дроби данного базисного полинома
        Polynomial Q (1);// Полином вида (x-x[j])
        L[0] = 1;
        for (int j = 0; j < this->size(); ++ j)
        {
            if (j == i)
                continue;
            Q[0] = - this->at(j).first;
            Q[1] = 1;
            L = L * Q;
        }
        L = L * calcPartPair(i) * this->at(i).second;// Домножаем, чтобы знаменатели были равны, затем умножаем на y[i]
        if (i % 2 == 0)
            numerator = numerator + L;	// Если добавляем многочлен Лагранжа с чётным номером, его необходимо прибавить
        else
            numerator = numerator - L;	// Иначе - вычесть
    }

    int denumerator = calcFullPair();// Знаменатель дроби
    int i = 0;// Для цикла и последующего анализа

    for (; (i < numerator.size()) && (numerator.at(i) % denumerator == 0); ++ i)
        numerator[i] /= denumerator;
    if (i == numerator.size())
        numerator.removeZeros();
    else
        numerator.setIsValid(false);

    return numerator;
}

int ValueTable::calcPartPair (int without) const
{
    int result = 1;
    int max = this->size();

    for (int i = 0; i < max - 1; ++ i){
        if (i == without)
            continue;
        for (int j = i + 1; j < max; ++ j){
            if (j == without)
                continue;
            result *= this->at(i).first - this->at(j).first;
        }
    }

    return result;
}

int ValueTable::calcFullPair () const
{
    int result = 1;
    int max = this->size();

    for (int i = 0; i < max - 1; ++ i)
        for (int j = i + 1; j < max; ++ j)
            result *= this->at(i).first - this->at(j).first;

    return result;
}
