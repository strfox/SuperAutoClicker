#include "beep.h"

#include <windows.h>

namespace sac {

void beep(unsigned int frequency, unsigned int duration) {
    Beep(frequency, duration);
}

} // namespace sac
