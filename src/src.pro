QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = sac
TEMPLATE = app

include(../sac.pri)

SOURCES += \
        aboutdialog.cpp \
        util.cpp \
        winbeep.cpp \
        beep.cpp \
        main.cpp \
        mainwindow.cpp \
        winautoclicker.cpp \
        winhook.cpp \
        winkeyboard.cpp

HEADERS += \
        aboutdialog.h \
        autoclicker.h \
        beep.h \
        hook.h \
        keyboard.h \
        mainwindow.h \
        types.h \
        util.h \
        winkeys.h

FORMS += \
        aboutdialog.ui \
        mainwindow.ui

RESOURCES += \
    images.qrc

INCLUDEPATH += $$PWD/../include

RC_FILE = $$PWD/../sac.rc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


