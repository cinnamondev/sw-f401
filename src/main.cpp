#include "mbed.h"
#include <stm32f4xx.h>
#include <stm32f4xx_ll_gpio.h>

#include "Bluetooth.hpp"
static BufferedSerial bleSerial(PA_11, PA_12, 9600);

bool use_ble = true;
FileHandle *mbed::mbed_override_console(int fd)
{
  return &bleSerial;
}

DigitalOut out(LED1);
void toggleLED(uint8_t cmd) {
  bool on = out.read();
  out.write(!on);
}

void pingPong(uint8_t cmd) {
  printf("Hello from BLE!");
  fflush(stdout);
  bleSerial.sync();
}

// example buggy control cmmands

void buggyStart(uint8_t cmd) {
  printf("buggy started");
  fflush(stdout);
}

void buggyStop(uint8_t cmd) {
  printf("buggy stopped");
  fflush(stdout);
}

void buggyTurnaround(uint8_t cmd) {
  printf("sequence added...");
  fflush(stdout);
}

int main(void) {
  Bluetooth bluetooth(
      &bleSerial,
      std::vector<Bluetooth::Command>{
          Bluetooth::Command('A', 0xFF, callback(&toggleLED)), //  LED Test
          Bluetooth::Command('B', 0xFF, callback(&pingPong)), // module response test
          Bluetooth::Command('S', 0xFF, callback(&buggyStart)),
          Bluetooth::Command('Z', 0xFF, callback(&buggyStop)),
          Bluetooth::Command('T', 0xFF, callback(&buggyTurnaround)),
      },
      true);
  while (true) {
    // spinny
  }
}
