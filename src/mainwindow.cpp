#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hook.h"

#include <QDebug>

MainWindow* mainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    keycomb_t keycombListen = getKeyCombinationFor(TOGGLE_LISTEN);
    keycomb_t keycombMouse = getKeyCombinationFor(TOGGLE_MOUSE);
    keycomb_t keycombClick = getKeyCombinationFor(TOGGLE_CLICK);

    ui->listenBindButton->setText(getStringNameFor(keycombListen));
    ui->clickBindButton->setText(getStringNameFor(keycombClick));
    ui->mouseBindButton->setText(getStringNameFor(keycombMouse));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::putMsg(QString msg) {
    ui->messages->append(msg);
}


void MainWindow::putDebugMsg(QString msg) {
#ifdef QT_DEBUG
    putMsg(msg);
#endif
}
