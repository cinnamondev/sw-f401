#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include "mbed.h"

class Bluetooth {
  BufferedSerial hm10;
  DigitalOut led;
  char cmd;
public:
  Bluetooth(PinName tx, PinName rx, PinName ledPin);

  void checkData();
  void beepLED();
};

#endif