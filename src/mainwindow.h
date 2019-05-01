    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> 

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

private:
    Ui::MainWindow *ui;

    void refreshUiButtons();
    void refreshUiInfo();
}; // class MainWindow


extern MainWindow* _mainWindow; // Use mainWindow()

MainWindow* mainWindow();


} // namespace sac

#endif // MAINWINDOW_H
