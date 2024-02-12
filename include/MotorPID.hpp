//
// Created by nutmeg on 2/12/24.
//

#ifndef PID_HPP
#define PID_HPP

#include <chrono>
#include "mbed.h"
#include "../CMSIS-DSP/Include/arm_math.h"

/**
 * \brief Motor PID Controller extending CMSIS-DSP1
 */
class MotorPID {
  bool running = false;
  arm_pid_instance_f32 instance;
  float setPoint;
  float inputFloor, inputCeiling, inputRange;
  float outputFloor, outputCeiling, outputRange;
  std::chrono::milliseconds sampleRate;
protected:
  Ticker ticker;
  /**
   * \brief Correct implementation should use this to pass variables to
   * compute.
   */
  virtual void tick(void);
public:
  MotorPID(float p, float i, float d);
  void limitInput(float min, float max);
  void limitOutput(float min, float max);
  void setCoefficients(float p, float i, float d);
  void setSampleRate(std::chrono::milliseconds sampleRate);
  void reset(void);
  float compute(void);
  float compute(float process);
  void stop(void);
  void start(void);
  bool isRunning(void);

};
#endif //PID_HPP
