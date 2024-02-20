/**
 * @file main.cpp
 * @author cinnamondev
 * @date 2023-10-29
 *
 * Technical Demo A program Task 3/4
 */

#include "mbed.h"
#include "../c12832/C12832.h"

//PwmOut motorL(PB_13);
PwmOut motorR(PB_14);

InterruptIn enableSwitch(A2);
InterruptIn disableSwitch(A3);
int main() {
  auto* enable = new DigitalOut(A4, 0);
  AnalogIn potL(A0);
  AnalogIn potR(A1);

  motorR.period_us(20);
  motorR = 0.5;
  //QEI encoderR(PB_13, PB_14, NC, 255);
  enableSwitch.fall(callback([enable]() {
    enable->write(0);
  }));
  disableSwitch.fall(callback([enable]() {
    enable->write(1);
  }));
  float outL = 0.5; float outR = 0.5;
  while(true) {
    outL = potL.read();
    outR = potR.read();
    if ((0.4 < outL) & (outL < 0.6)) { outL = 0.5; }
    if ((0.4 < outR) & (outR < 0.6)) { outR = 0.5; }
    //motorL = outL;
    motorR = outR;

  }
}
