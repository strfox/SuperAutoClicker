#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QString>

#include "types.h"

namespace sac {
namespace kb {

QString keycombstr(keycomb_t);

keycomb_t parse(QString);

QString stringify(keycomb_t comb);

} // namespace kb
} // namespace sac

#endif // KEYBOARD_H
