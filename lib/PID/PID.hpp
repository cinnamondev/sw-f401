/**
 * Motor PID CMSIS-DSP
 */
#ifndef PID_HPP
#define PID_HPP

#include <chrono>
#include "mbed.h"
#include "../mbed-dsp/cmsis_dsp/arm_math.h"
#include <limits>

/**
 * \brief Motor PID Controller extending CMSIS-DSP1
 */
class PID {
public:
  PID(Range inputRange, Range outputRange);
  PID(float setPoint, float *process, Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval, bool startImmediately);
  PID(Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval);
  bool initialize(float setPoint, float* process, Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval, bool startImmediately);
  void start(void);
  void stop(void);
  void reset(void);
  bool isRunning(void);
  void setGains(float kp, float ki, float kd);
  void setSP(float sp);
  void setPV(float* pv);
  void inputRange(float min, float max);
  void outputRange(float min, float max);
protected:
  /**
   * Passes PID output to implementation.
   */
  virtual void onCompute(float) = 0;
  /**
   * Function executed before computation occurs. Optional implementation.
   */
  virtual void preCompute(void) {}
private:
  Ticker ticker;
  std::chrono::microseconds interval = std::chrono::microseconds::max();
  arm_pid_instance_f32 pid;
  bool running = false;
  Range inRange;
  Range outRange;
  float setPoint;
  float processLast;
  float outputLast;
  float* process;
  float compute(void);
  void tick(void);
};
#endif //PID_HPP
