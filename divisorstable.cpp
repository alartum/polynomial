#include "divisorstable.h"
#include <QDebug>

DivisorsTable::DivisorsTable(ValueTable valueTable) : QList < QPair <int, Divisors> >()
{
    for (int i = 0; i < valueTable.size(); ++ i){
        QPair <int, Divisors> xyPair;
        QPair <int, int> valuePair = valueTable.at(i);

        xyPair.first = valuePair.first;
        xyPair.second = Divisors (valuePair.second);
        this->append(xyPair);
    }
}

void DivisorsTable::print() const
{
   qDebug() << "DivisorsTable::print()";
   for (int i = 0; i < this->size(); ++ i){
       qDebug() << this->at(i).first;
       this->at(i).second.print();
   }
}
