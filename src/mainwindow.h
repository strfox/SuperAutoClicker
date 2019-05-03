    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "aboutdialog.h"

namespace Ui {
    class MainWindow;
}

namespace sac {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void putMsg(QString msg);
    void putDebugMsg(QString msg);
    void refresh();

private slots:
    void on_actionAbout_triggered();
    void on_actionQuit_triggered();

    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;
    AboutDialog    *m_aboutDialog = nullptr;

    void refreshUiButtons();
    void refreshUiInfo();
}; // class MainWindow


extern MainWindow* _mainWindow; // Use mainWindow()
MainWindow* mainWindow();


} // namespace sac

#endif // MAINWINDOW_H
