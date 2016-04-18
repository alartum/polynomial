#ifndef DIVISORS_H
#define DIVISORS_H

#include <QList>

// Контейнер для делителей.
class Divisors : public QList<int>
{
public:
    Divisors ();
    // Находим делители n.
    Divisors(int n);

    // Выводим на экран.
    void print() const;
};

#endif // DIVISORS_H
