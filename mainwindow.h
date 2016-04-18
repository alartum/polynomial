#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "factorsmap.h"
#include "factorstack.h"
#include <QString>
#include <QThread>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FactorsMap mFactorsMap;
    FactorStack mFactorsStack;
    QString mOutput;
    QThread mFactorThread;
public slots:
    void buildTable (int number);
    void saveFile (QString filePath);
    void printResult();
private slots:
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void factorizePressed ();
    void on_actionClear_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_help_triggered();
};

#endif // MAINWINDOW_H
