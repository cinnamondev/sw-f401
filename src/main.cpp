/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Bluetooth.hpp"
/**
 * @brief Blink LED @ rate of BLINKING_RATE.
 */

DigitalOut led(LED1,0);

void toggleLED(char c) {
  static bool ledOn = false;
  ledOn=!ledOn;
  led = ledOn;
}
int main() {
  Bluetooth bluetooth(A1,D9);
  bluetooth.addCommand(Bluetooth::Command::build('A', callback(&toggleLED)));
  led = 0;
  while (true);
}