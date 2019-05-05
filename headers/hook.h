#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <QString>
#include <stdbool.h>
#include <stdio.h>

#include "keyboard.h"
#include "types.h"

namespace sac {
namespace hook {

void createKbdHook();

void releaseKbdHook();

QString getLastError();

void createMouseHook();

void releaseMouseHook();

} // namespace hook
} // namespace sac

#endif // SAC_HOOK_H
