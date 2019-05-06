#include "beep.h"

namespace sac {

void beepError() {
  beep(2000, 10);
  beep(0, 10);
  beep(2000, 20);
  beep(0, 10);
  beep(2000, 10);
}

void beepOn() { beep(5000, 100); }

void beepOff() { beep(1000, 50); }

void beepType() { beep(2000, 20); }

} // namespace sac
