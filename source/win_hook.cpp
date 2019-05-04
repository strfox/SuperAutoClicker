#include "hook.h"

#include <QTextStream>
#include <Windows.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "autoclicker.h"
#include "mainwindow.h"
#include "win_keys.h"

#define VK_LEN 0xFF // Assume there are at most 0xFF keys

using sac::kb::keycomb_t;

LRESULT __stdcall _keyboardHookProc(int code, WPARAM wParam, LPARAM lParam);

static HHOOK _hKbdHook;
static HHOOK _hMouseHook;

void sac::hook::createKbdHook() {
  assert(_hKbdHook == nullptr);
  _hKbdHook = SetWindowsHookEx(WH_KEYBOARD_LL,    // int       idHook
                               _keyboardHookProc, // HOOKPROC  lpfn
                               nullptr,           // HINSTANCE hmod
                               0                  // DWORD     dwThreadId
  );
  if (_hKbdHook == nullptr) {
    throw std::runtime_error("Could not create keyboard hook");
  }
}

void sac::hook::releaseKbdHook() {
  assert(_hKbdHook != nullptr);
  BOOL ret = UnhookWindowsHookEx(_hKbdHook);
  if (!ret) {
    throw std::runtime_error("Could not release hook");
  }
}

QString sac::hook::getLastError() {
  DWORD err = GetLastError();
  if (err == 0) {
    return nullptr;
  } else {
    wchar_t *buf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER             // DWORD    dFlags
            | FORMAT_MESSAGE_FROM_SYSTEM           //
            | FORMAT_MESSAGE_IGNORE_INSERTS,       //
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

void sac::hook::createMouseHook() {
  //  assert(_hMouseHook == nullptr);
  //  _hMouseHook = SetWindowsHookEx(WH_MOUSE_LL,             // int idHook
  //                                 _disableMouseButtonProc, // HOOKPROC  lpfn
  //                                 nullptr,                 // HINSTANCE hmod
  //                                 0 // DWORD     dwThreadId
  //  );
  //  if (_hMouseHook == nullptr) {
  //    throw std::runtime_error("Failed to create mouse hook");
  //  }
}

void sac::hook::releaseMouseHook() {
  //  assert(_hMouseHook != nullptr);
  //  BOOL ret = UnhookWindowsHookEx(_hMouseHook);
  //  if (!ret) {
  //    throw std::runtime_error("Failed to release mouse hook");
  //  }
  //  _hMouseHook = nullptr;
}

void _updateKeyComb(keycomb_t &keyComb, uint vkCode, WPARAM wParam) {
  const bool keyDown = wParam == WM_KEYDOWN;

  switch (vkCode) {
  case VK_LWIN:
  case VK_RWIN:
    keyComb.meta = keyDown;
    break;
  case VK_LCONTROL:
  case VK_RCONTROL:
    keyComb.ctrl = keyDown;
    break;
  case VK_LSHIFT:
  case VK_RSHIFT:
    keyComb.shift = keyDown;
    break;
  default:
    keyComb.vkCode = keyDown ? vkCode : 0;
    break;
  }
}

LRESULT __stdcall _keyboardHookProc(int code, WPARAM wParam, LPARAM lParam) {
  static KBDLLHOOKSTRUCT kbdStruct; // Stores the keyboard details
  static bool keysDown[VK_LEN];     // Stores the press-state of virtual keys
  static keycomb_t
      keyComb; // This will store key combination entered by the user

  kbdStruct =
      *(reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam)); // Store updated kbdStruct

  const DWORD vkCode = kbdStruct.vkCode; // Virtual key code
  assert(vkCode < VK_LEN);               // Ensure it is within range
  _updateKeyComb(keyComb, vkCode,
                 wParam); // Update the function-local static keyComb

  if (code >= 0) {
    if (wParam == WM_KEYDOWN && !keysDown[vkCode]) {
      keysDown[vkCode] = true; // Mark key as down

      qDebug("KeyComb: %s", qUtf8Printable(sac::kb::keycombstr(keyComb)));

      sac::AutoClicker *_ac = sac::autoClicker();

      if (keyComb == sac::getKeybind(sac::TOGGLE_LISTEN)) {
        _ac->toggleListenMode();
      } else if (keyComb == sac::getKeybind(sac::TOGGLE_CLICK)) {
        _ac->toggleClickMode();
      } else if (keyComb == sac::getKeybind(sac::TOGGLE_MOUSE)) {
        _ac->toggleMouseButton();
      } else {
        bool isNumpadPress = (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9);
        bool isNumberRowPress = (vkCode >= VK_KEY_0 && vkCode <= VK_KEY_9);

        if (isNumpadPress || isNumberRowPress) {
          uint number = 10UL;
          if (isNumpadPress) {
            number = vkCode - VK_NUMPAD0;
          } else {
            assert(isNumberRowPress);
            number = vkCode - VK_KEY_0;
          }
          assert((number == 0 || number > 0) &&
                 number <= 9); // >= expansion prevents -Wtype-limits
          _ac->typeNumber(number);
        }
      }
    } else if (wParam == WM_KEYUP) {
      keysDown[vkCode] = false; // Mark key as up
    }
  }
  return CallNextHookEx(_hKbdHook, code, wParam, lParam);
}
