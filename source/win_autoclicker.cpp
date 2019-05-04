#include "autoclicker.h"

#include <Windows.h>
#include <math.h>
#include <process.h>
#include <sstream>

#include "beep.h"
#include "hook.h"
#include "mainwindow.h"
#include "util.h"

using sac::kb::keycomb_t;

namespace sac {

keycomb_t _bindings[3] = {
    {VK_ADD, false, false, false, false},     // TOGGLE_CLICK
    {VK_SUBTRACT, false, false, false, true}, // TOGGLE_LISTEN
    {VK_DIVIDE, false, false, false, false},  // TOGGLE_MOUSE
};

} // namespace sac

void _autoclickProc(void *);       // Forward declaration
void _disableMouseBtnProc(void *); // Forward declaration

typedef struct {
  unsigned int *msInterval;
  sac::mousebtn_t *mouseButton;
  void **hRunMutex;
  bool *slowMode;
} autoClickProcArgs_t;

void _startThread(void **hMutex_out, void **hThread_out,
                  void (*_hookProc)(void *), void *procArg) {
  assert(*hMutex_out == nullptr);
  assert(*hThread_out == nullptr);

  *hMutex_out = CreateMutex(nullptr, // LPSECURITY_ATTRIBUTES lpMutexAttributes
                            true,    // WINBOOL               bInitialOwner
                            nullptr  // LPCWSTR               lpName
  );
  uintptr_t ptr = _beginthread(_hookProc, 0, procArg);
  *hThread_out = reinterpret_cast<void *>(ptr);
}

void _stopThread(void **hMutex_out, void **hThread_out) {
  assert(*hMutex_out != nullptr);
  assert(*hThread_out != nullptr);

  ReleaseMutex(*hMutex_out);
  WaitForSingleObject(reinterpret_cast<HANDLE>(hThread_out), 1000);

  *hMutex_out = nullptr;
  *hThread_out = nullptr;
}

void sac::AutoClicker::startClickThread() {
  autoClickProcArgs_t *arg = new autoClickProcArgs_t;

  arg->msInterval = &m_msInterval;
  arg->mouseButton = &m_mouseButton;
  arg->hRunMutex = &m_hClickMutex;
  // arg->holdButton  = &m_holdButtonMode;
  arg->slowMode = &m_slowClickMode;

  _startThread(&m_hClickMutex, &m_hClickThread, _autoclickProc, arg);
}

void sac::AutoClicker::stopClickThread() {
  _stopThread(&m_hClickMutex, &m_hClickThread);
}

void _autoclickProc(/* autoClickProcArgs_t */ void *arg) {
  autoClickProcArgs_t *props = static_cast<autoClickProcArgs_t *>(arg);

  do {
    assert(props != nullptr);
    assert(props->msInterval != nullptr);
    assert(props->mouseButton != nullptr);
    assert(props->hRunMutex != nullptr);
    assert(*props->hRunMutex != nullptr);
    assert(props->slowMode != nullptr);

    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = 0UL;
    input.mi.time = 0;

    switch (*props->mouseButton) {
    case sac::MOUSE1:
      input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN |
                          (*props->slowMode ? 0 : MOUSEEVENTF_LEFTUP));
      break;
    case sac::MOUSE2:
      input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN |
                          (*props->slowMode ? 0 : MOUSEEVENTF_RIGHTUP));
      break;
    }

    SendInput(1, &input, sizeof(INPUT));

    if (*props->slowMode) {
      Sleep(100); // Sleep 100 milliseconds before lifting mouse button
      switch (*props->mouseButton) {
      case sac::MOUSE1:
        input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
        break;
      case sac::MOUSE2:
        input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP);
        break;
      }
      SendInput(1, &input, sizeof(INPUT));
    }
  } while (WaitForSingleObject(*props->hRunMutex, *props->msInterval) ==
           WAIT_TIMEOUT);

  delete props;
}
