#include "polynomial.h"
#include <QDebug>

Polynomial::Polynomial () : QList <int>(), mIsValid (true)
{
    this->append(0);
}

Polynomial::Polynomial (int degree) : QList <int>(), mIsValid (true)
{
    if (degree >= 0)
        for (int i = 0; i <= degree; ++ i)
             this->append(0);
    else{
        qDebug() << "Polynomial: Negative degree was passed to constructor!";
        mIsValid = false;
    }
}

Polynomial::Polynomial (const Polynomial &other)
    : QList<int> (other)
{
    mIsValid = other.isValid();
}

int Polynomial::degree() const
{
    int size = this->size();
    return (size == 0)? 0 : size - 1;
}

void Polynomial::removeZeros ()
{
    // Сносим нули в старших коэффициентах.
    while (this->last() == 0 && this->size() > 1)
        this->removeLast();
}

const Polynomial Polynomial::operator + (const Polynomial &summand) const
{
    int minDegree;// Минимальная степень многочлена.
    bool isLess;// True, если степень нашего многочлена меньше слагаемого.

    // Находим их значения.
    if (this->size() < summand.size()){
        minDegree = this->size();
        isLess = true;
    }
    else{
        minDegree = summand.size();
        isLess = false;
    }

    // Счётчик для почленного сложения.
    int i = 0;
    // Результат операции.
    /// Может быть косяк с памятью, ибо возвращаем локальную переменную.
    Polynomial result (*this);

    for (; i < minDegree; ++ i) // Складываем до минимальной степени.
        result[i] += summand.at(i);
    // Если оказалось, что степень нашего многочлена меньше степени слагаемого.
    if (isLess)
        for (; i < summand.size(); ++ i)
            result << summand.at(i);
    // При сложении могли образоваться старшие нули. Трём.
    result.removeZeros();

    return result;
}

const Polynomial Polynomial::operator - (const Polynomial &subtrahend) const
{
    // Вычитание = сложение с обратным. Так что инвертируем знаки многочлена.
    Polynomial negateInput (subtrahend.size() - 1);

    for (int i = 0; i < subtrahend.size(); ++ i)
        negateInput[i] = -subtrahend.at(i);

    Polynomial result = (*this) + negateInput;

    return result;
}

const Polynomial Polynomial::operator * (const Polynomial &multiplier) const
{
    // Если умножаем на нулевой многочлен, то результат - нулевой многочлен.
    if (multiplier.last() == 0)
        return Polynomial();
    else{
        // Степень результирующего многочлена.
        int maxDegree = (this->size() - 1) + (multiplier.size() - 1);
        Polynomial result (maxDegree);

        for (int i = 0; i < this->size(); ++ i)					//коэффициенты перемножаются и записываются
            for (int j = 0; j < multiplier.size(); ++ j)				//в член со степенью i+j
                result[i + j] += this->at(i) * multiplier.at(j);
        //Старший член не равен 0, значит, нет необходимости чистить.

        return result;
    }
}

const Polynomial Polynomial::operator * (int n) const
{
    // Если умножаем на ноль, то результат - нулевой многочлен.
    if (n == 0)
        return Polynomial();
    else{
        Polynomial result (*this);

        // Умножаем каждый коэффициент на число.
        for (int i = 0; i < result.size(); ++ i)
            result[i] *= n;

        return result;
    }
}

const Polynomial Polynomial::operator / (int n) const
{
    // Защита от дурака: на ноль делить нельзя.
    if (n == 0){
        qDebug() << "Polynomial::operator /: Dividing by 0!";
        Polynomial result;
        result.setIsValid(false);

        return result;
    }
    else{
        Polynomial result (*this);

        // Делим каждый коэффициент на число.
        for (int i = 0; i < result.size(); ++ i)
            result[i] /= n;

        return result;
    }
}

int Polynomial::value (int x)
{
    // Результат.
    int y;

    // Работаем мо схеме Горнера.
    if (this->size() > 1){
        // Степень равна числу членов - 1.
        int degree = this->size() - 1;

        y = this->at(degree - 1) + x * this->at(degree);
        for (int i = degree - 2; i >= 0; -- i)
            y = this->at(i) + x * y;
    }
    else
        y = this->at(0);

    return y;
}

void Polynomial::copy (const Polynomial &other)
{
    this->clear();
    this->append(other);
}

QString Polynomial::toString() const
{
    QString polyToText;


    if (this->mIsValid == false){
        polyToText = "Polynomial::print(): The polynomial is invalid!";

        return polyToText;
    }
    switch (this->size()) {
    case 0:
        polyToText = "Polynomial::print(): Degree must be at least 0!";
        break;
    case 1:
        polyToText += QString("%1").arg(this->at(0));
        break;
    case 2:
        switch (this->at(1)){
            case 1:
                polyToText += QString("x");
                break;
            case -1:
                polyToText += QString("-x");
                break;
            case 0:
                break;
            default:
                polyToText += QString("%1x").arg(this->at(1));
        }
        if (this->at(0) != 0){
            if (this->at(0) > 0)
                polyToText += QString("+%1").arg(this->at(0));
            else
                polyToText += QString("%1").arg(this->at(0));
        }
        break;
    default:
        // Особый вывод для старшего члена, т.к. если он положителен, то знак + не ставится
        switch (this->last()){
            case 1:
                polyToText += QString("x^%1").arg(this->size() - 1);
                break;
            case -1:
                polyToText += QString("-x^%1").arg(this->size() - 1);
                break;
            default:
                polyToText += QString("%1x^%2").arg(this->last()).arg (this->size() - 1);
        }
        for (int i = this->size() - 2; i > 1; -- i)
            switch (this->at(i)){
            case 1:
                polyToText += QString("+x^%1").arg(i);
                break;
            case -1:
                polyToText += QString("-x^%1").arg(i);
                break;
            case 0:
                break;
            default:
                if (this->at(i) > 0)
                    polyToText += QString("+%1x^%2").arg(this->at(i)).arg(i);
                else
                    polyToText += QString("%1x^%2").arg(this->at(i)).arg(i);
            }
        switch (this->at(1)){
            case 1:
                polyToText += QString("+x");
                break;
            case -1:
                polyToText += QString("-x");
                break;
            case 0:
                break;
            default:
                if (this->at(1) > 0)
                    polyToText += QString("+%1x").arg(this->at(1));
                else
                    polyToText += QString("%1x").arg(this->at(1));
        }
        if (this->at(0) != 0){
            if (this->at(0) > 0)
                polyToText += QString("+%1").arg(this->at(0));
            else
                polyToText += QString("%1").arg(this->at(0));
        }
        break;
    }

    return polyToText;
}

void Polynomial::divide (const Polynomial &divider, Polynomial &quotient, Polynomial &modulo) const
{
    //Отсев быстрых случаев:
    if (this->size() < divider.size()){	//отсев случая, когда deg (x)<deg divider(x)
        modulo.copy(*this);
        quotient = Polynomial();
    }
    else if (divider.last() == 0){   //если делитель - нулевой многочлен, то деление невозможно
        qDebug() << "Polynomial::divide(): Division by 0!";
        quotient.setIsValid(false);
        modulo.setIsValid(false);
    }
    else if (this->last() == 0){	//если делимое - нулевой многочлен, то частное и остаток равны 0
        quotient = Polynomial();
        modulo = Polynomial();
    }
    else{//Если не прошёл ни один из случаев:
        Polynomial newQuotient (this->size() - divider.size()); // Степень частного = степень делимого - степень делителя
        Polynomial newModulo (*this);// Сначала остаток равен делимому

        //Само деление:
        int i = newQuotient.size() - 1;// Начинаем со старшего члена делимого.
        int divDegree = divider.size() - 1;// Степень делителя.

        for (; newModulo.at(divDegree + i) % divider.at(divDegree) == 0 && i >= 0; -- i){
            newQuotient[i] = newModulo[divDegree + i] / divider[divDegree];// Находим i-тый член частного
            newModulo[divDegree + i] = 0;// Из первого члена делимого вычитаем его же (по смыслу деления) => для экономии времени = 0.
            for (int j = divDegree - 1; j >= 0; -- j)
                newModulo[i + j] -= newQuotient[i] * divider[j]; //Вычитаем из каждого члена делимого член частного * член делителя
            newModulo.removeLast();
        }
        for (; i >= 0; -- i)
            newQuotient[i] = 0;
        newModulo.removeZeros();

        modulo.copy(newModulo);
        quotient.copy(newQuotient);
    }
}

const Polynomial Polynomial::operator % (const Polynomial &divider) const
{
    Polynomial quotientTemp;// Не нужное, по сути, частное
    Polynomial moduloRes;// Нужный остаток
    this->divide (divider, quotientTemp, moduloRes);

    return moduloRes;
}

const Polynomial Polynomial::operator / (const Polynomial &divider) const
{
    Polynomial quotientRes;// Частное - результат.
    Polynomial moduloTemp;// Остаток не нужен.
    this->divide (divider, quotientRes, moduloTemp);

    return quotientRes;
}

bool Polynomial::operator == (const Polynomial &other) const
{
    if (this->size() == other.size()){
        int i = 0;

        for (; i < this->size() && this->at(i) == other.at(i); ++ i);
        if (i == this->size())
            return true;
        else
            return false;
    }
    else
        return false;
}

void Polynomial::setIsValid(bool isValid)
{
    mIsValid = isValid;
}

bool Polynomial::isValid() const
{
    return mIsValid;
}

bool Polynomial::operator < (const Polynomial &other) const
{
    if (this->size() > other.size())
        return false;
    else if (this->size() < other.size())
        return true;
    else{
        Polynomial Q = *this - other;

        if (Q.last() > 0)
            return false;
        else
            return true;
    }
}
