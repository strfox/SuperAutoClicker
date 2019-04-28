#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>

#include "bimap.h"
#include "mainwindow.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

namespace Hook {


typedef ::bimap<actionkey_t, DWORD> _keymap_t;


_keymap_t _defaultKeyMap() {
    _keymap_t map;
    map.put(TOGGLE_CLICK, VK_ADD);
    map.put(TOGGLE_LISTEN, VK_SUBTRACT);
    map.put(TOGGLE_MOUSE, VK_DIVIDE);
    return map;
}


static _keymap_t _keyMap = _defaultKeyMap();
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
    if (_keyMap.count(keyCode) > 0) {
        QString err("keycode %1 already bound");
        err = err.arg(QString::number(keyCode));
        throw std::invalid_argument(err.toStdString());
    }
    _keyMap.put(actionkey, keyCode);
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

    const DWORD vkCode = kbdStruct.vkCode;
    assert(vkCode < VK_LEN);

    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN && !keysDown[vkCode]) {
            // Mark key as down
            keysDown[vkCode] = TRUE;

            assert(mainWindow != nullptr);
            mainWindow->putDebugMsg(QString("Key pressed: %1").arg(QString::number(vkCode)));

            const auto vkActionMap = _keyMap.getInverted();
            auto it = vkActionMap.find(vkCode);
            if (it != vkActionMap.end()) {
                const actionkey_t action = it->second;
                switch (action) {
                case TOGGLE_CLICK:
                    mainWindow->putDebugMsg("Toggle click");
                    break;
                case TOGGLE_MOUSE:
                    mainWindow->putDebugMsg("Toggle mouse");
                    break;
                case TOGGLE_LISTEN:
                    mainWindow->putDebugMsg("Toggle listen");
                    break;
                }
            }
        } else if (wParam == WM_KEYUP) {
            // Mark key as up
            keysDown[vkCode] = FALSE;
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

}
