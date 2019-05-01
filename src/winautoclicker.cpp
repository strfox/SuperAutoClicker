#include "autoclicker.h"

#include <windows.h>
#include <math.h>
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
#include "util.h"


namespace sac {


void _autoclickProc(void*);       // Forward declaration
void _disableMouseBtnProc(void*); // Forward declaration


typedef struct {
    unsigned int  *msInterval;
    mousebtn_t    *mouseButton;
    void         **hRunMutex;
    // bool          *holdButton;
} autoClickProcArgs_t;


typedef struct {
    mousebtn_t *mousebtn;
    void       *hDisableMouseBtnMutex;
} disableMouseBtnProcArgs_t;



AutoClicker *_autoClicker;
AutoClicker *autoClicker() {
    assert(_autoClicker != nullptr);
    return _autoClicker;
}



AutoClicker::AutoClicker()
{
    // Set up config

    QString cfgPath = getConfigFilePath();
    assert(!cfgPath.isEmpty());
    QFile file(cfgPath);

    // Touch config file if it doesn't exist

    if (!file.exists())
    {
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

    if (m_config->allKeys().size() == 0)
    {
        // Populate in-memory configuration with default keys and values

        m_config->setValue(CFGKEY_LISTEN    , QString::number(VK_SUBTRACT) + ",0,0,0,0");
        m_config->setValue(CFGKEY_CLICKMODE , QString::number(VK_ADD     ) + ",0,0,0,0");
        m_config->setValue(CFGKEY_MOUSEBTN  , QString::number(VK_DIVIDE  ) + ",0,0,0,0");
        // m_config->setValue(CFGKEY_HOLDBTN   , QString::number(VK_DECIMAL ) + ",0,0,0,0");
        m_config->sync();
    }

    // Ensure m_config has the right amount of keys

    const int keysAmount = m_config->allKeys().size();
    if (keysAmount != CFGKEYS_AMOUNT)
    {
        QString msg =
               QString("Expected config to have ")
             + QString::number(CFGKEYS_AMOUNT)
             + " keys but it has "
             + QString::number(keysAmount)
             + " instead. Please edit your configuration at \""
             + getConfigFilePath()
             + "\" and restart the program.";
        QMessageBox::critical(nullptr, tr("Configuration Error"), msg);
        throw std::runtime_error(msg.toStdString());
    }

    // Create keycomb_t instances from QSettings

    kb::keycomb_t listenComb   = kb::parseKeyComb(m_config->value(CFGKEY_LISTEN   ).toString());
    kb::keycomb_t clickComb    = kb::parseKeyComb(m_config->value(CFGKEY_CLICKMODE).toString());
    kb::keycomb_t mouseBtnComb = kb::parseKeyComb(m_config->value(CFGKEY_MOUSEBTN ).toString());
    // kb::keycomb_t holdBtnComb  = kb::parseKeyComb(m_config->value(CFGKEY_HOLDBTN  ).toString());

    // Assign config keys to hook

    hook::setBind(TOGGLE_LISTEN, listenComb  );
    hook::setBind(TOGGLE_CLICK , clickComb   );
    hook::setBind(TOGGLE_MOUSE , mouseBtnComb);
    // hook::setBind(TOGGLE_HOLD  , holdBtnComb );
}


AutoClicker::~AutoClicker() {
    delete m_config;
}


QString AutoClicker::getConfigFilePath() {
    QString path = QDir::homePath();
    path = path.append("/SuperAutoClicker Configuration.ini");
    if (path.isEmpty())
    {
        throw std::runtime_error("Could not find file path for config file");
    }
    else return path;
}


void AutoClicker::refreshMainWindow() {
    mainWindow()->refresh();
}


void AutoClicker::toggleListenMode() {
   if (m_listenMode)
   {
       if (m_msInput == 0)
       {
           mainWindow()->putMsg(tr("Please enter a number."));
       }
       else
       {
           assert(m_msInput    >  0);
           m_msInterval = m_msInput;
           m_msInput    = 0;
           assert(m_msInput    == 0);
           assert(m_msInterval >  0);
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
    assert(m_msInterval == 0 || m_msInterval > 0); // >= expansion prevents -Wtype-limits
    if (m_listenMode) {
        toggleListenMode();
    }

    MainWindow *_w = mainWindow();

    if (m_msInterval == 0)
    {
        beepError();
        _w->putMsg(QString(tr("Please enter milliseconds interval.")));
    }
    else
    {
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



/* void AutoClicker::toggleHoldButtonMode() {
    m_holdButtonMode = !m_holdButtonMode;
    hook::shouldDisableMouseBtn = m_holdButtonMode;
    refreshMainWindow();
} */


void AutoClicker::saveConfig() {
    m_config->sync();
}

void _startThread(
        void **hMutex_out,
        void **hThread_out,
        void (* _hookProc)(void *) __attribute__((cdecl)),
        void *procArg
    ) {
    assert(*hMutex_out  == nullptr);
    assert(*hThread_out == nullptr);

    *hMutex_out = CreateMutex(
                nullptr,  // LPSECURITY_ATTRIBUTES lpMutexAttributes
                true,     // WINBOOL               bInitialOwner
                nullptr   // LPCWSTR               lpName
              );
    uintptr_t ptr =
            _beginthread(
                _hookProc,
                0,
                procArg
                );
    *hThread_out = reinterpret_cast<void*>(ptr);
}

void _stopThread(void **hMutex_out, void **hThread_out) {
    assert(*hMutex_out  != nullptr);
    assert(*hThread_out != nullptr);

    ReleaseMutex(*hMutex_out);
    WaitForSingleObject(
                reinterpret_cast<HANDLE>(hThread_out),
                1000);

    *hMutex_out  = nullptr;
    *hThread_out = nullptr;
}

void AutoClicker::startClickThread() {
    autoClickProcArgs_t* arg = new autoClickProcArgs_t;

    arg->msInterval  = &m_msInterval;
    arg->mouseButton = &m_mouseButton;
    arg->hRunMutex   = &m_hClickMutex;
    // arg->holdButton  = &m_holdButtonMode;

    _startThread(&m_hClickMutex, &m_hClickThread, _autoclickProc, arg);
}


void AutoClicker::stopClickThread() {
    _stopThread(&m_hClickMutex, &m_hClickThread);
}


void AutoClicker::typeNumber(uint number) {
    assert(number == 0 || number > 0); // >= expansion prevents -Wtype-limits
    if (number > 9U) {
        throw std::invalid_argument(
                    std::string("expected number to be in range 0..9 but number was ")
                         + std::to_string(number));
    }

    if (m_listenMode)
    {
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


void _autoclickProc(/* autoClickProcArgs_t */ void *arg) {
    autoClickProcArgs_t *props = static_cast<autoClickProcArgs_t *>(arg);

    do {
        assert(props              != nullptr);
        assert(props->msInterval  != nullptr);
        assert(props->mouseButton != nullptr);
        assert(props->hRunMutex   != nullptr);
        assert(*props->hRunMutex  != nullptr);

        INPUT input;
        input.type           = INPUT_MOUSE;
        input.mi.dx          = 0;
        input.mi.dy          = 0;
        input.mi.mouseData   = 0;
        input.mi.dwExtraInfo = 0UL;
        input.mi.time        = 0;

        switch (*props->mouseButton) {
        case MOUSE1:
            input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN  | MOUSEEVENTF_LEFTUP);
            break;
        case MOUSE2:
            input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
            break;
        }

        bool doClick = true;

        /* if (*props->holdButton)
        {
            int key = -1;
            // I use a switch statement instead of a ternary because
            // if I add more values to the mouse enum, then this will have a warning.
            switch (*props->mouseButton) {
            case MOUSE1:
                key = VK_LEFT;
                break;
            case MOUSE2:
                key = VK_RIGHT;
                break;
            }
            assert(key != -1);
            doClick = GetKeyState(key) < 0;
        } */

        if (doClick) {
            SendInput(1, &input, sizeof(INPUT));
        }
    } while (WaitForSingleObject(*props->hRunMutex, *props->msInterval) == WAIT_TIMEOUT);

    delete props;
}

} // namespace sac
