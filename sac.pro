QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = sac
TEMPLATE = app

include(./sac.pri)

HDIR = $$PWD/headers/
SDIR = $$PWD/source/
UIDIR = $$PWD/ui

HDIRWIN32 = $$HDIR/win32
SDIRWIN32 = $$SDIR/win32

HDIRUNIX = $$HDIR/unix
SDIRUNIX = $$SDIR/unix

INCLUDEPATH += $$HDIR $$PWD/include

HEADERS += \
        $$HDIR/aboutdialog.h \
        $$HDIR/autoclicker.h \
        $$HDIR/beep.h \
        $$HDIR/hook.h \
        $$HDIR/keyboard.h \
        $$HDIR/mainwindow.h \
        $$HDIR/types.h \
        $$HDIR/util.h

SOURCES += \
        $$SDIR/aboutdialog.cpp \
        $$SDIR/util.cpp \
        $$SDIR/main.cpp \
        $$SDIR/mainwindow.cpp \
        $$SDIR/beep.cpp \
        $$SDIR/autoclicker.cpp

win32 {
    LIBS += -luser32

    HEADERS += \
            $$HDIRWIN32/winkeys.h

    SOURCES += \
            $$SDIRWIN32/autoclicker.cpp \
            $$SDIRWIN32/beep.cpp \
            $$SDIRWIN32/hook.cpp \
            $$SDIRWIN32/keyboard.cpp
}

unix {
    SOURCES += \
            $$SDIRUNIX/autoclicker.cpp \
            $$SDIRUNIX/beep.cpp \
            $$SDIRUNIX/hook.cpp \
            $$SDIRUNIX/keyboard.cpp
}

FORMS += \
        $$UIDIR/aboutdialog.ui \
        $$UIDIR/mainwindow.ui

RESOURCES += \
    $$PWD/images.qrc

RC_FILE = $$PWD/sac.rc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


