#include <stdio.h>
#include <process.h>
#include <assert.h>
#include "click.h"

void _clickproc();

int ThreadNr; // Number of threads started
HANDLE hRunMutex = NULL;

int clickIntervalMs = 0;
mousebtn_t mousebtn = MOUSEBTN_LEFT;

BOOL StartClickThread(int ms) {
    assert(ms > 0);
    assert(ThreadNr >= 0);

    if (ThreadNr > 0) {
        return FALSE;
    }

    assert(hRunMutex == NULL);

    hRunMutex = CreateMutex(NULL, TRUE, NULL);
    ThreadNr++;
    clickIntervalMs = ms;
    _beginthread(_clickproc, 0, &ThreadNr);
    return TRUE;
}

void StopClickThread() {
    assert(ThreadNr == 1);

    ReleaseMutex(hRunMutex);
    hRunMutex = NULL;
    ThreadNr--;
}

void SetMsInterval(int ms) {
    clickIntervalMs = ms;
}

void SetMouseButton(mousebtn_t btn) {
    mousebtn = btn;
}

mousebtn_t GetMouseButton() {
    return mousebtn;
}

void _clickproc() {
    do {
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.dx = 0;
        input.mi.dy = 0;

        switch (mousebtn) {
            case MOUSEBTN_LEFT:
                input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
                break;
            case MOUSEBTN_RIGHT:
                input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
                break;
            default:
                assert(FALSE);
        }

        input.mi.mouseData = 0;
        input.mi.dwExtraInfo = (long unsigned int) NULL;
        input.mi.time = 0;
        SendInput(1, &input, sizeof(INPUT));
        // Repeat while mutex is still taken.
    } while (WaitForSingleObject(hRunMutex, clickIntervalMs) == WAIT_TIMEOUT);
}