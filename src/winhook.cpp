#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <QTextStream>

#include "mainwindow.h"
#include "autoclicker.h"

// Assume there are at most 0xFF keys
#define VK_LEN 0xFF

namespace sac {
namespace hook {


static kb::keycomb_t bindings[4] = {
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


void setBind(action_t actionkey, kb::keycomb_t keycomb) {
    bindings[actionkey] = keycomb;
}


QString getLastError() {
    DWORD err = GetLastError();
    if (err == 0) { return nullptr; }
    else {
        wchar_t* buf;
        FormatMessageA(
                       FORMAT_MESSAGE_ALLOCATE_BUFFER             // DWORD    dFlags
                            | FORMAT_MESSAGE_FROM_SYSTEM          //
                            | FORMAT_MESSAGE_IGNORE_INSERTS,      //
                       nullptr,                                   // LPCVOID  source
                       err,                                       // DWORD    dwMessageId
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // DWORD    dwLanguageId
                       (LPSTR) &buf,                              // LPSTR    lpBuffer
                       0,                                         // DWORD    nSize
                       nullptr                                    // va_list *Arguments
                   );
        QString str = QString::fromWCharArray(buf);
        LocalFree(buf);
        return str;
    }
}


kb::keycomb_t getKeyCombinationFor(action_t action) {
    return bindings[action];
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

            QString name = kb::getStringNameFor({vkCode, false, false, false, false});
            qDebug("Key pressed: %s", qUtf8Printable(name));

            if (vkCode == bindings[TOGGLE_LISTEN].vkCode) {
                qDebug("Toggle listen");
                sac::autoClicker->toggleListenMode();
            }
            else if (vkCode == bindings[TOGGLE_CLICK].vkCode) {
                qDebug("Toggle click");
                sac::autoClicker->toggleClickMode();
            }
            else if (vkCode == bindings[TOGGLE_MOUSE].vkCode) {
                qDebug("Toggle mouse");
                sac::autoClicker->toggleMouseButton();
            }
            else if (vkCode == bindings[TOGGLE_HOLD].vkCode) {
                qDebug("Toggle hold");
                sac::autoClicker->toggleHoldButtonMode();
            }
        } else if (wParam == WM_KEYUP) {
            keysDown[vkCode] = FALSE; // Mark key as up
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

} // namespace hook
} // namespace sac
