#include "hook.h"

#include "mainwindow.h"
#include "autoclicker.h"

#include <QApplication>
#include <QMessageBox>

#ifdef SAC_WINDOWS
#include <windows.h>

void doMessageLoop(void* _);
#endif


int main(int argc, char *argv[])
{
    QApplication qtApp(argc, argv);

    sac::AutoClicker autoClicker;
    sac::autoClicker = &autoClicker;

    sac::MainWindow mainWindow;
    sac::mainWindow = &mainWindow;

    bool errors = false;
    bool hooked = false;

    hooked = sac::hook::createHook();
    if (!hooked) {
        QMessageBox msgBox;

        QString err = sac::hook::getLastError();
        msgBox.critical(nullptr, "Error", QString("Could not hook to system! Error: ").append(err));
        msgBox.setFixedSize(500, 200);
        errors = true;
    }

#ifdef SAC_WINDOWS
    /* Do message loop in a different thread so it doesn't time out */
    uintptr_t pThread = _beginthread(doMessageLoop, 0, nullptr);
    HANDLE hThread = (HANDLE) pThread;

    bool windowsThread = pThread != -1U;
    if (!windowsThread) {
        // Error while creating thread -- show message box displaying error
        QString msg("An error occurred! Code: ");
        msg.append(QString(strerror(errno)));
        QMessageBox msgBox;
        msgBox.critical(nullptr, "Error", msg);
        msgBox.setFixedSize(500, 200);
        errors = true;
    }
#endif

    // Start program only if there were no errors during initialization
    int retval = 1;
    if (!errors) {
        mainWindow.show();
        retval = qtApp.exec();
    }

    if (hooked) {
        sac::hook::releaseHook();
    }

#ifdef SAC_WINDOWS
    if (windowsThread) {
        qDebug("Waiting for message thread to quit.");
        DWORD threadId = GetThreadId(hThread);
        PostThreadMessage(threadId, WM_QUIT, 0, 0);
        WaitForSingleObject(hThread, INFINITE);
    }
#endif

    return retval;
}

#ifdef SAC_WINDOWS
void doMessageLoop(void*) {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) != FALSE) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif
