#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* mainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::putMessage(QString msg) {
    ui->messages->append(msg);
}
