#ifndef WINHOOK_H
#define WINHOOK_H

#include <QString>

namespace Hook {

typedef enum {
    TOGGLE_CLICK,
    TOGGLE_LISTEN,
    TOGGLE_MOUSE
} actionkey_t;

int createHook();
int releaseHook();
void setBind(actionkey_t actionkey, unsigned long keyCode);
QString getLastError();

}

#endif // WINHOOK_H
