#ifndef __MOTOR_INC_
#define __MOTOR_INC_

#include "../CMSIS-DSP/Include/arm_math.h"
#include "Quadrature.hpp"
#include "mbed.h"
#include "PID.hpp"
// compile time mode define MOTOR_USE_BIPOLAR alt mode
class Motor : protected PID {
public:
  enum Controller { PID, User };
  enum FeedbackSource { Quadrature, Current };
  enum Direction { Forwards, Backwards };
  Motor(PinName pwm, PinName current, Quadrature *encoder);
  Motor(PinName pwm, PinName current, Quadrature *encoder, Motor::Controller mode);
  void setMode(Controller mode);
  void setPWM(float pwm);
private:
  bool isPIDRunning = false;
  arm_pid_instance_f32 pid;
  PwmOut pwmOut;
  AnalogIn currentMonitor;
  FeedbackSource feedbackSource;
  Direction direction;
  class Quadrature *encoder;

  void onCompute(float result) override;

};

#endif