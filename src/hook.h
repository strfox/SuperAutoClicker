#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <stdbool.h>
#include <stdio.h>

#include <QString>

typedef enum action_t {
    TOGGLE_CLICK = 1,
    TOGGLE_LISTEN = 2,
    TOGGLE_MOUSE = 3
} action_t;


static char* actionnames[] = {
    "TOGGLE_CLICK",
    "TOGGLE_LISTEN",
    "TOGGLE_MOUSE"
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


#endif // SAC_HOOK_H
