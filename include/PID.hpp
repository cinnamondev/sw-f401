/**
 * Motor PID CMSIS-DSP
 */
#ifndef PID_HPP
#define PID_HPP

#include <chrono>
#include "mbed.h"
#include "../CMSIS-DSP/Include/arm_math.h"
#include <limits>

/**
 * \brief Motor PID Controller extending CMSIS-DSP1
 */
class PID {
public:
  struct Gains {
    float kp;
    float ki;
    float kd;
  };
  struct Range {
    float min;
    float max;
    float range;
  };
  PID(Range inputRange, Range outputRange);
  PID(float setPoint, float *process, Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval, bool startImmediately);
  PID(Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval);
  bool initialize(float setPoint, float* process, Gains gains, Range inputRange, Range outputRange, std::chrono::microseconds computeInterval, bool startImmediately);
  bool validate();
  void start(void);
  void stop(void);
  void reset(void);
  bool isRunning(void);
  /**
   * Set the gains/coefficients of the PID controller.
   * @param kp Proportional term
   * @param ki Integral term
   * @param kd Derivative term
   */
  void setGains(float kp, float ki, float kd);
  /**
   * Set the gains/coefficients of the PID controller
   * @param g Gains structure containing kp, ki, kd terms.
   */
  void setGains(Gains g);
  /**
   * Get the current gains of the PID controller
   * @return Gains structure containing kp,ki,kd.
   */
  Gains getGains(void);
  /**
   * Set the time taken between PID computations.
   * @param interval
   */
  void setInterval(std::chrono::microseconds interval);
  std::chrono::microseconds getInterval(void);
  void setSetPoint(float sp);
  void setPV(float* pv);
  void setInputRange(float min, float max);
  void setInputRange(Range r);
  void setOutputRange(float min, float max);
  void setOutputRange(Range r);
  float getSetPoint(void);
  float getProcessVariable(void);
  Range getInputRange(void);
  Range getOutputRange(void);
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
