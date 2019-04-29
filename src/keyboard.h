#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QString>

namespace sac {
namespace kb {

typedef unsigned long vkcode_t;

typedef struct keycomb_t {
    vkcode_t vkCode;
    bool ctrl;
    bool shift;
    bool alt;
    bool meta;
} keycomb_t;

QString getStringNameFor(keycomb_t);

keycomb_t parseKeyComb(QString);

} // namespace kb
} // namespace sac

#endif // KEYBOARD_H
