#include "hook.h"

#include <QString>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <memory>
#include <thread>

static std::unique_ptr<std::thread> eventThread;

[[noreturn]] void _XEventThread() {
  Display *d = XOpenDisplay(nullptr);
  assert(d != nullptr);

  KeySym ks;
  XComposeStatus xCS;
  char kBuf[20]; // Stores the pressed key's name
  int len;       // Length of the null-terminated string stored in szKeyBuf

  while (true) {
    XEvent ev;
    XNextEvent(d, &ev);

    switch (ev.type) {
    case KeyPress:
      len = XLookupString(&ev.xkey, kBuf, sizeof(kBuf), &ks, &xCS);

      if (len > 0 && isprint(kBuf[0])) {
        kBuf[len] = '\0';
        qDebug("XKeyPressed: %s", kBuf);
      } else {
        qDebug("XKeyPressed: %d", static_cast<int>(ks));
      }

      break;
    }
  }
}

void sac::hook::createKbdHook() {
  assert(eventThread.get() == nullptr);
  eventThread.reset(new std::thread(_XEventThread));
  assert(eventThread.get() != nullptr);
}

void sac::hook::releaseKbdHook() {
  assert(eventThread.get() != nullptr);
  eventThread.reset();
  assert(eventThread.get() == nullptr);
}

QString sac::hook::getLastError() {
  return QString("Unix_getLastError: Not implemented");
}

void sac::hook::createMouseHook() {}

void sac::hook::releaseMouseHook() {}
