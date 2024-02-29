# ESP Buggy Firmware

Buggy firmware for the Nucleo-F401RE.

The ESP is a group project, which will end with all groups constructing
a custom buggy design, which can drive autonomously.
## Building

### Development environment
Using either:
- Mbed Studio
- mbed-tools (CLI)
- Any IDE with PlatformIO support
- platformio (CLI)

### Submodule init

Project dependencies are organized into git submodules.

Make sure they are correctly cloned:
`git submodule init`
`git submodule update`

### Building

Then build with your IDE, or:
- `mbed-tools compile`
- `pio run`
    - Use `pio run --target upload` to upload the firmware.

## Thanks to:

- The contributors of the [C12832](https://os.mbed.com/teams/components/code/C12832/) library.
This project uses a modified version available [here](https://github.com/cinnamondev/C12832/tree/mbedOS-6), which allows it to be used with the latest version of Mbed OS.

