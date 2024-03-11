#include "mbed.h"
#include <stm32f4xx.h>
#include <stm32f4xx_ll_gpio.h>

#include "Bluetooth.hpp"

BufferedSerial bleSerial(PA_11,PA_12);

/*
FileHandle* mbed::mbed_target_override_console(int fd) {
  return &_bleSerial;
}
 */

DigitalOut out(LED1);
void toggleLED(uint8_t cmd) {
  bool on = out.read();
  out.write(!on);
}

int main(void) {
  Bluetooth bluetooth(&bleSerial, std::vector<Bluetooth::Command> {
                                       Bluetooth::Command('A', 0xFF, callback(&toggleLED)),
                                   }, true);
  while (true) {
  }
}
