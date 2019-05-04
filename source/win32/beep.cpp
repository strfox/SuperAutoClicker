#include "beep.h"

#include <Windows.h>

namespace sac {

void beep(unsigned int frequency, unsigned int duration) {
  Beep(frequency, duration);
}

} // namespace sac
