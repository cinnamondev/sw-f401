<!-- @mainpage -->
# sw-f401

<div style="text-align:center"><img src="./.attachments/esproj.png" /></div>

Firmware for the Nucleo-F401RE (All functions exc. Line Processing)

For line processing, see `sw-lp-g030`.

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

Refer to the [wiring diagram](https://github.com/embedded-systems-30/wiring-diagram).

## Line Processing

Line processing work is delegated to the line sensor module. See [sw-lp-g030](https://github.com/embedded-systems-30/sw-lp-g030).

## bleConsole script

Use bleConsole for debug output and sending commands. Requires python 3.
Install `bleak` and `aioconsole` to use, some properties may need to be changed.
HM-10 module should always use the same characteristic UUID for UART service, the script uses the device name to identify it, which is likely different for you.
