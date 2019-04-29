#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> 

namespace Ui {
    class MainWindow;
}

namespace sac {

class MainWindow; //fwddecl

extern MainWindow* mainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void putMsg(QString msg);
    void putDebugMsg(QString msg);

private:
    Ui::MainWindow *ui;

    void refreshUiButtons();
}; // class MainWindow

} // namespace sac

#endif // MAINWINDOW_H
