#include "mbed.h"
#include <stm32f4xx.h>
#include <stm32f4xx_ll_gpio.h>

#include "Bluetooth.hpp"

BufferedSerial _bleSerial(PA_11,PA_12);
FileHandle* mbed::mbed_target_override_console(int fd) {
  return &_bleSerial;
}


DigitalOut out(D5);

int main(void) {
  // setup code
  while (true) {
    // main loop
  }
}
