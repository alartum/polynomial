#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Some magic here: we don't want to get errors because of threading
    qRegisterMetaType<Polynomial>("Polynomial");

    this->setFixedSize(360, 430);
    ui->setupUi(this);
    ui->Coef->horizontalHeader()->setVisible(true);
    ui->progressBar->setValue(0);
    connect (&mFactorsStack, SIGNAL (progressDone(int)), ui->progressBar, SLOT (setValue(int)));
    connect (ui->Degree, SIGNAL (valueChanged(int)),
             this, SLOT (buildTable(int)));
    this->buildTable(1);
    connect (ui->Factorize, SIGNAL (pressed ()),
             this, SLOT (factorizePressed()));

    mFactorsStack.moveToThread(&mFactorThread);
    connect (&mFactorsStack, SIGNAL(constantFound(int)), &mFactorsMap, SLOT (addConstant(int)));
    connect (&mFactorsStack, SIGNAL(factorFound(Polynomial)), &mFactorsMap, SLOT(addPolynomial(Polynomial)));
    connect (&mFactorsStack, SIGNAL(factorized()), this, SLOT (printResult()));
    connect (&mFactorsStack, SIGNAL(factorized()), &mFactorThread, SLOT(quit()));
    connect (&mFactorThread, SIGNAL(started()), &mFactorsStack, SLOT (factorize()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow :: buildTable(int number)
{
    ui->Coef->setColumnCount(number + 1);
    for (int i = 0; i <= number; i ++)
    {
        ui->Coef->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("x^%1").arg(number - i)));
        ui->Coef->setItem (0, i, new QTableWidgetItem (QString ("0")));
        ui->Coef->item (0, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}
void MainWindow::on_actionAbout_triggered()
{
    QWidget *test = new QWidget ();
    test->setWindowTitle("О программе");
    test->setFixedSize(300, 100);
    QTextBrowser *info = new QTextBrowser (test);
    info->setGeometry(0,0,300,100);
    info->setFontPointSize(9);
    info->setText ("Программа предназначена для факторизации многочленов "
                   "с целыми коэффициентами над полем целых чисел. Для "
                   "выполнения этой задачи был использован алгоритм"
                   " Кронекера. Данная программа быстро исполняется при небольших "
                   "степенях и коэффициентах полиномов.");
    info->setAlignment(Qt::AlignJustify);
    test->show();
}
void MainWindow::on_actionSave_triggered()
{
    if (ui->Result->toPlainText() != "")
    {
        QFileDialog *test = new QFileDialog(this, "Save", QApplication::applicationDirPath(), "Текстовый файл (*.txt)");
        test->setAcceptMode(QFileDialog::AcceptSave);
        test->setDefaultSuffix("txt");
        test->show();
        connect(test, SIGNAL(fileSelected(QString)), this, SLOT(saveFile(QString)));
    }
    else
    {
        QMessageBox::warning(this, "Ошибка",
                             "Нет информации для записи.\n"
                             "Сначала необходимо раложить полином.",
                             QMessageBox::Ok | QMessageBox::Escape);
        return;
    }
}
void MainWindow::saveFile(QString filePath)
{
    QFile * file = new QFile(filePath);
    file->open(QIODevice::Append | QIODevice::Text);
    QTextStream out(file);
    out << ui->Result->toPlainText();
    out.flush();
    file->close();
}
void MainWindow :: factorizePressed ()
{
    mFactorThread.exit();
    ui->Result->clear();
    int degree = ui->Degree->value();
    mFactorsMap.reset();

    Polynomial P(degree);

    for (int i = 0; i < P.size(); ++ i)
    {
        if (ui->Coef->item(0, degree - i)->text() == "")
        {
            QMessageBox::warning(this, "Ошибка",
                                 "В таблице не должно быть пустых ячеек.",
                                 QMessageBox::Ok | QMessageBox::Escape);
            return;
        }
        bool isOk;
        P[i] = ui->Coef->item(0, degree - i)->text().toInt(&isOk);
        if (isOk == false){
            QMessageBox::warning(this, "Ошибка",
                                 "В ячейках таблицы должны быть только цифры.",
                                 QMessageBox::Ok | QMessageBox::Escape);
            return;
        }
    }
    P.removeZeros();

    mOutput = P.toString();
    ui->Result->setText(mOutput);
    ui->Result->update();

    mFactorsStack.setPolynomial(P);
    mFactorThread.start();
}
void MainWindow::on_actionClear_triggered()
{
    for (int i = 0; i <= ui->Degree->value(); i ++)
        ui->Coef->item(0, i)->setText("0");
    ui->Result->clear();
}
void MainWindow::on_actionExit_triggered()
{
    this->close();
}
void MainWindow::on_actionOpen_help_triggered()
{
    QWidget *test = new QWidget ();
    test->setWindowTitle("Помощь");
    test->setFixedSize(300, 220);
    QTextBrowser *info = new QTextBrowser (test);
    info->setGeometry(0,0,300,220);
    info->setFontPointSize(9);
    info->setText ("1. Выберете степень многочлена путём ввода данных или прокрутки\n"
                   "2. Введите коэффициенты, стоящие при переменной x, в соответствующие клетки таблицы. "
                   "Пустая ячейка считается ячейкой, в которой стоит 0.\n"
                   "3. Нажмите кнопку \"Факторизовать\" для разложения введённого многочлена на множители.\n"
                   "4. При необходимости сохраните результат, нажав кнопку \"Сохранить\" в меню \"Файл\" "
                   "и выбрав файл для сохранения.\n"
                   "5. Очистить таблицу можно с помощью кнопки \"Очистить\" в меню \"Файл\".\n");
    info->setAlignment(Qt::AlignJustify);
    test->show();
}

void MainWindow::printResult()
{
    qDebug()<<mFactorsMap;
    mOutput += mFactorsMap.toString();
    ui->Result->setText(mOutput);
}
