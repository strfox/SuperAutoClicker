#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include <QFile>
#include <QObject>
#include <QSettings>
#include <QString>

#include <memory>
#include <vector>

#include "keyboard.h"

#define CFGKEY_LISTEN "ListenModeKey"
#define CFGKEY_CLICKMODE "ClickModeKey"
#define CFGKEY_MOUSEBTN "MouseButtonKey"
#define MAX_MS_DIGITS 6
#define CFGKEYS_AMOUNT 3

namespace sac {

extern kb::keycomb_t _bindings[];

kb::keycomb_t getKeybind(action_t action);

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

  void syncBindings();

private:
  QString getConfigFilePath();
  void refreshMainWindow();
  void mainWindowPutMsg();

  void startClickThread(); // Platform-specific
  void stopClickThread();  // Platform-specific

  void *m_hClickMutex = nullptr;  // Mutex handle for "Click Mode" thread
  void *m_hClickThread = nullptr; // "Click Mode" thread handle

  QString getConfigPath();
  void touchFile(QFile &file);
  void insertDefaultKeysAndValues();
}; // class AutoCLicker

extern AutoClicker *_autoClicker; // Use autoClicker() for safety
AutoClicker *autoClicker();

} // namespace sac

#endif // AUTOCLICKER_H
