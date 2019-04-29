#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <stdbool.h>
#include <stdio.h>
#include <QString>

#include "keyboard.h"

namespace sac {
namespace hook {

typedef enum action_t {
    TOGGLE_CLICK,
    TOGGLE_LISTEN,
    TOGGLE_MOUSE,
    TOGGLE_HOLD
} action_t;


static std::string actionnames[] = {
    "TOGGLE_CLICK",
    "TOGGLE_LISTEN",
    "TOGGLE_MOUSE",
    "TOGGLE_HOLD"
};

extern kb::keycomb_t bindings[];

int createHook();

int releaseHook();

void setBind(action_t, kb::keycomb_t);

kb::keycomb_t getKeyCombinationFor(action_t action);

QString getLastError();


} // namespace hook
} // namespace sac

#endif // SAC_HOOK_H
