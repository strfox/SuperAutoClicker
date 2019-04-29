#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include <QString>
#include <QSettings>
#include <QObject>

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

private:
    QString getConfigFilePath();

    QSettings *m_config;

}; // class AutoCLicker

} // namespace sac

#endif // AUTOCLICKER_H
