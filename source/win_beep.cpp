#include "beep.h"

#include <Windows.h>

void sac::beep(unsigned int frequency, unsigned int duration) {
  Beep(frequency, duration);
}
