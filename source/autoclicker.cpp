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
using sac::kb::parse;

namespace sac {

AutoClicker *_autoClicker;
AutoClicker *autoClicker() {
  assert(_autoClicker != nullptr);
  return _autoClicker;
}

} // namespace sac

keycomb_t sac::getKeybind(action_t action) { return _bindings[action]; }

sac::AutoClicker::AutoClicker() {
  // Set up config
  QString configPath = getConfigPath();
  QFile file(getConfigPath());
  // Touch config file if it doesn't exist
  touchFile(file);

  m_config = new QSettings(configPath, QSettings::IniFormat, this);

  // Populate in-memory configuration with default keys and values
  if (m_config->allKeys().empty()) {
    m_config->setValue(CFGKEY_LISTEN, kb::stringify(_bindings[TOGGLE_LISTEN]));
    m_config->setValue(CFGKEY_CLICKMODE,
                       kb::stringify(_bindings[TOGGLE_CLICK]));
    m_config->setValue(CFGKEY_MOUSEBTN, kb::stringify(_bindings[TOGGLE_MOUSE]));
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

  syncBindings();
}

sac::AutoClicker::~AutoClicker() { delete m_config; }

QString sac::AutoClicker::getConfigFilePath() {
  QString path = QDir::homePath();
  path = path.append("/SuperAutoClicker Configuration.ini");
  if (path.isEmpty()) {
    throw std::runtime_error("Could not find file path for config file");
  } else {
    return path;
  }
}

void sac::AutoClicker::refreshMainWindow() { mainWindow()->refresh(); }

void sac::AutoClicker::toggleListenMode() {
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

void sac::AutoClicker::toggleClickMode() {
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

void sac::AutoClicker::toggleMouseButton() {
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

void sac::AutoClicker::saveConfig() { m_config->sync(); }

void sac::AutoClicker::typeNumber(uint number) {
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

QString sac::AutoClicker::getConfigPath() {
  QString cfgPath = getConfigFilePath();
  assert(!cfgPath.isEmpty());
  return cfgPath;
}

void sac::AutoClicker::touchFile(QFile &file) {
  if (!file.exists()) {
    bool ret = file.open(QIODevice::WriteOnly);
    if (!ret) {
      throw std::runtime_error(
          std::string("Could not create config file at: ") +
          file.fileName().toStdString() +
          ". Error: " + std::to_string(file.error()));
    }
  }
}

void sac::AutoClicker::syncBindings() {
  _bindings[TOGGLE_LISTEN] = parse(m_config->value(CFGKEY_LISTEN).toString());
  _bindings[TOGGLE_CLICK] = parse(m_config->value(CFGKEY_CLICKMODE).toString());
  _bindings[TOGGLE_MOUSE] = parse(m_config->value(CFGKEY_MOUSEBTN).toString());
}
