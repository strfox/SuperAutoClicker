#ifndef TYPES_H
#define TYPES_H

#include <string>

namespace sac {


typedef enum action_t {
    TOGGLE_CLICK,
    TOGGLE_LISTEN,
    TOGGLE_MOUSE,
    // TOGGLE_HOLD
} action_t;


static std::string actionnames[] = {
    "TOGGLE_CLICK",
    "TOGGLE_LISTEN",
    "TOGGLE_MOUSE",
    // "TOGGLE_HOLD"
};


typedef enum {
    MOUSE1, MOUSE2
} mousebtn_t;


namespace kb {

typedef unsigned long vkcode_t;

typedef struct keycomb_t {
    vkcode_t vkCode;
    bool meta;
    bool ctrl;
    bool alt;
    bool shift;
} keycomb_t;

} // namespace kb
} // namespace sac

#endif // TYPES_H
