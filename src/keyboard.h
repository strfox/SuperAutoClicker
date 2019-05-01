#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QString>

namespace sac {
namespace kb {

typedef unsigned long vkcode_t;

typedef struct keycomb_t {
    vkcode_t vkCode;
    bool meta;
    bool ctrl;
    bool alt;
    bool shift;
} keycomb_t;

QString keycombstr(keycomb_t);

keycomb_t parseKeyComb(QString);

QString stringify(keycomb_t comb);

} // namespace kb
} // namespace sac

#endif // KEYBOARD_H
