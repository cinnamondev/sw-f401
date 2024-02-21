/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Bluetooth.hpp"

#ifdef DEMO_CONFIG_REDIRECT_STDIO_HM10
// If macro enabled redirect PRINTF to BLE MODULE :)
static BufferedSerial hm10(NC,NC,9600);
namespace mbed {
FileHandle *mbed_target_override_console(int) {
  return &hm10;
}
#endif
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