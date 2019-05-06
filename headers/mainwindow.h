#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "aboutdialog.h"
#include "types.h"

namespace Ui {
class MainWindow;
}

namespace sac {

class MainWindow : public QMainWindow {
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
  void on_slowClickCheckBox_stateChanged(int arg1);

  void on_listenBindButton_pressed(); // v2.0.3
  void on_clickBindButton_pressed();  // v2.0.3
  void on_mouseBindButton_pressed();  // v2.0.3

private:
  Ui::MainWindow *ui;
  AboutDialog *m_aboutDialog = nullptr;

  /**
   * v2.0.3: Sets the program into binding mode for an action.
   */
  void changeInput(QPushButton *btn, action_t whichAction);
}; // class 1500MainWindow

extern MainWindow *_mainWindow; // Use mainWindow()
MainWindow *mainWindow();

} // namespace sac

#endif // MAINWINDOW_H
