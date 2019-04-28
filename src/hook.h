#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <QString>

namespace Hook {


typedef unsigned int action_t;
const action_t
    TOGGLE_CLICK = 1,
    TOGGLE_LISTEN = 2,
    TOGGLE_MOUSE = 3;


static std::string actionnames[] = {
    "TOGGLE_CLICK",
    "TOGGLE_LISTEN",
    "TOGGLE_MOUSE"
};


typedef struct keycomb_t {
    unsigned long vkCode;
    bool ctrl;
    bool shift;
    bool alt;
    bool meta;

    bool operator==(const keycomb_t &o) const {
        return vkCode == o.vkCode && ctrl == o.ctrl && shift == o.shift && alt == o.alt && meta == o.meta;
    }

    bool operator<(const keycomb_t &o) const {
        return     (!meta && o.meta)
                || (!alt && o.alt)
                || (!shift && o.shift)
                || (!ctrl && o.ctrl)
                || (vkCode < o.vkCode);
    }
} keycomb_t;


int createHook();


int releaseHook();


void setBind(action_t, keycomb_t);


QString getLastError();


keycomb_t getKeyCombinationFor(action_t action);


QString getStringNameFor(keycomb_t);


}

#endif // SAC_HOOK_H
