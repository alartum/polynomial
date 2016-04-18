#include "factorsmap.h"
#include <QDebug>

FactorsMap::FactorsMap() : QMap <QString, int>(), mConstant (1)
{

}

FactorsMap::FactorsMap(const Polynomial &P) : QMap <QString, int>(), mConstant (1)
{
    this->insert(P.toString(), 1);
}

void FactorsMap::addPolynomial (const Polynomial &P)
{
    qDebug() << "Received:" << P.toString();
    (*this)[P.toString()] ++;
}

QString FactorsMap::toString() const
{
    QString result;

    if (isEmpty())
        result += " неприводим\n";
    else if (this->size() == 1 && this->first() == 1 && (mConstant == 1))
        result += " неприводим\n";
    else{
        result += " = ";
        if (mConstant != 1)
            result += QString("%1").arg(mConstant);

        QMap <QString, int>::const_iterator i = this->constBegin();

        for (;i != this->constEnd(); ++ i) {
            result += QString ("(");
            result += i.key();

            int factorDegree = i.value();// Степень, в которой множитель входит в разложение.

            if (factorDegree != 1)
                result += QString (")^%1").arg(factorDegree);
            else
                result += QString (")");
        }
    }

    return result;
}

void FactorsMap::addConstant (int constant)
{
    mConstant *= constant;
}

void FactorsMap::reset()
{
    this->clear();
    mConstant = 1;
}
