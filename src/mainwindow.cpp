#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hook.h"

MainWindow* mainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Hook::keycomb_t keycombListen = Hook::getKeyCombinationFor(Hook::TOGGLE_LISTEN);
    Hook::keycomb_t keycombMouse = Hook::getKeyCombinationFor(Hook::TOGGLE_MOUSE);
    Hook::keycomb_t keycombClick = Hook::getKeyCombinationFor(Hook::TOGGLE_CLICK);

    ui->listenBindButton->setText(Hook::getStringNameFor(keycombListen));
    ui->mouseBindButton->setText(Hook::getStringNameFor(keycombMouse));
    ui->clickBindButton->setText(Hook::getStringNameFor(keycombClick));
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
