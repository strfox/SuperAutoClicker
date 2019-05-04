#ifndef TYPES_H
#define TYPES_H

#include <string>

namespace sac {

typedef enum action_t { TOGGLE_CLICK, TOGGLE_LISTEN, TOGGLE_MOUSE } action_t;

static std::string actionnames[] = {"TOGGLE_CLICK", "TOGGLE_LISTEN",
                                    "TOGGLE_MOUSE"};

typedef enum { MOUSE1, MOUSE2 } mousebtn_t;

namespace kb {

typedef unsigned long vkcode_t;

typedef struct keycomb_t {
  vkcode_t vkCode;
  bool meta;
  bool ctrl;
  bool alt;
  bool shift;

  bool operator==(const keycomb_t &other) {
    return vkCode == other.vkCode && meta == other.meta && ctrl == other.ctrl &&
           alt == other.alt && shift == other.shift;
  }
} keycomb_t;

} // namespace kb
} // namespace sac

#endif // TYPES_H
