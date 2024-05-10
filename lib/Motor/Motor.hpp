#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "PID.hpp"
#include "Encoder.hpp"

/**
 * @brief Mbed + STSTM32 Encoder + Motor PID Controller
 * 
 */
class Motor : protected PID {
public:
  /**
   * @brief Motor pinout + PID configuration
   * 
   */
  struct Config {
    PinName pwm;
    PinName current;
    PID::Config pid;
  };
  Motor(Motor::Config config, TIM_HandleTypeDef* tim);
  void setSpeed(float stepsPTick);
  /** Update gains PID algorithm */
  using PID::setGains;
  void setConfig(PID::Config) override;
  Encoder encoder;
private:
    void onCompute(float) override;
    void preCompute(void) override;
    PwmOut pwmOut;
    float speed = 0;
};

#endif // MOTOR_HPP