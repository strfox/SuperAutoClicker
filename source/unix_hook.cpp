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
  Display *display;
  Window window;
  XEvent ev;
  int s;

  // Open connection with the X display server
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    throw std::runtime_error("Cannot open connection with X display server");
  }
  s = DefaultScreen(display);
  window = XRootWindow(display, s);
  XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

  while (true) {
    XNextEvent(display, &ev);
    qDebug("XEvent: %d", ev.type);
    switch (ev.type) {
    case KeyPress:
      qDebug("KeyPress");
      break;
    }
  }

  // Close the connection
  XCloseDisplay(display);
}

void sac::hook::createKbdHook() {
  assert(eventThread.get() == nullptr);
  eventThread.reset(new std::thread(_XEventThread));
  qDebug("UnixKbdHookCreated");
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
