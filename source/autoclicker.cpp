#include "autoclicker.h"

#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QSettings>

#include "beep.h"
#include "hook.h"
#include "mainwindow.h"
#include "util.h"

using sac::kb::keycomb_t;
using sac::kb::parseKeyComb;

namespace sac {

AutoClicker *_autoClicker;
AutoClicker *autoClicker() {
  assert(_autoClicker != nullptr);
  return _autoClicker;
}

AutoClicker::AutoClicker() {
  // Set up config

  QString cfgPath = getConfigFilePath();
  assert(!cfgPath.isEmpty());
  QFile file(cfgPath);

  // Touch config file if it doesn't exist

  if (!file.exists()) {
    bool ret = file.open(QIODevice::WriteOnly);
    if (!ret) {
      throw std::runtime_error(
          std::string("Could not create config file at: ") +
          cfgPath.toStdString() + ". Error: " + std::to_string(file.error()));
    }
  }

  m_config = new QSettings(cfgPath, QSettings::IniFormat, this);

  if (m_config->allKeys().size() == 0) {
    // Populate in-memory configuration with default keys and values
    auto keys = getDefaultKeys();
    assert(keys.size() == 3);
    m_config->setValue(CFGKEY_LISTEN, kb::stringify(keys[0]));
    m_config->setValue(CFGKEY_CLICKMODE, kb::stringify(keys[1]));
    m_config->setValue(CFGKEY_MOUSEBTN, kb::stringify(keys[2]));
    m_config->sync();
  }

  // Ensure m_config has the right amount of keys

  const int keysAmount = m_config->allKeys().size();
  if (keysAmount != CFGKEYS_AMOUNT) {
    QString msg = QString("Expected config to have ") +
                  QString::number(CFGKEYS_AMOUNT) + " keys but it has " +
                  QString::number(keysAmount) +
                  " instead. Please edit your configuration at \"" +
                  getConfigFilePath() + "\" and restart the program.";
    QMessageBox::critical(nullptr, tr("Configuration Error"), msg);
    throw std::runtime_error(msg.toStdString());
  }

  // Create keycomb_t instances from QSettings

  keycomb_t listenComb =
      parseKeyComb(m_config->value(CFGKEY_LISTEN).toString());
  keycomb_t clickComb =
      parseKeyComb(m_config->value(CFGKEY_CLICKMODE).toString());
  keycomb_t mouseBtnComb =
      parseKeyComb(m_config->value(CFGKEY_MOUSEBTN).toString());

  // Assign config keys to hook

  sac::hook::setBind(TOGGLE_LISTEN, listenComb);
  sac::hook::setBind(TOGGLE_CLICK, clickComb);
  sac::hook::setBind(TOGGLE_MOUSE, mouseBtnComb);
}

AutoClicker::~AutoClicker() { delete m_config; }

QString AutoClicker::getConfigFilePath() {
  QString path = QDir::homePath();
  path = path.append("/SuperAutoClicker Configuration.ini");
  if (path.isEmpty()) {
    throw std::runtime_error("Could not find file path for config file");
  } else {
    return path;
  }
}

void AutoClicker::refreshMainWindow() { mainWindow()->refresh(); }

void AutoClicker::toggleListenMode() {
  if (m_listenMode) {
    if (m_msInput == 0) {
      mainWindow()->putMsg(
          tr("Enter a millisecond interval using the number keys."));
    } else {
      assert(m_msInput > 0);
      m_msInterval = m_msInput;
      m_msInput = 0;
      assert(m_msInput == 0);
      assert(m_msInterval > 0);
    }
  }
  m_listenMode = !m_listenMode;

  MainWindow *_w = mainWindow();
  if (m_listenMode) {
    _w->putMsg(tr("Listen Mode: ON"));
  } else {
    _w->putMsg(tr("Listen Mode: OFF"));
  }
  refreshMainWindow();
}

void AutoClicker::toggleClickMode() {
  assert(m_msInterval == 0 ||
         m_msInterval > 0); // >= expansion prevents -Wtype-limits
  if (m_listenMode) {
    toggleListenMode();
  }

  MainWindow *_w = mainWindow();

  if (m_msInterval == 0) {
    beepError();
    _w->putMsg(QString(tr("No millisecond interval entered.")));
  } else {
    if (m_clickMode) {
      stopClickThread();
      _w->putMsg(tr("Click Mode: OFF"));
    } else {
      startClickThread();
      _w->putMsg(tr("Click Mode: ON"));
    }

    m_clickMode = !m_clickMode;
    refreshMainWindow();
  }
}

void AutoClicker::toggleMouseButton() {
  /* In case the program is added support for more mouse buttons, this function
   * will cycle through them, hence the switch statement.
   */
  MainWindow *_w = mainWindow();

  switch (m_mouseButton) {
  case MOUSE1:
    m_mouseButton = MOUSE2;
    _w->putMsg(tr("Using MOUSE2 button."));
    break;
  case MOUSE2:
    m_mouseButton = MOUSE1;
    _w->putMsg(tr("Using MOUSE1 button."));
    break;
  }
  refreshMainWindow();
}

void AutoClicker::saveConfig() { m_config->sync(); }

void AutoClicker::typeNumber(uint number) {
  assert(number == 0 || number > 0); // >= expansion prevents -Wtype-limits
  if (number > 9U) {
    throw std::invalid_argument(
        std::string("expected number to be in range 0..9 but number was ") +
        std::to_string(number));
  }

  if (m_listenMode) {
    const uint digits = digitsInNumber(m_msInput);
    qDebug("Digits in %d: %d", m_msInput, digits);

    if (digits >= MAX_MS_DIGITS) {
      mainWindow()->putMsg(tr("Digit limit reached! Turn off listen mode."));
      beepError();
    } else {
      m_msInput *= 10;
      m_msInput += number;
      beepTypeMs();
    }
    refreshMainWindow();
  }

  assert(digitsInNumber(m_msInput) <= MAX_MS_DIGITS);
}

} // namespace sac
