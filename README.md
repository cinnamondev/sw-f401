# ESP Buggy Firmware

Buggy firmware for the Nucleo-F401RE.

The ESP is a group project, which will end with all groups constructing
a custom buggy design, which can drive autonomously.
## Building

### Development environment

This project structure is centred around using [PlatformIO](https://platformio.org). PlatformIO is availale
as a command-line tool, and as an extension to most of your favourite editors/IDEs.

## Documentation

To build Doxygen documentation, you should ensure that the [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)
submodule is cloned. In the project root, execute:
- `git submodule init`
- `git submodule update`

### Building

Then build with your IDE, or:
- `mbed-tools compile`
- `pio run`
    - Use `pio run --target upload` to upload the firmware.

## Wiring

Refer to the [wiring diagram](https://github.com/embedded-systems-30/wiring-diagram).

## Thanks to:

- The contributors of the [C12832](https://os.mbed.com/teams/components/code/C12832/) library.
This project uses a modified version available [here](https://github.com/cinnamondev/C12832/tree/mbedOS-6), which allows it to be used with the latest version of Mbed OS.

