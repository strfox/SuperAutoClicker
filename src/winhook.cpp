#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <QTextStream>

#include "mainwindow.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

static keycomb_t bindings[4] = {
    { VK_ADD,      false, false, false, false }, // TOGGLE_CLICK
    { VK_SUBTRACT, false, false, false, false }, // TOGGLE_LISTEN
    { VK_DIVIDE,   false, false, false, false }, // TOGGLE_MOUSE
    { VK_MULTIPLY, false, false, false, false }  // TOGGLE_HOLD
};


static HHOOK _hook;


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam);


int hookExists() {
    return !!_hook;
}


int createHook() {
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, _hookProc, nullptr, 0);
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
    if (err == 0) { return nullptr; }
    else {
        wchar_t* buf;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
                            | FORMAT_MESSAGE_FROM_SYSTEM
                            | FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &buf, 0, nullptr);
        QString str = QString::fromWCharArray(buf);
        LocalFree(buf);
        return str;
    }
}


keycomb_t getKeyCombinationFor(action_t action) {
    return bindings[action];
}


QString getStringNameFor(keycomb_t keycomb) {
    BYTE barrKeyState[256] = {0};

    wchar_t szwKeyName[10];
    int retcode = ToUnicodeEx(
                keycomb.vkCode,         //       UINT    wVirtKey
                0,                      //       UINT    wScanCode
                barrKeyState,           // const BYTE   *lpKeyState
                szwKeyName,             //       LPWSTR  pwszBuff
                _countof(szwKeyName),   //       int     cchBuff
                0,                      //       UINT    wFlags
                nullptr                 //       HKL     dwhkl
              );

    const QString strKeyName = QString::fromWCharArray(szwKeyName);
    QString strFullName;
    QTextStream stream(&strFullName);

    if (retcode == 0) {
        stream << "Unknown";
    } else {
        if (keycomb.meta)  { stream << "META "; }
        if (keycomb.ctrl)  { stream << "CTRL "; }
        if (keycomb.alt)   { stream << "ALT ";  }
        if (keycomb.shift) { stream << "SHIFT"; }
        stream << strKeyName;
    }
    return strFullName;
}


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static KBDLLHOOKSTRUCT kbdStruct;
    static BOOL keysDown[VK_LEN]; // Used to store the press-state of virtual keys.

    kbdStruct = *((KBDLLHOOKSTRUCT *) lParam);

    const DWORD vkCode = kbdStruct.vkCode;
    assert(vkCode < VK_LEN);

    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN && !keysDown[vkCode]) {
            keysDown[vkCode] = TRUE; // Mark key as down

            QString name = getStringNameFor({vkCode, false, false, false, false});
            assert(mainWindow != NULL);
            mainWindow->putDebugMsg(QString("Key pressed: %1").arg(name));

            if (vkCode == bindings[TOGGLE_LISTEN].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle mouse"));
            } else if (vkCode == bindings[TOGGLE_CLICK].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle click"));
            } else if (vkCode == bindings[TOGGLE_MOUSE].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle mouse"));
            } else if (vkCode == bindings[TOGGLE_HOLD].vkCode) {
                mainWindow->putDebugMsg(QString("Toggle hold"));
            }
        } else if (wParam == WM_KEYUP) {
            keysDown[vkCode] = FALSE; // Mark key as up
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}
