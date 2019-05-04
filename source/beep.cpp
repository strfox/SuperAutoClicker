#include "beep.h"

namespace sac {

void beepError() {
  beep(2000, 10);
  beep(0, 10);
  beep(2000, 20);
  beep(0, 10);
  beep(2000, 10);
}

void beepClickModeOn() { beep(5000, 200); }

void beepClickModeOff() { beep(200, 200); }

void beepListenModeOn() { beep(5000, 100); }

void beepListenModeOff() { beep(1000, 50); }

void beepTypeMs() { beep(2000, 20); }

void beepMsDigitLimitReached() { beep(5000, 50); }

void beepMouseButtonToggle() { beep(8000, 50); }

/* void beepHoldModeOn() {
    beep(500, 50);
    beep(0, 50);
    beep(500, 50);
} */

/* void beepHoldModeOff() {
    beep(200, 50);
    beep(0, 50);
    beep(200, 50);
} */

} // namespace sac
