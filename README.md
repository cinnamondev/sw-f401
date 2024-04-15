<!-- @mainpage -->
# ESProject G30 2023/24: sw-f401

Firmware for the Nucleo-F401RE of the G30 ESProject buggy.

The ESProject is a 2nd year group project to design a line-following buggy, requiring
students to design around a few key components, with the final designs being tested against
one-another in a final race.

## Building

### Development environment

This project structure is centred around using [PlatformIO](https://platformio.org). PlatformIO is availale as a command-line tool, or as an extension to most of your favourite editors/IDEs.

### Dependencies

- mbed-dsp
- mbed-os

Above will be downloaded automagically by PlatformIO on build.

## Wiring

\image{inline} html caddrawio.png "Wiring Diagram" height=712px
Refer to the [wiring diagram](https://github.com/embedded-systems-30/wiring-diagram) for the editable diagram.

## Line Sensor Module

Line Sensor Module requires it's own firmware. Please refer to [sw-lp-g030](https://github.com/cinnamondev/sw-lp-g030).
(Documentation available at [docs: sw-lp-g030](https://cinnamondev.github.io/sw-lp-g030)).

See [sw-lp-g030](https://github.com/cinnamondev/sw-lp-g030).

## bleConsole script

Use bleConsole for debug output and sending commands. Requires python 3.
Install `bleak` and `aioconsole` to use, some properties may need to be changed.
HM-10 module should always use the same characteristic UUID for UART service, the script uses the device name to identify it, which is likely different for you.

## Code styling

Code style is enforced by Clang-Format ([LLVM Style](https://llvm.org/docs/CodingStandards.html#header-guard)). All contributed code should follow this style.
