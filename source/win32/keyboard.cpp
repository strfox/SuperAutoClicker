#include "keyboard.h"

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <Windows.h>

#include "win32/winkeys.h"

namespace sac {
namespace kb {

QString keycombstr(keycomb_t keycomb) {
  QString keyname;

  switch (keycomb.vkCode) {
  // Auto-generated code
  case (VK_ABNT_C1):
    keyname = QString("ABNT_C1");
    break;
  case (VK_ABNT_C2):
    keyname = QString("ABNT_C2");
    break;
  case (VK_ADD):
    keyname = QString("NUMPAD+");
    break;
  case (VK_ATTN):
    keyname = QString("ATTN");
    break;
  case (VK_BACK):
    keyname = QString("BACK");
    break;
  case (VK_CANCEL):
    keyname = QString("CANCEL");
    break;
  case (VK_CLEAR):
    keyname = QString("CLEAR");
    break;
  case (VK_CRSEL):
    keyname = QString("CRSEL");
    break;
  case (VK_DECIMAL):
    keyname = QString("NUMPAD.");
    break;
  case (VK_DIVIDE):
    keyname = QString("NUMPAD/");
    break;
  case (VK_EREOF):
    keyname = QString("EREOF");
    break;
  case (VK_ESCAPE):
    keyname = QString("ESCAPE");
    break;
  case (VK_EXECUTE):
    keyname = QString("EXECUTE");
    break;
  case (VK_EXSEL):
    keyname = QString("EXSEL");
    break;
  case (VK_ICO_CLEAR):
    keyname = QString("ICO_CLEAR");
    break;
  case (VK_ICO_HELP):
    keyname = QString("ICO_HELP");
    break;
  case (VK_KEY_0):
    keyname = QString("KEY_0");
    break;
  case (VK_KEY_1):
    keyname = QString("KEY_1");
    break;
  case (VK_KEY_2):
    keyname = QString("KEY_2");
    break;
  case (VK_KEY_3):
    keyname = QString("KEY_3");
    break;
  case (VK_KEY_4):
    keyname = QString("KEY_4");
    break;
  case (VK_KEY_5):
    keyname = QString("KEY_5");
    break;
  case (VK_KEY_6):
    keyname = QString("KEY_6");
    break;
  case (VK_KEY_7):
    keyname = QString("KEY_7");
    break;
  case (VK_KEY_8):
    keyname = QString("KEY_8");
    break;
  case (VK_KEY_9):
    keyname = QString("KEY_9");
    break;
  case (VK_KEY_A):
    keyname = QString("KEY_A");
    break;
  case (VK_KEY_B):
    keyname = QString("KEY_B");
    break;
  case (VK_KEY_C):
    keyname = QString("KEY_C");
    break;
  case (VK_KEY_D):
    keyname = QString("KEY_D");
    break;
  case (VK_KEY_E):
    keyname = QString("KEY_E");
    break;
  case (VK_KEY_F):
    keyname = QString("KEY_F");
    break;
  case (VK_KEY_G):
    keyname = QString("KEY_G");
    break;
  case (VK_KEY_H):
    keyname = QString("KEY_H");
    break;
  case (VK_KEY_I):
    keyname = QString("KEY_I");
    break;
  case (VK_KEY_J):
    keyname = QString("KEY_J");
    break;
  case (VK_KEY_K):
    keyname = QString("KEY_K");
    break;
  case (VK_KEY_L):
    keyname = QString("KEY_L");
    break;
  case (VK_KEY_M):
    keyname = QString("KEY_M");
    break;
  case (VK_KEY_N):
    keyname = QString("KEY_N");
    break;
  case (VK_KEY_O):
    keyname = QString("KEY_O");
    break;
  case (VK_KEY_P):
    keyname = QString("KEY_P");
    break;
  case (VK_KEY_Q):
    keyname = QString("KEY_Q");
    break;
  case (VK_KEY_R):
    keyname = QString("KEY_R");
    break;
  case (VK_KEY_S):
    keyname = QString("KEY_S");
    break;
  case (VK_KEY_T):
    keyname = QString("KEY_T");
    break;
  case (VK_KEY_U):
    keyname = QString("KEY_U");
    break;
  case (VK_KEY_V):
    keyname = QString("KEY_V");
    break;
  case (VK_KEY_W):
    keyname = QString("KEY_W");
    break;
  case (VK_KEY_X):
    keyname = QString("KEY_X");
    break;
  case (VK_KEY_Y):
    keyname = QString("KEY_Y");
    break;
  case (VK_KEY_Z):
    keyname = QString("KEY_Z");
    break;
  case (VK_MULTIPLY):
    keyname = QString("NUMPAD*");
    break;
  case (VK_NONAME):
    keyname = QString("NONAME");
    break;
  case (VK_NUMPAD0):
    keyname = QString("NUMPAD0");
    break;
  case (VK_NUMPAD1):
    keyname = QString("NUMPAD1");
    break;
  case (VK_NUMPAD2):
    keyname = QString("NUMPAD2");
    break;
  case (VK_NUMPAD3):
    keyname = QString("NUMPAD3");
    break;
  case (VK_NUMPAD4):
    keyname = QString("NUMPAD4");
    break;
  case (VK_NUMPAD5):
    keyname = QString("NUMPAD5");
    break;
  case (VK_NUMPAD6):
    keyname = QString("NUMPAD6");
    break;
  case (VK_NUMPAD7):
    keyname = QString("NUMPAD7");
    break;
  case (VK_NUMPAD8):
    keyname = QString("NUMPAD8");
    break;
  case (VK_NUMPAD9):
    keyname = QString("NUMPAD9");
    break;
  case (VK_OEM_1):
    keyname = QString("OEM_1");
    break;
  case (VK_OEM_102):
    keyname = QString("OEM_102");
    break;
  case (VK_OEM_2):
    keyname = QString("OEM_2");
    break;
  case (VK_OEM_3):
    keyname = QString("OEM_3");
    break;
  case (VK_OEM_4):
    keyname = QString("OEM_4");
    break;
  case (VK_OEM_5):
    keyname = QString("OEM_5");
    break;
  case (VK_OEM_6):
    keyname = QString("OEM_6");
    break;
  case (VK_OEM_7):
    keyname = QString("OEM_7");
    break;
  case (VK_OEM_8):
    keyname = QString("OEM_8");
    break;
  case (VK_OEM_ATTN):
    keyname = QString("OEM_ATTN");
    break;
  case (VK_OEM_AUTO):
    keyname = QString("OEM_AUTO");
    break;
  case (VK_OEM_AX):
    keyname = QString("OEM_AX");
    break;
  case (VK_OEM_BACKTAB):
    keyname = QString("OEM_BACKTAB");
    break;
  case (VK_OEM_CLEAR):
    keyname = QString("OEM_CLEAR");
    break;
  case (VK_OEM_COMMA):
    keyname = QString("OEM_COMMA");
    break;
  case (VK_OEM_COPY):
    keyname = QString("OEM_COPY");
    break;
  case (VK_OEM_CUSEL):
    keyname = QString("OEM_CUSEL");
    break;
  case (VK_OEM_ENLW):
    keyname = QString("OEM_ENLW");
    break;
  case (VK_OEM_FINISH):
    keyname = QString("OEM_FINISH");
    break;
  case (VK_OEM_FJ_LOYA):
    keyname = QString("OEM_FJ_LOYA");
    break;
  case (VK_OEM_FJ_MASSHOU):
    keyname = QString("OEM_FJ_MASSHOU");
    break;
  case (VK_OEM_FJ_ROYA):
    keyname = QString("OEM_FJ_ROYA");
    break;
  case (VK_OEM_FJ_TOUROKU):
    keyname = QString("OEM_FJ_TOUROKU");
    break;
  case (VK_OEM_JUMP):
    keyname = QString("OEM_JUMP");
    break;
  case (VK_OEM_MINUS):
    keyname = QString("OEM_MINUS");
    break;
  case (VK_OEM_PA1):
    keyname = QString("OEM_PA1");
    break;
  case (VK_OEM_PA2):
    keyname = QString("OEM_PA2");
    break;
  case (VK_OEM_PA3):
    keyname = QString("OEM_PA3");
    break;
  case (VK_OEM_PERIOD):
    keyname = QString("OEM_PERIOD");
    break;
  case (VK_OEM_PLUS):
    keyname = QString("OEM_PLUS");
    break;
  case (VK_OEM_RESET):
    keyname = QString("OEM_RESET");
    break;
  case (VK_OEM_WSCTRL):
    keyname = QString("OEM_WSCTRL");
    break;
  case (VK_PA1):
    keyname = QString("PA1");
    break;
  case (VK_PACKET):
    keyname = QString("PACKET");
    break;
  case (VK_PLAY):
    keyname = QString("PLAY");
    break;
  case (VK_PROCESSKEY):
    keyname = QString("PROCESSKEY");
    break;
  case (VK_RETURN):
    keyname = QString("NUMPAD_ENTER");
    break;
  case (VK_SELECT):
    keyname = QString("SELECT");
    break;
  case (VK_SEPARATOR):
    keyname = QString("SEPARATOR");
    break;
  case (VK_SPACE):
    keyname = QString("SPACE");
    break;
  case (VK_SUBTRACT):
    keyname = QString("NUMPAD-");
    break;
  case (VK_TAB):
    keyname = QString("TAB");
    break;
  case (VK_ZOOM):
    keyname = QString("ZOOM");
    break;
  case (0xFF):
    keyname = QString("_none_");
    break;
  case (VK_ACCEPT):
    keyname = QString("ACCEPT");
    break;
  case (VK_APPS):
    keyname = QString("APPS");
    break;
  case (VK_BROWSER_BACK):
    keyname = QString("BROWSER_BACK");
    break;
  case (VK_BROWSER_FAVORITES):
    keyname = QString("BROWSER_FAVORITES");
    break;
  case (VK_BROWSER_FORWARD):
    keyname = QString("BROWSER_FORWARD");
    break;
  case (VK_BROWSER_HOME):
    keyname = QString("BROWSER_HOME");
    break;
  case (VK_BROWSER_REFRESH):
    keyname = QString("BROWSER_REFRESH");
    break;
  case (VK_BROWSER_SEARCH):
    keyname = QString("BROWSER_SEARCH");
    break;
  case (VK_BROWSER_STOP):
    keyname = QString("BROWSER_STOP");
    break;
  case (VK_CAPITAL):
    keyname = QString("CAPITAL");
    break;
  case (VK_CONVERT):
    keyname = QString("CONVERT");
    break;
  case (VK_DELETE):
    keyname = QString("DELETE");
    break;
  case (VK_DOWN):
    keyname = QString("DOWN");
    break;
  case (VK_END):
    keyname = QString("END");
    break;
  case (VK_F1):
    keyname = QString("F1");
    break;
  case (VK_F10):
    keyname = QString("F10");
    break;
  case (VK_F11):
    keyname = QString("F11");
    break;
  case (VK_F12):
    keyname = QString("F12");
    break;
  case (VK_F13):
    keyname = QString("F13");
    break;
  case (VK_F14):
    keyname = QString("F14");
    break;
  case (VK_F15):
    keyname = QString("F15");
    break;
  case (VK_F16):
    keyname = QString("F16");
    break;
  case (VK_F17):
    keyname = QString("F17");
    break;
  case (VK_F18):
    keyname = QString("F18");
    break;
  case (VK_F19):
    keyname = QString("F19");
    break;
  case (VK_F2):
    keyname = QString("F2");
    break;
  case (VK_F20):
    keyname = QString("F20");
    break;
  case (VK_F21):
    keyname = QString("F21");
    break;
  case (VK_F22):
    keyname = QString("F22");
    break;
  case (VK_F23):
    keyname = QString("F23");
    break;
  case (VK_F24):
    keyname = QString("F24");
    break;
  case (VK_F3):
    keyname = QString("F3");
    break;
  case (VK_F4):
    keyname = QString("F4");
    break;
  case (VK_F5):
    keyname = QString("F5");
    break;
  case (VK_F6):
    keyname = QString("F6");
    break;
  case (VK_F7):
    keyname = QString("F7");
    break;
  case (VK_F8):
    keyname = QString("F8");
    break;
  case (VK_F9):
    keyname = QString("F9");
    break;
  case (VK_FINAL):
    keyname = QString("FINAL");
    break;
  case (VK_HELP):
    keyname = QString("HELP");
    break;
  case (VK_HOME):
    keyname = QString("HOME");
    break;
  case (VK_ICO_00):
    keyname = QString("ICO_00");
    break;
  case (VK_INSERT):
    keyname = QString("INSERT");
    break;
  case (VK_JUNJA):
    keyname = QString("JUNJA");
    break;
  case (VK_KANA):
    keyname = QString("KANA");
    break;
  case (VK_KANJI):
    keyname = QString("KANJI");
    break;
  case (VK_LAUNCH_APP1):
    keyname = QString("LAUNCH_APP1");
    break;
  case (VK_LAUNCH_APP2):
    keyname = QString("LAUNCH_APP2");
    break;
  case (VK_LAUNCH_MAIL):
    keyname = QString("LAUNCH_MAIL");
    break;
  case (VK_LAUNCH_MEDIA_SELECT):
    keyname = QString("LAUNCH_MEDIA_SELECT");
    break;
  case (VK_LBUTTON):
    keyname = QString("LBUTTON");
    break;
  case (VK_LCONTROL):
    keyname = QString("LCONTROL");
    break;
  case (VK_LEFT):
    keyname = QString("LEFT");
    break;
  case (VK_LMENU):
    keyname = QString("LMENU");
    break;
  case (VK_LSHIFT):
    keyname = QString("LSHIFT");
    break;
  case (VK_LWIN):
    keyname = QString("LWIN");
    break;
  case (VK_MBUTTON):
    keyname = QString("MBUTTON");
    break;
  case (VK_MEDIA_NEXT_TRACK):
    keyname = QString("MEDIA_NEXT_TRACK");
    break;
  case (VK_MEDIA_PLAY_PAUSE):
    keyname = QString("MEDIA_PLAY_PAUSE");
    break;
  case (VK_MEDIA_PREV_TRACK):
    keyname = QString("MEDIA_PREV_TRACK");
    break;
  case (VK_MEDIA_STOP):
    keyname = QString("MEDIA_STOP");
    break;
  case (VK_MODECHANGE):
    keyname = QString("MODECHANGE");
    break;
  case (VK_NEXT):
    keyname = QString("NEXT");
    break;
  case (VK_NONCONVERT):
    keyname = QString("NONCONVERT");
    break;
  case (VK_NUMLOCK):
    keyname = QString("NUMLOCK");
    break;
  case (VK_OEM_FJ_JISHO):
    keyname = QString("OEM_FJ_JISHO");
    break;
  case (VK_PAUSE):
    keyname = QString("PAUSE");
    break;
  case (VK_PRINT):
    keyname = QString("PRINT");
    break;
  case (VK_PRIOR):
    keyname = QString("PRIOR");
    break;
  case (VK_RBUTTON):
    keyname = QString("RBUTTON");
    break;
  case (VK_RCONTROL):
    keyname = QString("RCONTROL");
    break;
  case (VK_RIGHT):
    keyname = QString("RIGHT");
    break;
  case (VK_RMENU):
    keyname = QString("RMENU");
    break;
  case (VK_RSHIFT):
    keyname = QString("RSHIFT");
    break;
  case (VK_RWIN):
    keyname = QString("RWIN");
    break;
  case (VK_SCROLL):
    keyname = QString("SCROLL");
    break;
  case (VK_SLEEP):
    keyname = QString("SLEEP");
    break;
  case (VK_SNAPSHOT):
    keyname = QString("SNAPSHOT");
    break;
  case (VK_UP):
    keyname = QString("UP");
    break;
  case (VK_VOLUME_DOWN):
    keyname = QString("VOLUME_DOWN");
    break;
  case (VK_VOLUME_MUTE):
    keyname = QString("VOLUME_MUTE");
    break;
  case (VK_VOLUME_UP):
    keyname = QString("VOLUME_UP");
    break;
  case (VK_XBUTTON1):
    keyname = QString("XBUTTON1");
    break;
  case (VK_XBUTTON2):
    keyname = QString("XBUTTON2");
    break;
  default:
    keyname =
        QString("UNKNOWN (0x%1)").arg(QString::number(keycomb.vkCode, 16));
    break;
  }

  QString strComb;
  QTextStream s(&strComb);

  if (keycomb.meta)
    s << "WIN ";
  if (keycomb.ctrl)
    s << "CTRL ";
  if (keycomb.alt)
    s << "ALT ";
  if (keycomb.shift)
    s << "SHIFT ";

  s << keyname;
  return strComb;
}

/**
 * @brief parseKeyComb
 *
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
keycomb_t parseKeyComb(QString str) {
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

QString stringify(keycomb_t comb) {
  QString str;
  QTextStream s(&str);
  s << comb.vkCode << "," << comb.meta << "," << comb.ctrl << "," << comb.alt
    << "," << comb.shift;
  return str;
}

} // namespace kb
} // namespace sac
