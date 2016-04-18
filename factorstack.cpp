#include "factorstack.h"
#include <QDebug>
#include <QtMath>

#undef VERBOSE

FactorStack::FactorStack() : QStack()
{
}

FactorStack::FactorStack(Polynomial P) : QStack()
{
    this->push(P);
}


void FactorStack::setPolynomial(Polynomial P)
{
    this->clear();
    this->push(P);
}

bool FactorStack::checkValueCombinations(const ValueTable &table)
{
    QBitArray comb (table.size()); // Массив с двоичным числом. 1 = берём значение, 0 = не берём
    int counter = 0; // Cчётчик числа взятых в набор элементов

    mVariants = qPow(2, table.size());
    mVariantsDone = 0;
    do{
        if ((counter - 1 <= (table.size() - 1) / 2) && counter >= 2){// В наборе должно быть не больше Q.deg/2+1 элементов
                                                                //(по алгоритму Кронекера), но больше 1 (т.к. хотя бы линейный)
            ValueTable tempTable;// Таблица только с теми значениями, которые берём. (т.е. в номерах которых стоит 1)

            for (int i = 0 ; i < comb.size(); ++ i)
                if (comb.testBit(i) == true)
                    tempTable << table.at(i);
#ifdef VERBOSE
            qDebug() << "checkValueCombinations::tempTable " << tempTable;
#endif
            bool isDivisorFound = checkDivisorsCombinations(DivisorsTable(tempTable));// Был ли найден новый делитель?
            if (isDivisorFound)// Если да, говорим об этом.
                return true;
        }
        mVariantsDone ++;
        int percent = ((double)mVariantsDone) / mVariants * 100;

        emit progressDone(percent);
        if (comb.testBit(0) == false) {// Если младший бит 0 - делаем 1.
            comb.setBit(0, true);
            counter ++;
        }
        else{// Иначе прибавляем к числу 1.
            int i = 0;

            for (; (i < comb.size() - 1) && (comb.testBit(i) == true); ++ i){
                comb.setBit(i, false);
                counter --;
            }
            comb.setBit(i, true);
            counter ++;
        }
    } while (counter < comb.size());

    return false;// Если делитель так и не был найден.
}

bool FactorStack::checkDivisorsCombinations(const DivisorsTable &table)
{
    QVector<int> divNumbers (table.size());// Список номеров делителей, которые войдут в новую таблицу.
    // Чтобы рассмотреть все возможные комбинации делителей, рассматриваем "число" на каждой позиции которого
    //может стоять число, не большее числа элементов в выбранном множестве делителей.
    do{
        ValueTable newTable;// Таблица значений, сформированная из делителей.

        for (int i = 0; i < table.size(); ++ i){
            QPair<int, int> pairToAdd;// Пара значений, которую добавим в новую таблицу.

            pairToAdd.first = table.at(i).first;// Значение x берём с тем же номером.
            pairToAdd.second = table.at(i).second.at(divNumbers.at(i));// Значение y берём с номером, который в divNumbers
            newTable << pairToAdd;// Добавляем пару в таблицу.
        }
#ifdef VERBOSE
        qDebug() << "checkDivisorsCombinations::newTable: " << newTable;
#endif
        bool isDivisorFound = checkMinusLocation(newTable);// Был ли найден новый делитель?

        if (isDivisorFound)// Если да, говорим об этом.
            return true;
        divNumbers[0] ++;
        for (int i = 0; (i < divNumbers.size() - 1) && (divNumbers[i] == table.at(i).second.size()); ++ i){
            divNumbers[i] = 0;
            divNumbers[i + 1] ++;
        }
    } while (divNumbers.last() != table.last().second.size());

    return false;// Если делитель так и не был найден.
}

bool FactorStack::checkMinusLocation(const ValueTable &table)
{
    QBitArray minus (table.size()); // Массив с двоичным числом. 1 = берём значение с минусом, 0 = без минуса

    do{
        ValueTable tempTable;// Новая таблица

        for (int i = 0 ; i < minus.size(); ++ i){
            QPair<int, int> pairToAdd;

            pairToAdd.first = table.at(i).first; // У x знак не меняем.
            if (minus.testBit(i) == true)// Если бит равен 1, то берём с минусом.
                pairToAdd.second = -table.at(i).second;
            else// Иначе - с плюсом
                pairToAdd.second = table.at(i).second;
            tempTable << pairToAdd;// Добавляем новую пару значений.
        }
#ifdef VERBOSE
        qDebug() << "checkMinusLocation::tempTable: " << tempTable;
#endif
        bool isDivisorFound = checkFactor(tempTable);// Был ли найден новый делитель?

        if (isDivisorFound)// Если да, говорим об этом.
            return true;
        if (minus.testBit(0) == false)// Если младший бит 0 - делаем 1.
            minus.setBit(0, true);
        else{// Иначе прибавляем к числу 1.
            int i = 0;

            for (; (i < minus.size() - 1) && (minus.testBit(i) == true); ++ i)
                minus.setBit(i, false);
            minus.setBit(i, true);
        }
    } while (minus.testBit(minus.size()-1) != true);

    return false;// Если делитель так и не был найден.
}

int FactorStack::findGCD(const Polynomial &P)
{
    int i = 0;
    for (; i < P.size() && P.at(i) == 0; ++ i);

    int firstNotNull = P.at(i);// Первый ненулевой коэффициент
    Divisors divisors (firstNotNull);// Делители первого ненулевого члена.
    int GCD = 1;// Наибольший общий делитель всех коэффициентов.

    for (int i = 0; i < divisors.size(); ++ i) {
        int currentDivisor = divisors.at(i);// Делитель, который проверяем в данный момент.

        if (currentDivisor > GCD){
            int j = 1;

            for (; (P.at(j) % currentDivisor == 0) && (j < P.size()); ++ j);
            if (j == P.size())
                GCD = currentDivisor;
        }
    }

    return GCD;
}

void FactorStack::removeTopRoots()
{
    if (this->top().degree() == 1)// Если обрабатываем линейный многочлен
        emit factorFound(this->pop());// Выкидываем его и передаём как множитель.
    else{
        Polynomial Q = this->pop();// Оставшийся многочлен
        ValueTable table(Q);// Строим таблицу значений верхнего многочлена
#ifdef VERBOSE
        qDebug() << "RemoveTopRoots::valueTable:  " << table;
#endif
        for (int i = 0; i < table.size(); ++ i)
        {
            if (table.at(i).second == 0)
            {
#ifdef VERBOSE
                qDebug() << "RemoveTopRoots::root found:  " << table.at(i);
#endif
                // Если нашли корень, строим линейный множитель.
                Polynomial P (1);

                P[0] = -table.at(i).first;
                P[1] = 1;

                // Остаток от деления.
                Polynomial modulo, quotient;
                Q.divide(P, quotient, modulo); // Делим на P
                while (modulo.last() == 0 )
                {
                    emit factorFound(P);// Отправляем линейный множитель.
                    if (quotient.degree() != 0){// Если при делении получилась не константа (могла получиться только +-1)
                        if (quotient.degree() == 1){// Если получен линейный множитель, отправляем его.
                            emit factorFound(quotient);
                            return;
                        }
                        else
                            Q.copy(quotient);
                            Q.divide(P, quotient, modulo);
                    }
                    else
                    {
                        emit constantFound(Q.at(0));// Если нашли константу
                        return;
                    }
                }
            }  
        }
        // Polynomial we get at the end
        this->push(Q);
    }
}

void FactorStack::factorize()
{
    if (this->isEmpty()){
        qDebug() << "FactorStack::factorize(): Error: the stack is empty!";
        return;
    }
    else if (this->top().degree() == 0){//Если дана константа, просто её выкидываем.
        emit constantFound(this->pop()[0]);
        emit factorized();
    }
    else{
        int GCD = findGCD(this->top());

        if (GCD > 1) {
            Polynomial Q = this->pop() / GCD;// Новый примитивный полином.

            this->push(Q);// Заменяем исходный многочлен на примитивный.

            emit constantFound(GCD);// Сигналим о том, что нашли множитель-число
        }
        while (!this->isEmpty())// Пока стэк не опустеет, будем искать множители.
            manageTop();// Обрабатываем верхний полином.

        emit factorized();// Сигналим о том, что разложение закончено.
    }
}

void FactorStack::manageTop()
{
    removeTopRoots();// Подготавливаем к нормальному поиску множителей - удаляем линейные.
    if (this->isEmpty())
        return;

    ValueTable table (this->top());// Строим таблицу значений верхнего полинома. В ней уже нет нулей.

    bool isTopFactorable = checkValueCombinations(table);// Приводим ли верхний многочлен?

    emit progressDone(100);
    if (!isTopFactorable)// Если он неприводим, передаём его.
        emit factorFound(this->pop());
}

bool FactorStack::checkFactor(const ValueTable &table)
{
    Polynomial P = table.interpolate();// Строим интерполяционный полином по таблице.

    if (P.isValid() && (P.size() > 1)){// Если полином корректен и его степень хотя бы 1.
        Polynomial modulo;// Остаток от деления верхнего полинома на возможный делитель.
        Polynomial result;// Частное от деления.

        this->top().divide(P, result, modulo);
        if (modulo.last() == 0){// Если поделилось, то найден новый делитель.
            this->pop();// Выкидываем верхний многочлен.
            this->push(result);// Запихиваем частное.
            emit factorFound(P);// Сигналим, что нашли новый делитель.

            return true;// Делитель найден.
        }
    }

    return false;// Если полинома - не делитель
}

void FactorStack::print() const
{
    qDebug() << "FactorStack::print()";
    for (int i = 0; i < this->size(); ++ i)
        qDebug() << this->at(i).toString();
}
