QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = sac
TEMPLATE = app

include(./sac.pri)

HDIR = $$PWD/headers/
SDIR = $$PWD/source/
UIDIR = $$PWD/ui

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
        $$SDIR/autoclicker.cpp \
        $$SDIR/keyboard.cpp

win32 {
    LIBS += -luser32

    HEADERS += \
            $$HDIR/win_keys.h

    SOURCES += \
            $$SDIR/win_autoclicker.cpp \
            $$SDIR/win_beep.cpp \
            $$SDIR/win_hook.cpp \
            $$SDIR/win_keyboard.cpp
}

unix {
    LIBS += -lX11

    SOURCES += \
            $$SDIR/unix_autoclicker.cpp \
            $$SDIR/unix_beep.cpp \
            $$SDIR/unix_hook.cpp \
            $$SDIR/unix_keyboard.cpp
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


