#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    sqliteEngine = new SqliteEngine();
    //sqliteEngine->initDB();
}

MainWindow::~MainWindow() {
    delete sqliteEngine;
    delete ui;
}

void MainWindow::insert() {
    sqliteEngine->insert();
}

void MainWindow::query() {
    sqliteEngine->query();
}
