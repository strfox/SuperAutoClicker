#ifndef SAC_HOOK_H
#define SAC_HOOK_H

#include <QString>
#include <stdbool.h>
#include <stdio.h>

#include "keyboard.h"
#include "types.h"

namespace sac {
namespace hook {

extern kb::keycomb_t bindings[];

void createKbdHook();

void releaseKbdHook();

void setBind(action_t, kb::keycomb_t);

kb::keycomb_t getKeyCombinationFor(action_t action);

QString getLastError();

void createMouseHook();

void releaseMouseHook();

} // namespace hook
} // namespace sac

#endif // SAC_HOOK_H
