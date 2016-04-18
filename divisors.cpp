#include "divisors.h"
#include "QDebug"

Divisors::Divisors () : QList()
{
}

Divisors::Divisors(int n)
{
    if (n == 0)
        qDebug() << "Divisors: error! 0 has been passed!";
    else if (n < 0)
        n = -n;

    int i = 1;

    for (; i * i < n; ++ i)
        if (n % i == 0) {
            this->append(i);
            this->append(n / i);
        }
    if ((i * i) == n)
        this->append(i);
}

void Divisors::print() const
{
    qDebug() << "Divisors::print()";
    foreach (int divisor, *this)
        qDebug() << divisor << "  ";
}

