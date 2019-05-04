#include "keyboard.h"

#include <QString>
#include <QStringList>
#include <QTextStream>

using sac::kb::keycomb_t;

/**
 * Parses a string into a keycomb_t struct. The string should follow the format:
 *
 *  0   1   2   3   4
 * "INT,INT,INT,INT,INT"
 *
 * Where each INT at n position means:
 *  0: Virtual key code [range: 0-256]
 *  1: META  modifier   [range: 0-1  ]
 *  2: CTRL  modifier   [range: 0-1  ]
 *  3: ALT   modifier   [range: 0-1  ]
 *  4: SHIFT modifier   [range: 0-1  ]
 *
 * INTs 1, 2, 3, 4 are boolean ints.
 *
 * @param str String to parse
 * @return Parsed keyboard combination
 */
keycomb_t sac::kb::parse(QString str) {
  QStringList tokens = str.split(QString(","));

  if (tokens.size() != 5) {
    throw std::runtime_error(
        std::string(
            "Expected 5 tokens while parsing key combination but got ") +
        std::to_string(tokens.size()) + ". Input: " + qUtf8Printable(str));
  }

  uint vkCode = tokens[0].toUInt();
  bool meta = tokens[1].toInt();
  bool ctrl = tokens[2].toInt();
  bool alt = tokens[3].toInt();
  bool shift = tokens[4].toInt();

  return {vkCode, meta, ctrl, alt, shift};
}

QString sac::kb::stringify(keycomb_t comb) {
  QString str;
  QTextStream s(&str);
  s << comb.vkCode << "," << comb.meta << "," << comb.ctrl << "," << comb.alt
    << "," << comb.shift;
  return str;
}
