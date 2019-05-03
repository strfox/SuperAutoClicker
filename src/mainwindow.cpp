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

MainWindow *_mainWindow;
MainWindow* mainWindow() {
    assert(_mainWindow != nullptr);
    return _mainWindow;
}

}



sac::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refresh();
}

sac::MainWindow::~MainWindow(){
    delete ui;
}


void sac::MainWindow::putMsg(QString msg) {
    ui->messages->append(msg);
}


void sac::MainWindow::putDebugMsg(QString msg) {
    Q_UNUSED(msg);
#ifdef QT_DEBUG
    putMsg(msg);
#endif
}


void sac::MainWindow::refresh() {
    kb::keycomb_t keycombListen = hook::getKeyCombinationFor(TOGGLE_LISTEN);
    kb::keycomb_t keycombMouse  = hook::getKeyCombinationFor(TOGGLE_MOUSE );
    kb::keycomb_t keycombClick  = hook::getKeyCombinationFor(TOGGLE_CLICK );
    // kb::keycomb_t keycombHold   = hook::getKeyCombinationFor(TOGGLE_HOLD  );

    ui->listenBindButton     ->setText(keycombstr(keycombListen));
    ui->clickBindButton      ->setText(keycombstr(keycombClick));
    ui->mouseBindButton      ->setText(keycombstr(keycombMouse));

    AutoClicker *_ac = autoClicker();

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


void sac::MainWindow::on_actionAbout_triggered(){
    if (m_aboutDialog == nullptr) {
        m_aboutDialog = new AboutDialog(this);
    }
    m_aboutDialog->show();
}


void sac::MainWindow::on_actionQuit_triggered(){
    QApplication::quit();
}


void sac::MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl(PROGRAM_REPO));
}


void sac::MainWindow::on_slowClickCheckBox_stateChanged(int state)
{
    AutoClicker *_ac = autoClicker();
    switch (state) {
    case Qt::Unchecked:
        _ac->m_slowClickMode = false;
        break;
    case Qt::Checked:
        _ac->m_slowClickMode = true;
        break;
    case Qt::PartiallyChecked:
        // fall-through
    default:
        assert(false);
        break;
    }

    putMsg(QString("Slow Click Mode: ").append(_ac->m_slowClickMode ? "ON" : "OFF"));
}
