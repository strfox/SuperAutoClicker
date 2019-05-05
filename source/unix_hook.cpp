#include "hook.h"

#include <QString>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput.h>
#include <assert.h>
#include <memory>
#include <thread>

#define INVALID_EVENT_TYPE -1;

static int MotionType = INVALID_EVENT_TYPE;
static int ButtonPressType = INVALID_EVENT_TYPE;
static int ButtonReleaseType = INVALID_EVENT_TYPE;
static int KeyPressType = INVALID_EVENT_TYPE;
static int KeyReleaseType = INVALID_EVENT_TYPE;
static int ProximityInType = INVALID_EVENT_TYPE;
static int ProximityOutType = INVALID_EVENT_TYPE;

static std::unique_ptr<std::thread> eventThread;

int _XInputVersion(Display *display) {
  static int v = -1;
  if (v != -1) {
    return v;
  }

  XExtensionVersion *version = XGetExtensionVersion(display, INAME);
  if (version &&
      (version != reinterpret_cast<XExtensionVersion *>(NoSuchExtension))) {
    v = version->major_version;
    XFree(version);
  }

  return v;
}

XDeviceInfo *_findDeviceInfo(Display *display, const char *name,
                             const bool onlyExtended) {
  int nDevices;
  bool isId = true;
  XID id = -1UL;
  XDeviceInfo *found = nullptr;
  XDeviceInfo *devices = XListInputDevices(display, &nDevices);

  for (int i = 0; i < static_cast<int>(strlen(name)); i++) {
    if (!isdigit(name[i])) {
      isId = false;
      break;
    }
  }

  if (isId) {
    id = static_cast<unsigned long>(atoi(name));
  }

  for (int i = 0; i < nDevices; i++) {
    if ((!onlyExtended || (devices[i].use >= IsXExtensionDevice)) &&
        ((!isId && strcmp(devices[i].name, name) == 0) ||
         (isId && devices[i].id == id))) {
      if (found) {
        printf("Warning: There are multiple devices named '%s'.\n"
               "To ensure the correct one is selected, please use "
               "the device ID instead.\n\n",
               name);
        return nullptr;
      } else {
        found = &devices[i];
      }
    }
  }

  return found;
}

int _registerEvents(Display *display, const XDeviceInfo *info,
                    const char *devName, const bool handleProximity) {
  XEventClass eventList[7]; // Stores X event classes for registering with
                            // XSelectExtensionEvent
  XDevice *device;
  Window rootWin; // The Xorg root window
  int number = 0; // Number of events registered
  int screen;     // The default screen number referenced in the XOpenDisplay
                  // routine.

  screen = DefaultScreen(display);
  rootWin = RootWindow(display, screen);
  device = XOpenDevice(display, info->id);

  if (!device) {
    throw std::runtime_error(std::string("unable to open device ") + devName);
  }

  if (device->num_classes > 0) {
    int i;
    XInputClassInfo *ip;

    for (i = 0, ip = device->classes; i < info->num_classes; ip++, i++) {
      switch (ip->input_class) {
      case KeyClass:
        DeviceKeyPress(device, KeyPressType, eventList[number]);
        number++;
        DeviceKeyRelease(device, KeyReleaseType, eventList[number]);
        number++;
        break;
      case ButtonClass:
        DeviceButtonPress(device, ButtonPressType, eventList[number]);
        number++;
        DeviceButtonRelease(device, ButtonReleaseType, eventList[number]);
        number++;
        break;
      case ValuatorClass:
        DeviceMotionNotify(device, MotionType, eventList[number]);
        number++;
        if (handleProximity) {
          ProximityIn(device, ProximityInType, eventList[number]);
          number++;
          ProximityOut(device, ProximityOutType, eventList[number]);
          number++;
        }
        break;
      default:
        qDebug("unknown class");
        break;
      }
    }

    if (XSelectExtensionEvent(display, rootWin, eventList, number)) {
      qDebug("Error selecting extended events");
      return 0;
    }
  }
  return number;
}

[[noreturn]] void _printEvents(Display *display) {
  XEvent event;

  while (true) {
    XNextEvent(display, &event);
    qDebug("Got event: %d", event.type);
  }
}

[[noreturn]] void _test(Display *display, const char *deviceId) {
  const XDeviceInfo *info = _findDeviceInfo(display, deviceId, true);
  const bool handleProximity = true;

  qDebug("_test: deviceId=%s", deviceId);

  if (!info) {
    throw std::runtime_error(std::string("unable to find device ") + deviceId);
  } else {
    if (_registerEvents(display, info, deviceId, handleProximity)) {
      _printEvents(display);
    } else
      throw std::runtime_error("no events registered...");
  }
}

[[noreturn]] void _XEventThread() {
  Display *display;
  int event;
  int error;
  int xi_opcode;

  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    throw std::runtime_error("Could not connect to X server");
  }

  if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event,
                       &error)) {
    throw std::runtime_error("X Input Extension not available");
  }

  // Determine the installed version of X11
  if (!_XInputVersion(display)) {
    throw std::runtime_error(std::string(INAME) + " extension not available");
  }

  char deviceId[10];
  sprintf(deviceId, "11");

  _test(display, deviceId);

  // XSync(display, false);
  // XCloseDisplay(display);
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
