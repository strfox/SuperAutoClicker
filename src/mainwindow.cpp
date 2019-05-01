#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hook.h"
#include "autoclicker.h"
#include "types.h"
#include "aboutdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>

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
    kb::keycomb_t keycombListen = hook::getKeyCombinationFor(TOGGLE_LISTEN);
    kb::keycomb_t keycombMouse  = hook::getKeyCombinationFor(TOGGLE_MOUSE );
    kb::keycomb_t keycombClick  = hook::getKeyCombinationFor(TOGGLE_CLICK );
    // kb::keycomb_t keycombHold   = hook::getKeyCombinationFor(TOGGLE_HOLD  );

    ui->listenBindButton     ->setText(keycombstr(keycombListen));
    ui->clickBindButton      ->setText(keycombstr(keycombClick));
    ui->mouseBindButton      ->setText(keycombstr(keycombMouse));

    AutoClicker* _ac = autoClicker();

    ui->listenModeStatusLabel->setText(_ac->m_listenMode ? "ON" : "OFF" );
    switch (_ac->m_mouseButton) {
    case MOUSE1:
        ui->mouseButtonStatusLabel->setText("MOUSE1");
        break;
    case MOUSE2:
        ui->mouseButtonStatusLabel->setText("MOUSE2");
        break;
    }
    ui->clickModeStatusLabel->setText(_ac->m_clickMode ? "ON" : "OFF" );

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
    AboutDialog* about = new AboutDialog(this);
    about->show();
}


void MainWindow::on_actionQuit_triggered(){
    QApplication::quit();
}


void MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl(PROGRAM_REPO));
}

} //namespace sac

