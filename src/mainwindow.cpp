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
    refreshUiButtons();
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


void MainWindow::refreshUiButtons() {
    sac::hook::keycomb_t keycombListen = getKeyCombinationFor(sac::hook::TOGGLE_LISTEN);
    sac::hook::keycomb_t keycombMouse  = getKeyCombinationFor(sac::hook::TOGGLE_MOUSE);
    sac::hook::keycomb_t keycombClick  = getKeyCombinationFor(sac::hook::TOGGLE_CLICK);
    sac::hook::keycomb_t keycombHold   = getKeyCombinationFor(sac::hook::TOGGLE_HOLD);

    ui->listenBindButton     ->setText(getStringNameFor(keycombListen));
    ui->clickBindButton      ->setText(getStringNameFor(keycombClick ));
    ui->mouseBindButton      ->setText(getStringNameFor(keycombMouse ));
    ui->holdButtonMouseButton->setText(getStringNameFor(keycombHold  ));
}
