#ifndef __MOTOR_INC_
#define __MOTOR_INC_

#include "Quadrature.hpp"
#include "mbed.h"
#include "PID.hpp"
#include "QEI.hpp"
// compile time mode define MOTOR_USE_BIPOLAR alt mode
class Motor : protected PID {
public:
  class QEI *encoder;
  enum Controller { EncoderControlled, CurrentControlled, OperatorControlled };
  /**
   * The direction the motor turns (relative to itself).
   */
  enum Direction { Forwards, Backwards };
  Motor(PinName pwm, PinName current, QEI *encoder);
  Motor(PinName pwm, PinName current, QEI *encoder, Motor::Controller mode);
  void PIDInit(PID::Gains g, PID::Range rangeIn, PID::Range rangeOut);
  void setMode(Controller mode);
  /**
   * Set the duty cycle of the Motor manually.
   * \note No effect outside of OperatorControlled mode.
   * \param pwm PWM Duty cycle
   */
  void setPWM(float pwm);
private:
  bool isPIDRunning = false;
  PwmOut pwmOut;
  AnalogIn currentMonitor;
  Controller controllerMode;
  Direction direction;

  void onCompute(float result) override;

};

#endif