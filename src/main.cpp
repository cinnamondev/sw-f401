/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Sequencer.hpp"
#include "QEI.hpp"
#include "../c12832/C12832.h"

//C12832 display(D11,D13,D12,D7,D10);
//QEI encoderL(PB_6, PB_7, NC, 255);
QEI encoderL(PB_12, PB_7, NC, 255);
//Motor motorL(PA_8, A5, &encoderL);
Motor motorL(PB_1, A5, &encoderL);
//QEI encoderR(PA_6, PA_7, NC, 255);
QEI encoderR(PA_12, PA_11, NC, 255);
Motor motorR(PC_7, A4, &encoderR);

//Thread displayThread;


float stepsToDegrees(unsigned int stepsPerRevolution, unsigned int steps) {
  return float(steps) * (360.0f/float(stepsPerRevolution));
}

int degreesToSteps(float degrees, int stepsPerRevolution) {
  return degrees * (float(stepsPerRevolution)/360.0f);
}

int distanceToSteps(float distanceMM, float diameterMM, int stepsPerRevolution) {
  return (360.0f * float(stepsPerRevolution) * distanceMM)/(M_PI * diameterMM);
}



void refreshDisplay() {
  while (true) {
    //display.cls();
    //display.locate(10,10);
    //display.printf("Pulses: %d", encoderR.getPulses());
    ThisThread::sleep_for(10ms);
  }
}
int main() {
  //displayThread.start(callback(&refreshDisplay));
  //encoderR.reset();
  //int S1M = distanceToSteps(1000, 22, 255);
  //Sequencer sequencer(&motorL,&motorR,std::vector<Movement*> {new Line(S1M), new Turn(90), new Line(S1M), new Turn(90), new Line(S1M), new Turn(S1M)});
  //sequencer.repeats(true);
  //sequencer.play();

  while(true);
}