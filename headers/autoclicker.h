#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <memory>

#include "keyboard.h"
#include <vector>

#define CFGKEY_LISTEN "ListenModeKey"
#define CFGKEY_CLICKMODE "ClickModeKey"
#define CFGKEY_MOUSEBTN "MouseButtonKey"
#define MAX_MS_DIGITS 6
#define CFGKEYS_AMOUNT 3

namespace sac {

static kb::keycomb_t bindings[3] = {
    {0, false, false, false, false}, // TOGGLE_CLICK
    {0, false, false, false, true},  // TOGGLE_LISTEN
    {0, false, false, false, false}, // TOGGLE_MOUSE
};

class AutoClicker : public QObject {
  Q_OBJECT

public:
  AutoClicker();
  virtual ~AutoClicker();

  void toggleListenMode();
  void toggleClickMode();
  void toggleMouseButton();
  void typeNumber(uint number);
  void saveConfig();

  QSettings *m_config = nullptr;
  mousebtn_t m_mouseButton = MOUSE1; // Mouse button to press
  uint m_msInterval = 0;             // Click interval in milliseconds
  uint m_msInput = 0;                // Used only for "Listen Mode" input
  bool m_listenMode = false;         // "Listen Mode" flag
  bool m_clickMode = false;          // "Click Mode" flag
  bool m_slowClickMode = false;      // v2.0.2: "Slow Click" Flag

private:
  QString getConfigFilePath();
  void refreshMainWindow();
  void mainWindowPutMsg();

  std::vector<kb::keycomb_t> getDefaultKeys(); // Platform-specific
  void startClickThread();                     // Platform-specific
  void stopClickThread();                      // Platform-specific

  void *m_hClickMutex = nullptr;  // Mutex handle for "Click Mode" thread
  void *m_hClickThread = nullptr; // "Click Mode" thread handle

}; // class AutoCLicker

extern AutoClicker *_autoClicker; // Use autoClicker() for safety
AutoClicker *autoClicker();

} // namespace sac

#endif // AUTOCLICKER_H
