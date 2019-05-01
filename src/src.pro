QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = sac
TEMPLATE = app

include(../sac.pri)

SOURCES += \
        winbeep.cpp \
        beep.cpp \
        main.cpp \
        mainwindow.cpp \
        winautoclicker.cpp \
        winhook.cpp \
        winkeyboard.cpp

HEADERS += \
        autoclicker.h \
        beep.h \
        hook.h \
        keyboard.h \
        mainwindow.h \
        winkeys.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += $$PWD/../include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
