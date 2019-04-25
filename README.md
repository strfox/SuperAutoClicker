# SuperAutoClicker

**SuperAutoClicker** is a free, open-source auto clicker that can be controlled on-the-fly by using the Numpad buttons. You can set the interval, toggle it on and off and switch between mouse buttons without ever leaving your game.

## Supported Operating Systems

Only Windows is supported.

## Recommendations

It is recommended that you have a keyboard with a dedicated numpad.

# Usage

You control SuperAutoClicker by using the Numpad on your keyboard. **All buttons referenced below have to be pressed on their Numpad counterparts.**

To begin using the auto clicker, press `-` (Minus) to enter **Listen Mode**. In Listen Mode, you can use the number keys to specify the interval (in milliseconds) between clicks. Do note that there is a limit of 5 digits in your input.

When you're done entering your desired interval, you can press `-` again to save it or you can simply press `+` to start the autoclicker immediately. Once you press `+`, SuperAutoClicker will enter `Clicking Mode`.

You can press the `/` key to switch between the left and right mouse buttons. If you want to stop the auto clicker, press `+` again.

You can also change the interval on the go, while the auto clicker is clicking. Simply press `-`, type your millisecond interval and then press `-` again to save.

In a nutshell:
* `/` ➡ Switches between left and right mouse buttons.
* `-` ➡ Toggles `Listening Mode`.
* `+` ➡ Toggles `Clicking Mode`.
* `NUMPAD0-9` ➡ Input milliseconds while in `Listening Mode`.

# Download
[SuperAutoClicker 1.0.0 (x86_64)](https://github.com/michelfaria/SuperAutoClicker/releases/download/1.0.0/SuperAutoClicker.v1.0.0.exe)

[All Releases](https://github.com/michelfaria/SuperAutoClicker/releases)

# Building

## Windows

**Requirements:** [MinGW](http://www.mingw.org/), [CMake 3.14+](https://cmake.org/).

Under Windows, run the `build.ps1` PowerShell script. 

`build.ps1` params:
* `-Debug` ➡ Compile debug build
* `-Clean` ➡ Delete build folder before compiling

# Planned Features

* Support for changing key mappings
* Mouse button toggle

# License

Project is licensed under MIT License. See LICENSE.txt for details.