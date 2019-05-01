#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hook.h"
#include "autoclicker.h"

#include <QDebug>
#include <QMessageBox>

namespace sac {


MainWindow* _mainWindow;

MainWindow* mainWindow() {
    assert(_mainWindow != nullptr);
    return _mainWindow;
}


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

    ui->listenBindButton     ->setText(keycombstr(keycombListen));
    ui->clickBindButton      ->setText(keycombstr(keycombClick ));
    ui->mouseBindButton      ->setText(keycombstr(keycombMouse ));
    ui->holdButtonMouseButton->setText(keycombstr(keycombHold  ));

    AutoClicker* _ac = autoClicker();

    ui->listenModeStatusLabel    ->setText(_ac->m_listenMode     ? "ON"     : "OFF"   );
    ui->mouseButtonStatusLabel   ->setText(_ac->m_mouseButton    ? "MOUSE2" : "MOUSE1");
    ui->clickModeStatusLabel     ->setText(_ac->m_clickMode      ? "ON"     : "OFF"   );
    ui->holdButtonModeStatusLabel->setText(_ac->m_holdButtonMode ? "ON"     : "OFF"   );

    if (_ac->m_listenMode) {
        ui->millisecondsStatusLabel->setNum(static_cast<int>(_ac->m_msInput));
    }
    else {
        int msInterval = static_cast<int>(_ac->m_msInterval);
        assert(msInterval >= 0);
        ui->millisecondsStatusLabel->setText(
                     msInterval > 0 ? QString::number(msInterval) : QString("N/A")
                    );
    }

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
