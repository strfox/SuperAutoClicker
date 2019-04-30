#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hook.h"
#include "autoclicker.h"

#include <QDebug>
#include <QMessageBox>

namespace sac {

MainWindow* mainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refresh();
}

MainWindow::~MainWindow(){
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


void MainWindow::refresh() {
    kb::keycomb_t keycombListen = getKeyCombinationFor(sac::hook::TOGGLE_LISTEN);
    kb::keycomb_t keycombMouse  = getKeyCombinationFor(sac::hook::TOGGLE_MOUSE );
    kb::keycomb_t keycombClick  = getKeyCombinationFor(sac::hook::TOGGLE_CLICK );
    kb::keycomb_t keycombHold   = getKeyCombinationFor(sac::hook::TOGGLE_HOLD  );

    ui->listenBindButton     ->setText(getStringNameFor(keycombListen));
    ui->clickBindButton      ->setText(getStringNameFor(keycombClick ));
    ui->mouseBindButton      ->setText(getStringNameFor(keycombMouse ));
    ui->holdButtonMouseButton->setText(getStringNameFor(keycombHold  ));

    assert(autoClicker != nullptr);

    ui->listenModeStatusLabel    ->setText(autoClicker->m_listenMode     ? "ON"     : "OFF"   );
    ui->mouseButtonStatusLabel   ->setText(autoClicker->m_mouseButton    ? "MOUSE2" : "MOUSE1");
    ui->clickModeStatusLabel     ->setText(autoClicker->m_clickMode      ? "ON"     : "OFF"   );
    ui->holdButtonModeStatusLabel->setText(autoClicker->m_holdButtonMode ? "ON"     : "OFF"   );
}

void MainWindow::on_actionAbout_triggered(){
    static QString msg;
    if (msg.isEmpty()) {
        QTextStream s(&msg);
        s << PROGRAM_NAME
          << " version " << PROGRAM_VERSION
          << " by " << PROGRAM_AUTHOR
          << ". GitHub: " << PROGRAM_REPO;
    }

    QMessageBox msgbox;
    msgbox.setWindowTitle("About");
    msgbox.setText(msg);
    msgbox.exec();
}

void MainWindow::on_actionQuit_triggered(){
    QApplication::quit();
}

} //namespace sac
