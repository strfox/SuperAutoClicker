#include "autoclicker.h"

#include <sstream>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>

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
}

void AutoClicker::toggleClickMode() {
    qDebug("sac::toggleClickMode");
}

void AutoClicker::toggleMouseButton() {
    qDebug("sac::toggleMouseButton");
}

void AutoClicker::toggleHoldButtonMode() {
    qDebug("sac::toggleHoldButtonMode");
}

} // namespace sac
