#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

#include "hook.h"

#ifdef SAC_WINDOWS
#include <windows.h>

void doMessageLoop(void* _);
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    mainWindow = &w;

    bool errors = false;
    bool hooked = false;

    hooked = Hook::createHook();
    if (!hooked) {
        QMessageBox msgBox;
        msgBox.critical(nullptr, "Error", QString("Could not hook to system! Error: ").append(Hook::getLastError()));
        msgBox.setFixedSize(500, 200);
        errors = true;
    }

#ifdef SAC_WINDOWS
    /* Do message loop in a different thread so it doesn't time out */
    uintptr_t pThread = _beginthread(doMessageLoop, 0, nullptr);
    HANDLE hThread = (HANDLE) pThread;

    bool windowsThread = pThread != -1ULL;
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
        retval = a.exec();
    }

    if (hooked) {
        Hook::releaseHook();
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
void doMessageLoop(void* _) {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) != FALSE) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif
