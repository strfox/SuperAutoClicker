#include "hook.h"

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <QTextStream>

#include "mainwindow.h"
#include "autoclicker.h"
#include "winkeys.h"

#define VK_LEN 0xFF // Assume there are at most 0xFF keys

namespace sac {
namespace hook {


kb::keycomb_t bindings[4] = {
    { VK_ADD,      false, false, false, false }, // TOGGLE_CLICK
    { VK_SUBTRACT, false, false, false, true  }, // TOGGLE_LISTEN
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
                       reinterpret_cast<LPSTR>(&buf),             // LPSTR    lpBuffer
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


LRESULT __stdcall _hookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static KBDLLHOOKSTRUCT kbdStruct;           // Stores the keyboard details
    static BOOL keysDown[VK_LEN];               // Stores the press-state of virtual keys

    kbdStruct = *(reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam));  // Store updated kbdStruct

    const DWORD vkCode = kbdStruct.vkCode;      // Virtual key code
    assert(vkCode < VK_LEN);                    // Ensure it is within range

    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN && !keysDown[vkCode])
        {
            keysDown[vkCode] = TRUE; // Mark key as down

            qDebug("Key pressed: %s", qUtf8Printable(
                 kb::getStringNameFor({vkCode, false, false, false, false})
                )
              );

            assert(sac::autoClicker != nullptr); // Ensure autoClicker has been instantiated by this point

            if (vkCode == bindings[TOGGLE_LISTEN].vkCode)
            {
                qDebug("Toggle listen");
                sac::autoClicker->toggleListenMode();
            }
            else
            if (vkCode == bindings[TOGGLE_CLICK ].vkCode)
            {
                qDebug("Toggle click");
                sac::autoClicker->toggleClickMode();
            }
            else
            if (vkCode == bindings[TOGGLE_MOUSE ].vkCode)
            {
                qDebug("Toggle mouse");
                sac::autoClicker->toggleMouseButton();
            }
            else
            if (vkCode == bindings[TOGGLE_HOLD ].vkCode)
            {
                qDebug("Toggle hold");
                sac::autoClicker->toggleHoldButtonMode();
            }
            else
            {
                bool isNumpadPress    = (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9);
                bool isNumberRowPress = (vkCode >= VK_KEY_0   && vkCode <= VK_KEY_9);

                if (isNumpadPress || isNumberRowPress)
                {
                    uint number = -1U;
                    if (isNumpadPress)
                    {
                        number = vkCode - VK_NUMPAD0;
                    }
                    else
                    {
                        assert(isNumberRowPress);
                        number = vkCode - VK_KEY_0;
                    }
                    assert(number >= 0 && number <= 9);
                    sac::autoClicker->typeNumber(number);
                }
            }
        }
        else if (wParam == WM_KEYUP)
        {
            keysDown[vkCode] = FALSE; // Mark key as up
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

} // namespace hook
} // namespace sac
