#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "PID.hpp"

class Motor : public PID {
public:
  Motor(Motor::Config);
  Motor(PinName pwm, PinName current, PID::Config pidConfig);
  struct Config {
    PinName pwm;
    PinName current;
    PID::Config pidCfg;
  };
private:void onCompute(float) override;
    void preCompute(void) override;
    AnalogIn current;

};

#endif // MOTOR_HPP