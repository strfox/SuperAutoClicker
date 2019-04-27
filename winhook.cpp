#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <map>

#include "mainwindow.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

namespace Hook {


typedef std::map<actionkey_t, DWORD> _keymap_t;


_keymap_t _defaultKeyMap() {
    _keymap_t map;
    map[TOGGLE_CLICK] = VK_ADD;
    map[TOGGLE_LISTEN] = VK_SUBTRACT;
    map[TOGGLE_MOUSE] = VK_DIVIDE;
    return map;
}


static _keymap_t _keyMap;
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


void setBind(actionkey_t actionkey, unsigned long keyCode) {
    assert(_keyMap.count(actionkey) == 1);
    _keyMap[actionkey] = keyCode;
}


QString getLastError() {
    DWORD err = GetLastError();
    if (err == 0) {
        return QString();
    }

    LPSTR buf;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &buf, 0, nullptr);

    return QString(buf);
}


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static KBDLLHOOKSTRUCT kbdStruct;
    kbdStruct = *((KBDLLHOOKSTRUCT *) lParam);

    /**
     * @brief keysDown: Used to store the press-state of virtual keys.
     */
    static BOOL keysDown[VK_LEN];

    DWORD vkCode = kbdStruct.vkCode;
    assert(vkCode < VK_LEN);

    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN && !keysDown[vkCode]) {
            // Mark key as down
            keysDown[vkCode] = TRUE;

            assert(mainWindow != NULL);

#ifdef QT_DEBUG
            QString msg = QString("Key pressed: %1\n").arg(QString::number(vkCode));
            mainWindow->putMessage(msg);
#endif

            // TODO

        } else if (wParam == WM_KEYUP) {
            // Mark key as up
            keysDown[vkCode] = FALSE;
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

}
