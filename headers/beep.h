#ifndef BEEP_H
#define BEEP_H

namespace sac {

void beep(unsigned int frequency, unsigned int duration);

void beepError();
void beepClickModeOn();
void beepClickModeOff();
void beepListenModeOn();
void beepListenModeOff();
void beepTypeMs();
void beepMsDigitLimitReached();
void beepMouseButtonToggle();
// void beepHoldModeOn();
// void beepHoldModeOff();

}

#endif // BEEP_H
