/**
 * @example bluetooth.cpp
 * @author cinnamondev
 * @brief HM-10 Bluetooth Library Usage
 */
#include "Bluetooth.hpp"
#include "mbed.h"

DigitalOut led(LED1, 0);
BufferedSerial bleSerial(PA_11, PA_12, 9600); // Serial Interface to HM10

// Including this override retargets calls to printf to go through the BLE
// interface.
FileHandle *mbed::mbed_override_console(int fd) { return &bleSerial; }

void toggleLED(uint8_t cmd) { // Toggle an LED on/off.
  bool on = led.read();       // Executes in shared thread context
  led.write(!on);  
}

void ledCmd(uint8_t cmd) { // State of final bit determines
  led = cmd & 0x01;        // whether to turn LED on or off.
}

int main(void) {
  Bluetooth bluetooth(
      &bleSerial,
      // Provide an initial set of bluetooth commands 
      std::vector<Bluetooth::Command>{
          Bluetooth::Command('A', 0xFF, callback(&toggleLED)), // Toggle LED
          Bluetooth::Command(0xEE, 0xFE, callback(&ledCmd)),   // Set LED on/off
      },
      false); // Does not accept any new commands. Set true to start
              // immediately (optional parameter)

  bluetooth.start(); // Starts accepting new commands NOW.
  while (true)
    ; // Spin forever...
}