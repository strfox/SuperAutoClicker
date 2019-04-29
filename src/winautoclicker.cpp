#include "autoclicker.h"

#include <windows.h>
#include <sstream>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "hook.h"

#define CFGKEYS_AMOUNT 4

namespace sac {

AutoClicker* autoClicker;

AutoClicker::AutoClicker() {
    // Set up config
    {
       QString cfgPath = getConfigFilePath();
       assert(!cfgPath.isEmpty());
       QFile file(cfgPath);

       // Touch config file if it doesn't exist

       if (!file.exists()) {
           bool ret = file.open(QIODevice::WriteOnly);
           if (!ret) {
               std::ostringstream os;
               os << "Could not create config file at ";
               os << cfgPath.toStdString();
               os << ". Error: ";
               os << file.error();
               throw std::runtime_error(os.str());
           }
       }

       m_config = new QSettings(cfgPath, QSettings::IniFormat, this);
    }

    if (m_config->allKeys().size() == 0) {
        // Initial configuration
        m_config->setValue(CFGKEY_LISTEN    , QString::number(VK_SUBTRACT) + ",0,0,0,0");
        m_config->setValue(CFGKEY_CLICKMODE , QString::number(VK_ADD     ) + ",0,0,0,0");
        m_config->setValue(CFGKEY_MOUSEBTN  , QString::number(VK_DIVIDE  ) + ",0,0,0,0");
        m_config->setValue(CFGKEY_HOLDBTN   , QString::number(VK_DECIMAL ) + ",0,0,0,0");
        m_config->sync();
    }

    const int keysAmount = m_config->allKeys().size();
    if (keysAmount != CFGKEYS_AMOUNT) {
        char buf[256];
        snprintf(buf, sizeof(buf),  "Config might be corrupted. Expected config to have %d keys but instead got %d", CFGKEYS_AMOUNT, keysAmount);
        throw std::runtime_error(buf);
    }

    // Create key combinations from config

    kb::keycomb_t listenComb   = kb::parseKeyComb(m_config->value(CFGKEY_LISTEN   ).toString());
    kb::keycomb_t clickComb    = kb::parseKeyComb(m_config->value(CFGKEY_CLICKMODE).toString());
    kb::keycomb_t mouseBtnComb = kb::parseKeyComb(m_config->value(CFGKEY_MOUSEBTN ).toString());
    kb::keycomb_t holdBtnComb  = kb::parseKeyComb(m_config->value(CFGKEY_HOLDBTN  ).toString());

    hook::setBind(hook::TOGGLE_LISTEN, listenComb  );
    hook::setBind(hook::TOGGLE_CLICK , clickComb   );
    hook::setBind(hook::TOGGLE_MOUSE , mouseBtnComb);
    hook::setBind(hook::TOGGLE_HOLD  , holdBtnComb );
}

AutoClicker::~AutoClicker() {
    delete m_config;
}

QString AutoClicker::getConfigFilePath() {
    QString path = QDir::homePath();
      path = path.append("/SuperAutoClicker Configuration.ini");

    qDebug("Config file path: %s", qUtf8Printable(path));

    if (path.isEmpty()) {
        throw std::runtime_error("Could not find file path for config file");
    } else return path;
}

void AutoClicker::toggleListenMode() {
   qDebug("sac::toggleListenMode");
   m_listenMode = !m_listenMode;
}

void AutoClicker::toggleClickMode() {
    qDebug("sac::toggleClickMode");
    m_clickMode = !m_clickMode;
}

void AutoClicker::toggleMouseButton() {
    qDebug("sac::toggleMouseButton");
    m_mouseButton = !m_mouseButton;
}

void AutoClicker::toggleHoldButtonMode() {
    qDebug("sac::toggleHoldButtonMode");
    m_holdButtonMode = !m_holdButtonMode;
}

} // namespace sac
