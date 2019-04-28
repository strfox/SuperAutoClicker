#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <QTextStream>

#include "mainwindow.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

static keycomb_t bindings[3] = {
    { VK_ADD,      false, false, false, false }, // TOGGLE_CLICK
    { VK_SUBTRACT, true, true, true, true     }, // TOGGLE_LISTEN
    { VK_DIVIDE,   false, false, true, false  }  // TOGGLE_MOUSE
};


static HHOOK _hook;


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam);


int hookExists() {
    return !!_hook;
}


int createHook() {
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, _hookProc, NULL, 0);
    return hookExists();
}


int releaseHook() {
    return UnhookWindowsHookEx(_hook);
}


void setBind(action_t actionkey, keycomb_t keycomb) {
    bindings[actionkey] = keycomb;
}


QString getLastError() {
    DWORD err = GetLastError();
    if (err == 0) {
        return NULL;
    }
    wchar_t* buf;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &buf, 0, NULL);
    QString str = QString::fromWCharArray(buf);
    LocalFree(buf);
    return str;
}


keycomb_t getKeyCombinationFor(action_t action) {
    return bindings[action];
}


QString getStringNameFor(keycomb_t keycomb) {
    BYTE barrKeyState[256] = {0};

    wchar_t szwKeyName[10];
    int retcode = ToUnicodeEx(keycomb.vkCode, 0, barrKeyState, szwKeyName, _countof(szwKeyName), 0, NULL);
    const QString strKeyName = QString::fromWCharArray(szwKeyName);

    QString strFullName;
    QTextStream stream(&strFullName);

    if (retcode == 0) {
        stream << "Unknown";
    } else {
        if (keycomb.meta) {
            stream << "META ";
        }
        if (keycomb.ctrl) {
            stream << "CTRL ";
        }
        if (keycomb.alt) {
            stream << "ALT ";
        }
        if (keycomb.shift) {
            stream << "SHIFT";
        }
        stream << strKeyName;
    }

    return strFullName;
}


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static KBDLLHOOKSTRUCT kbdStruct;
    kbdStruct = *((KBDLLHOOKSTRUCT *) lParam);

    /**
     * @brief keysDown: Used to store the press-state of virtual keys.
     */
    static BOOL keysDown[VK_LEN];

    const DWORD vkCode = kbdStruct.vkCode;
    assert(vkCode < VK_LEN);

    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN && !keysDown[vkCode]) {
            // Mark key as down
            keysDown[vkCode] = TRUE;

            QString name = getStringNameFor({vkCode, false, false, false, false});
            assert(mainWindow != NULL);
            mainWindow->putDebugMsg(QString("Key pressed: %1").arg(name));

            if (vkCode == bindings[TOGGLE_LISTEN].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle mouse"));
            } else if (vkCode == bindings[TOGGLE_CLICK].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle click"));
            } else if (vkCode == bindings[TOGGLE_MOUSE].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle mouse"));
            }
        } else if (wParam == WM_KEYUP) {
            // Mark key as up
            keysDown[vkCode] = FALSE;
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}
