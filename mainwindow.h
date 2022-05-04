#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqliteengine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initDB();


private slots:
    void insert();
    void query();
private:
    Ui::MainWindow *ui;
    SqliteEngine * sqliteEngine;
};
#endif // MAINWINDOW_H
