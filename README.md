![](https://i.imgur.com/KvqVlJL.png)
# SuperAutoClicker

**SuperAutoClicker** is a free, open-source auto clicker that can be controlled on-the-fly by only the keyboard buttons. You can set the interval, toggle it on and off and switch between mouse buttons without ever leaving your game.

## Supported Operating Systems

Only Windows is supported.

## Recommendations

It is recommended that you have a keyboard with a dedicated numpad.

# Usage

You control SuperAutoClicker by using the keyboard. **The window does not have to be active for it to listen to your inputs.**

To begin using the auto clicker, press the `Listen Mode` button. In Listen Mode, you can use the number keys to specify the interval (in milliseconds) between clicks. Do note that there is a limit of 7 digits in your input.

When you're done entering your desired interval, you can press the Listen Mode button again to save it or you can simply press the `Click Mode` button to toggle Click Mode. When it is turned on, the program will begin clicking at such interval.

You can press the `Toggle Mouse Button` key to switch between the left and right mouse buttons. If you want to stop the auto clicker, press the Toggle Mouse Button again.

You can also change the interval on the go, while the auto clicker is clicking. Simply press turn on Listen Mode, type your millisecond interval and then turn off Listen Mode.

## In a nutshell...

|                           | Description                                                                                                                                      |
|---------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| `Toggle Mouse button`     | Switches between left and right mouse buttons.                                                                                                   |
| `Listen Mode`             | State in which the program listens for your numeric inputs. Your numeric inputs will be stored as millisecond intervals for the autoclicker.     |
| `Click Mode`              | State in which the program repeatedly clicks with either the left or right mouse button repeatedly, depending on your Toggle Mouse Button state. |
| `Number Row or NUMPAD0-9` | Inputs milliseconds while in Listen Mode.                                                                                                        |

## Example

- Example 1: Setting the autoclicker to click at a 100 ms interval.
    - Open SuperAutoClicker.
    - Press the `Listen Mode` button.
    - Type `1 0 0` with the number row or your numpad.
    - Press the `Listen Mode` button to save your input.
        - Press the `Click Mode` button to start clicking.
        - Press the `Click Mode` button again to turn the autoclicker off.
    - **OR:**  Press the `Click Mode` button to immediately start clicking.

- Example 2: Changing the autoclicker on the fly from 200 ms to 10 ms.
   - Open SuperAutoClicker.
   - Press the `Listen Mode` button.
   - Type `2 0 0`.
   - Press `Click Mode` button to start clicking.
   - Press the `Listen Mode` button again.
   - Type `1 0`.
   - Press the `Listen Mode` button again.
   - The auto clicker should instantly switch to the new speed.

# Customizing Keys

As SuperAutoClicker is a work in progress, there is currently no user-friendly way to change the keybindings. As it turns out, you can configure the keys by editing the .ini configuration file, located in your home directory. In Windows, this should be `"%USERPROFILE%\SuperAutoClicker Configuration.ini"`.

The key binding strings have the following format: `KEYCODE,META,CTRL,ALT,SHIFT`, where `KEYCODE` is a **decimal** keycode, and `META`, `CTRL`, `ALT` and `SHIFT` are either 1 (for enabled) and 0 (for disabled). Change the first number in the comma-separated list of numbers to the keycode you want. The list of valid keycodes (in **hexadecimal**) can be found [here](KEYCODES.md).

## Example

`'C:\Users\John\SuperAutoClicker Configuration.ini'`

    [General]
    ListenModeKey="120,0,0,0,0"    ..... F9
    ClickModeKey="121,0,1,0,1"     ..... CTRL SHIFT F10
    MouseButtonKey="122,1,1,1,1"   ..... WIN CTRL ALT SHIFT F11

# Download

[SuperAutoClicker 2.0.1_PRE (x86_64)]()

[SuperAutoClicker 2.0.0_PRE (x86_64)](https://github.com/michelfaria/SuperAutoClicker/releases/download/2.0.0/Super.Auto.Clicker.zip)

[SuperAutoClicker 1.0.0 (x86_64)](https://github.com/michelfaria/SuperAutoClicker/releases/download/1.0.0/SuperAutoClicker.v1.0.0.exe)

[All Releases](https://github.com/michelfaria/SuperAutoClicker/releases)

# Building

## Windows

**Requirements:** **Qt Creator 4.9.0, QMake 3.1, Qt (5.12.3), (MSVC 2017)**

- **Command-line:**
    - Release

            qmake src.pro -spec win32-msvc "CONFIG+=qtquickcompiler"
            jom in Release
    
    - Debug

            qmake src.pro -spec win32-msvc "CONFIG+=debug" "CONFIG+=qml_debug"
            jom in Debug

- **Qt Creator**
    - Open `src/src.pro` and build the project.

# Planned Features

- User friendly way to edit keybindings

# License

Project is licensed under MIT License. See LICENSE.txt for details.
