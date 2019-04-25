#ifndef AUTOCLICKER_CLICK_H
#define AUTOCLICKER_CLICK_H

#include <windows.h>

typedef enum {
    MOUSEBTN_LEFT = 0,
    MOUSEBTN_RIGHT = 1
} mousebtn_t;

BOOL StartClickThread(int ms);

void StopClickThread();

void SetMsInterval(int ms);

void SetMouseButton(mousebtn_t);

mousebtn_t GetMouseButton();

#endif //AUTOCLICKER_CLICK_H
