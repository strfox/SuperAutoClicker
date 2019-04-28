#include "hook.h"

#include <QString>
#include <QTextStream>

#include <windows.h>
#include <assert.h>
#include <stdio.h>

#include "bimap.h"
#include "mainwindow.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

namespace Hook {


typedef ::bimap<action_t, keycomb_t> _keymap_t;


_keymap_t _defaultKeyMap() {
    _keymap_t map;
    map.put(TOGGLE_CLICK, {VK_ADD, false, false, true, false});
    map.put(TOGGLE_LISTEN, {VK_SUBTRACT, false, true, false, false});
    map.put(TOGGLE_MOUSE, {VK_DIVIDE, true, false, false, false});
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


void setBind(action_t actionkey, keycomb_t keycomb) {
    assert(_keyMap.count(actionkey) == 1);
    if (_keyMap.count(keycomb) > 0) {
        throw std::invalid_argument("already bound");
    }
    _keyMap.put(actionkey, keycomb);
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


keycomb_t getKeyCombinationFor(action_t action) {
    auto map = _keyMap.getMap();
    qDebug("getKeyCombinationFor(%d)", action);
    auto it = map.find(action);
    if (it == map.end()) {
        throw std::runtime_error(std::string("Action ") + actionnames[action] +  " not found in actionKey<->vkCode map");
    }
    return it->second;
}


QString getStringNameFor(keycomb_t keycomb) {
    const int sizeBuf = 32;
    wchar_t szBuf[sizeBuf];
    BYTE barrKeyState[256] = {0};
    int retcode = ToUnicodeEx(keycomb.vkCode, 0, barrKeyState, szBuf, _countof(szBuf), 0, nullptr);
    if (retcode == 0) {
        return QString("unknown");
    } else {
        QString key = QString::fromWCharArray(szBuf, sizeBuf);
        QString fullname;
        QTextStream out(&fullname);
        if (keycomb.meta) {
            out << "META ";
        }
        if (keycomb.ctrl) {
            out << "CTRL ";
        }
        if (keycomb.alt) {
            out << "ALT ";
        }
        if (keycomb.shift) {
            out << "SHIFT ";
        }
        out << key;
        return fullname;
    }
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
            auto it = vkActionMap.find({vkCode, false, false, false, false});
            if (it != vkActionMap.end()) {
                const action_t action = it->second;
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
