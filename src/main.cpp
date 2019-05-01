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
    sac::_autoClicker = &autoClicker;

    sac::MainWindow mainWindow;
    sac::_mainWindow = &mainWindow;

    bool kbdHooked     = false;
    bool mouseHooked   = false;
    bool windowsThread = false;
    bool errors        = false;

    // Create the hooks

    try {
        sac::hook::createKbdHook();
        kbdHooked = true;
        sac::hook::createMouseHook();
        mouseHooked = true;
    }
    catch(std::runtime_error) {
        qDebug("RuntimeError Code: %d", errno);
        QMessageBox::critical(
                    nullptr,
                    "Error",
                    QString("Could not hook to system! Error: ") + sac::hook::getLastError());
        errors = true;
    }

#ifdef SAC_WINDOWS

    // Do message loop in a different thread so it doesn't time out

    uintptr_t pThread = _beginthread(doMessageLoop, 0, nullptr);
    HANDLE    hThread = reinterpret_cast<HANDLE>(pThread);

    if (pThread == -1U)
    {
        // Error while creating thread -- show message box displaying error
        QMessageBox::critical(
                    nullptr,
                    "Error while creating thread",
                    QString("An error occurred while creating the Windows "
                            "message loop thread. Error code: ")
                            + QString(strerror(errno)));
        errors = true;
    } else {
        windowsThread = true;
    }

#endif

    // Start program only if there were no errors during initialization

    int retval = 1;
    if (!errors) {
        mainWindow.show();
        retval = qtApp.exec();
    }

    // Release hooks

    if (kbdHooked) {
        try {
            sac::hook::releaseKbdHook();
        } catch (std::exception &e) {
            qDebug("Error while unhooking keyboard: %s", e.what());
        }
    }
    if (mouseHooked) {
        try {
            sac::hook::releaseMouseHook();
        } catch (std::exception &e) {
            qDebug("Error while unhooking mouse: %s", e.what());
        }
    }

#ifdef SAC_WINDOWS
    // Quit Windows thread

    if (windowsThread) {
        qDebug("Waiting for Windows message thread to quit.");

        PostThreadMessage(
                    GetThreadId(hThread), // DWORD  idThread
                    WM_QUIT,              // UINT   msg
                    0,                    // WPARAM wParam
                    0                     // LPARAM lParam
                 );

        DWORD ret = WaitForSingleObject(hThread, 5000);
        if (ret != WAIT_OBJECT_0) {
            qDebug("Windows thread not ended: %ld", ret);
        }
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
