#include "beep.h"

void BeepError() {
    Beep(1000, 50);
    Beep(0, 10);
    Beep(1000, 50);
}

void BeepStart() {
    Beep(5000, 200);
}

void BeepListenStart() {
    Beep(5000, 50);
}

void BeepListenStop() {
    Beep(1000, 50);
}

void BeepStop() {
    Beep(200, 200);
}

void BeepType() {
    Beep(2000, 20);
}

void BeepInputLimit() {
    Beep(5000, 50);
}

void BeepSwitchButton() {
    Beep(8000, 50);
}
