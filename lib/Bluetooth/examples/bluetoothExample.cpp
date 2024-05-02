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

EventQueue eventQueue(32 * EVENTS_EVENT_SIZE);
int main(void) {
  Bluetooth bluetooth(
      &bleSerial,
      &eventQueue, // replace with `mbed_event_queue` if you want and are using
                   // RTOS build.
      // Provide an initial set of bluetooth commands
      std::vector<Bluetooth::Command>{
          Bluetooth::Command('A', 0xFF, callback(&toggleLED)), // Toggle LED
          Bluetooth::Command(0xEE, 0xFE, callback(&ledCmd)),   // Set LED on/off
          Bluetooth::Command(
              'C', 0xFF, callback([](uint8_t cmd) {
                led = !led; // You can also use lambdas in callbacks, be aware
                            // that callbacks have a constrained size so they
                            // can be passed around like pointers, so
                            // realistically only use this on things that handle
                            // say, static duration items, `as uint8_t cmd` is
                            // still required here.
              }))},
      false); // Does not accept any new commands. Set true to start
              // immediately (optional parameter)

  bluetooth.start(); // Starts accepting new commands NOW.
  while (true) {
    eventQueue.dispatch_once(); // other events in main thread
  }
}