#include "mbed.h"
#include <stm32f4xx.h>
#include <stm32f4xx_ll_gpio.h>

#include "logging.h"
#include "Bluetooth.hpp"
static void buggyStart(uint8_t cmd);
static void buggyStop(uint8_t cmd);
static void buggyTurnaround(uint8_t cmd);
static void toggleLED(uint8_t cmd);
static void pingPong(uint8_t cmd);


static BufferedSerial bleSerial(PA_11, PA_12, 9600);
FileHandle *mbed::mbed_override_console(int fd) { return &bleSerial; }

static Bluetooth bluetooth(
    &bleSerial,
    std::vector<Bluetooth::Command>{
        Bluetooth::Command('A', 0xFF, callback(&toggleLED)), //  LED Test
        Bluetooth::Command('B', 0xFF, callback(&pingPong)),
        Bluetooth::Command('S', 0xFF, callback(&buggyStart)),
        Bluetooth::Command('Z', 0xFF, callback(&buggyStop)),
        Bluetooth::Command('T', 0xFF, callback(&buggyTurnaround)),
    },
    true);


DigitalOut out(LED1);
static void toggleLED(uint8_t cmd) {
  bool on = out.read();
  out.write(!on);
}

static void pingPong(uint8_t cmd) {
  DEBUG("Hello from BLE!");
}

// example buggy control cmmands

static void buggyStart(uint8_t cmd) {
  DEBUG("buggy started");
}

static void buggyStop(uint8_t cmd) {
  DEBUG("buggy stopped");
}

static void buggyTurnaround(uint8_t cmd) {
  DEBUG("sequence added...");
}

I2C lineSensor(SDA,SCL);

int main(void) {
  float linePosition;
  char rx_buf[4];
  while (true) {
    lineSensor.read(0x20, rx_buf, 4);
    linePosition = *reinterpret_cast<float*>(rx_buf);
    DEF_DEBUG("hello world");
    // spinny
  }
}
