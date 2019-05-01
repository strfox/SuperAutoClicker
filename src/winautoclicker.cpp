#include "autoclicker.h"

#include <windows.h>
#include <sstream>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "mainwindow.h"
#include "hook.h"
#include "beep.h"

#define CFGKEYS_AMOUNT 4


namespace sac {

void _autoclickProc(void*); // Forward declaration


typedef struct {
    unsigned int *msInterval;
    bool         *mouseButton;
    void         *hRunMutex;
} autoClickProcArgs_t;


AutoClicker *autoClicker;


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
               throw std::runtime_error(
                       std::string("Could not create config file at: ")
                       + cfgPath.toStdString()
                       + ". Error: "
                       + std::to_string(file.error())
                     );
           }
       }

       m_config = new QSettings(cfgPath, QSettings::IniFormat, this);
    }

    if (m_config->allKeys().size() == 0) {
        // Populate in-memory configuration with default keys and values
        m_config->setValue(CFGKEY_LISTEN    , QString::number(VK_SUBTRACT) + ",0,0,0,0");
        m_config->setValue(CFGKEY_CLICKMODE , QString::number(VK_ADD     ) + ",0,0,0,0");
        m_config->setValue(CFGKEY_MOUSEBTN  , QString::number(VK_DIVIDE  ) + ",0,0,0,0");
        m_config->setValue(CFGKEY_HOLDBTN   , QString::number(VK_DECIMAL ) + ",0,0,0,0");
        m_config->sync();
    }

    // Ensure m_config has the right amount of keys

    const int keysAmount = m_config->allKeys().size();
    if (keysAmount != CFGKEYS_AMOUNT)
    {
        throw std::runtime_error(
                    std::string("Config might be corrupted. Expected config to have ")
                         + std::to_string(CFGKEYS_AMOUNT)
                         + " but instead got "
                         + std::to_string(keysAmount)
                  );
    }

    // Create keycomb_t instances from QSettings

    kb::keycomb_t listenComb   = kb::parseKeyComb(m_config->value(CFGKEY_LISTEN   ).toString());
    kb::keycomb_t clickComb    = kb::parseKeyComb(m_config->value(CFGKEY_CLICKMODE).toString());
    kb::keycomb_t mouseBtnComb = kb::parseKeyComb(m_config->value(CFGKEY_MOUSEBTN ).toString());
    kb::keycomb_t holdBtnComb  = kb::parseKeyComb(m_config->value(CFGKEY_HOLDBTN  ).toString());

    // Assign config keys to hook

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

    if (m_clickMode) { startClickThread(); }
    else             { stopClickThread();  }
}


void AutoClicker::toggleMouseButton() {
    qDebug("sac::toggleMouseButton");
    m_mouseButton = !m_mouseButton;
}


void AutoClicker::toggleHoldButtonMode() {
    qDebug("sac::toggleHoldButtonMode");
    m_holdButtonMode = !m_holdButtonMode;
}


void AutoClicker::saveConfig() {
    m_config->sync();
    qDebug("Config saved");
}


void AutoClicker::startClickThread() {
    if (m_msInterval == 0) {
        beepError();
        assert(mainWindow != nullptr);
        mainWindow->putMsg(QString("Please enter milliseconds interval."));
    } else {
        assert(m_hRunMutex == nullptr);

        m_hRunMutex = CreateMutex(
                    nullptr,  // LPSECURITY_ATTRIBUTES lpMutexAttributes
                    true,     // WINBOOL               bInitialOwner
                    nullptr   // LPCWSTR               lpName
                   );

        autoClickProcArgs_t* arg = new autoClickProcArgs_t;

        arg->msInterval  = &m_msInterval;
        arg->mouseButton = &m_mouseButton;
        arg->hRunMutex   =  m_hRunMutex;

        uintptr_t ptr =
                _beginthread(
                    _autoclickProc, // void (* _StartAddress)(void *) __attribute__((cdecl))
                    0,              // unsigned int  _StackSize
                    arg             // void         *_ArgList
                   );

        m_hThread = reinterpret_cast<void*>(ptr);
    }
}


void AutoClicker::stopClickThread() {
    ReleaseMutex(m_hRunMutex);
    WaitForSingleObject(
                reinterpret_cast<HANDLE>(m_hThread), // HANDLE hHandle
                1000                                 // DWORD  dwMilliseconds
               );
    m_hRunMutex = nullptr;
    m_hThread = nullptr;
}

void AutoClicker::typeNumber(uint number) {
    if (number < 0U || number > 9U) {
        throw std::invalid_argument(
                    std::string("expected number to be in range 0..9 but number was ")
                         + std::to_string(number)
              );
    }
    assert(mainWindow != nullptr);
    mainWindow->putMsg(QString::number(number));


}


void _autoclickProc(/* autoClickProcArgs_t */ void* arg) {
    autoClickProcArgs_t *props = static_cast<autoClickProcArgs_t *>(arg);

    do {
        assert(props              != nullptr);
        assert(props->msInterval  != nullptr);
        assert(props->mouseButton != nullptr);
        assert(props->hRunMutex   != nullptr);

        INPUT input;
        input.type           = INPUT_MOUSE;
        input.mi.dx          = 0;
        input.mi.dy          = 0;
        input.mi.mouseData   = 0;
        input.mi.dwExtraInfo = 0UL;
        input.mi.time        = 0;

        if (props->mouseButton)
             { input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN  | MOUSEEVENTF_LEFTUP);  }
        else { input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP); }

        SendInput(1, &input, sizeof(INPUT));

    } while (WaitForSingleObject(props->hRunMutex, *props->msInterval) == WAIT_TIMEOUT);

    delete props;
}


} // namespace sac
