#ifndef FACTORSMAP_H
#define FACTORSMAP_H

#include <QObject>
#include <QMap>
#include <QString>
#include "polynomial.h"

class FactorsMap : public QObject, public QMap <QString, int>
{
    Q_OBJECT
public:
    FactorsMap();
    FactorsMap(const Polynomial &P);
    QString toString() const;
private:
    // Числовая константа, вынесенная из многочлена, если есть.
    int mConstant;
public slots:
    void addPolynomial (const Polynomial &P);
    void addConstant (int constant);
    void reset();
};

#endif // FACTORSMAP_H
