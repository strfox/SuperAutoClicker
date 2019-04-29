#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <stdbool.h>
#include <stdio.h>

#include <QString>

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

typedef unsigned long VKCODE;

typedef struct keycomb_t {
    VKCODE vkCode;
    bool ctrl;
    bool shift;
    bool alt;
    bool meta;
} keycomb_t;


int createHook();


int releaseHook();


void setBind(action_t, keycomb_t);


QString getLastError();


keycomb_t getKeyCombinationFor(action_t action);


QString getStringNameFor(keycomb_t keycomb);


} // namespace hook
} // namespace sac

#endif // SAC_HOOK_H
