/**
 * Motor PID CMSIS-DSP
 */
#ifndef PID_HPP
#define PID_HPP

#include <chrono>
#include "mbed.h"
#include "../mbed-dsp/cmsis_dsp/arm_math.h"
#include <limits>

#ifndef PID_CONTROL_INTERVAL
#define PID_CONTROL_INTERVAL 1ms
#endif

/**
 * @brief CMSIS-DSP PID wrapper 
 * 
 * Use via polymorphism and override virtual methods to implement neccesary logic
 */
class PID {
public:
  PID();
  bool ready(void);
  bool start(void);
  void stop(void);
  void reset(void) { arm_pid_reset_f32(&pid); }
  bool isRunning(void) { return running; }
  void setGains(float kp, float ki, float kd);

  void inputRange(float min, float max);
  void outputRange(float min, float max);
  struct Config {
    float kp;
    float ki;
    float kd;
    float in_min; float in_max;
    float out_min; float out_max;
    float sp;
    float* pv;
    std::chrono::microseconds tick_interval;
  };
  PID::Config getConfig(void) {
    return (PID::Config) {
      .kp = pid.Kp,
      .ki = pid.Ki,
      .kd = pid.Kd,
      .in_min = inMin,
      .in_max = inMax,
      .out_min = outMin,
      .out_max = outMax,
      .sp = setPoint,
    };
  }
  void setConfig(PID::Config);
protected:
  /**
   * Passes PID output to implementation.
   */
  virtual void onCompute(float) = 0;
  /**
   * Function executed before computation occurs.
   */
  virtual void preCompute(void) = 0;
private:
  Ticker ticker;
  arm_pid_instance_f32 pid;
  bool running = false;
  float inMin;
  float inMax;
  float outMin;
  float outMax;
  float setPoint;
  float processLast;
  float outputLast;
  float* process;
  float compute(void);
  void tick(void);
};
#endif //PID_HPP
