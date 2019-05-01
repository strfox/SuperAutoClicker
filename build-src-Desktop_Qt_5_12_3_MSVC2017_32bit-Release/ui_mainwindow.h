/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionQuit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *infoGroupBox;
    QFormLayout *formLayout_2;
    QLabel *modeLabel;
    QLabel *listenModeStatusLabel;
    QLabel *label;
    QLabel *clickModeStatusLabel;
    QLabel *label_3;
    QLabel *mouseButtonStatusLabel;
    QLabel *label_8;
    QLabel *millisecondsStatusLabel;
    QGroupBox *keysGroupBox;
    QFormLayout *formLayout;
    QLabel *toggleListenModeLabel;
    QPushButton *listenBindButton;
    QLabel *toggleClickModeLabel;
    QPushButton *clickBindButton;
    QLabel *toggleMouseButtonLabel;
    QPushButton *mouseBindButton;
    QTextBrowser *messages;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(313, 531);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        infoGroupBox = new QGroupBox(centralWidget);
        infoGroupBox->setObjectName(QString::fromUtf8("infoGroupBox"));
        formLayout_2 = new QFormLayout(infoGroupBox);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        modeLabel = new QLabel(infoGroupBox);
        modeLabel->setObjectName(QString::fromUtf8("modeLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, modeLabel);

        listenModeStatusLabel = new QLabel(infoGroupBox);
        listenModeStatusLabel->setObjectName(QString::fromUtf8("listenModeStatusLabel"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, listenModeStatusLabel);

        label = new QLabel(infoGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label);

        clickModeStatusLabel = new QLabel(infoGroupBox);
        clickModeStatusLabel->setObjectName(QString::fromUtf8("clickModeStatusLabel"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, clickModeStatusLabel);

        label_3 = new QLabel(infoGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        mouseButtonStatusLabel = new QLabel(infoGroupBox);
        mouseButtonStatusLabel->setObjectName(QString::fromUtf8("mouseButtonStatusLabel"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, mouseButtonStatusLabel);

        label_8 = new QLabel(infoGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_8);

        millisecondsStatusLabel = new QLabel(infoGroupBox);
        millisecondsStatusLabel->setObjectName(QString::fromUtf8("millisecondsStatusLabel"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, millisecondsStatusLabel);


        gridLayout->addWidget(infoGroupBox, 1, 0, 1, 1);

        keysGroupBox = new QGroupBox(centralWidget);
        keysGroupBox->setObjectName(QString::fromUtf8("keysGroupBox"));
        formLayout = new QFormLayout(keysGroupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        toggleListenModeLabel = new QLabel(keysGroupBox);
        toggleListenModeLabel->setObjectName(QString::fromUtf8("toggleListenModeLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, toggleListenModeLabel);

        listenBindButton = new QPushButton(keysGroupBox);
        listenBindButton->setObjectName(QString::fromUtf8("listenBindButton"));

        formLayout->setWidget(0, QFormLayout::FieldRole, listenBindButton);

        toggleClickModeLabel = new QLabel(keysGroupBox);
        toggleClickModeLabel->setObjectName(QString::fromUtf8("toggleClickModeLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, toggleClickModeLabel);

        clickBindButton = new QPushButton(keysGroupBox);
        clickBindButton->setObjectName(QString::fromUtf8("clickBindButton"));

        formLayout->setWidget(1, QFormLayout::FieldRole, clickBindButton);

        toggleMouseButtonLabel = new QLabel(keysGroupBox);
        toggleMouseButtonLabel->setObjectName(QString::fromUtf8("toggleMouseButtonLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, toggleMouseButtonLabel);

        mouseBindButton = new QPushButton(keysGroupBox);
        mouseBindButton->setObjectName(QString::fromUtf8("mouseBindButton"));

        formLayout->setWidget(2, QFormLayout::FieldRole, mouseBindButton);


        gridLayout->addWidget(keysGroupBox, 0, 0, 1, 1);

        messages = new QTextBrowser(centralWidget);
        messages->setObjectName(QString::fromUtf8("messages"));

        gridLayout->addWidget(messages, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 313, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionAbout);
        mainToolBar->addAction(actionHelp);
        mainToolBar->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SuperAutoClicker 2", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "About", nullptr));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        infoGroupBox->setTitle(QApplication::translate("MainWindow", "Info", nullptr));
        modeLabel->setText(QApplication::translate("MainWindow", "Listen:", nullptr));
        listenModeStatusLabel->setText(QApplication::translate("MainWindow", "?", nullptr));
        label->setText(QApplication::translate("MainWindow", "Click:", nullptr));
        clickModeStatusLabel->setText(QApplication::translate("MainWindow", "?", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "MOUSEBTN:", nullptr));
        mouseButtonStatusLabel->setText(QApplication::translate("MainWindow", "?", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Ms:", nullptr));
        millisecondsStatusLabel->setText(QApplication::translate("MainWindow", "Not Set", nullptr));
        keysGroupBox->setTitle(QApplication::translate("MainWindow", "Keys", nullptr));
        toggleListenModeLabel->setText(QApplication::translate("MainWindow", "[T] Listen Mode", nullptr));
        listenBindButton->setText(QApplication::translate("MainWindow", "?", nullptr));
        toggleClickModeLabel->setText(QApplication::translate("MainWindow", "[T] Click Mode", nullptr));
        clickBindButton->setText(QApplication::translate("MainWindow", "?", nullptr));
        toggleMouseButtonLabel->setText(QApplication::translate("MainWindow", "[T] MOUSE1/MOUSE2", nullptr));
        mouseBindButton->setText(QApplication::translate("MainWindow", "?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
