#ifndef BEEP_H
#define BEEP_H

namespace sac {

void beep(unsigned int frequency, unsigned int duration);

void beepError();
void beepOn();
void beepOff();
void beepType();

} // namespace sac

#endif // BEEP_H
