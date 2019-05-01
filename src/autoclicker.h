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
#define CFGKEY_HOLDBTN "HoldButtonModeKey"

namespace sac {


class AutoClicker;


extern AutoClicker* autoClicker;


class AutoClicker: public QObject {
    Q_OBJECT
public:
    AutoClicker();
    virtual ~AutoClicker();

    void toggleListenMode();
    void toggleClickMode();
    void toggleMouseButton();
    void toggleHoldButtonMode();
    void typeNumber(uint number);

    void saveConfig();

    QSettings    *m_config;
    bool          m_listenMode     = false;
    bool          m_clickMode      = false;
    bool          m_mouseButton    = false;
    bool          m_holdButtonMode = false;
    unsigned int  m_msInterval     = 100;
private:
    QString  getConfigFilePath();

    void startClickThread();
    void stopClickThread();

    void *m_hRunMutex;
    void *m_hThread;

}; // class AutoCLicker

} // namespace sac

#endif // AUTOCLICKER_H
