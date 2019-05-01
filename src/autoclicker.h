#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include <QString>
#include <QSettings>
#include <QObject>
#include <memory>

#include "keyboard.h"

#define CFGKEY_LISTEN "ListenModeKey"
#define CFGKEY_CLICKMODE "ClickModeKey"
#define CFGKEY_MOUSEBTN "MouseButtonKey"
// #define CFGKEY_HOLDBTN "HoldButtonModeKey"

#define MAX_MS_DIGITS  6
#define CFGKEYS_AMOUNT 3

namespace sac {


class AutoClicker: public QObject {
    Q_OBJECT
public:
    AutoClicker();
    virtual ~AutoClicker();

    void toggleListenMode();
    void toggleClickMode();
    void toggleMouseButton();
    // void toggleHoldButtonMode();
    void typeNumber(uint number);
    void saveConfig();

    QSettings  *m_config         = nullptr;
    bool        m_listenMode     = false;
    bool        m_clickMode      = false;
    mousebtn_t  m_mouseButton    = MOUSE1;
    // bool        m_holdButtonMode = false;
    uint        m_msInterval     = 0;
    uint        m_msInput        = 0;       // Used only for listen mode input
private:
    QString  getConfigFilePath();
    void     refreshMainWindow();
    void     mainWindowPutMsg();
    void     startClickThread();
    void     stopClickThread();

    void *m_hClickMutex  = nullptr;
    void *m_hClickThread = nullptr;

}; // class AutoCLicker

extern AutoClicker* _autoClicker; // Use autoClicker()

AutoClicker* autoClicker();


} // namespace sac

#endif // AUTOCLICKER_H
