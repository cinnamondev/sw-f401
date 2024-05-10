#ifndef STEERINGCONTROLLER_HPP
#define STEERINGCONTROLLER_HPP

#include "mbed.h"
#include "LineSensor.hpp"
#include "Motor.hpp"
#include "PID.hpp"
#include "ConditionMovement.hpp"

#define LS_I2C_ADDR 0x23
#define LS_I2C_TDB_WR_CMD 0x42

/**
 * @brief
 *
 */
class BuggyController
    : protected PID,
      protected Queue<ConditionMovement, 16> {
public:
  using Queue::try_put;
  using Queue::try_put_for;
  BuggyController(PID::Config cfg, Motor *l, Motor *r,
                                 LineSensor *ls, PinName driverEnable = PB_2);
  volatile float linePosition;
  bool buggyEnabled = false;
  void startBuggy(void);
  void stopBuggy(void);
  bool isEnabled(void);
private:
  volatile bool buggyRunning = false;
  volatile bool movementStarted = false;
  Timeout movementTimer;
  Ticker pidTicker; /**< Periodically run LS PID, ran as ticker to overthrow
                      *  other threads.
                      */
  Motor* motorLeft; /**< Buggy left motor */
  Motor* motorRight; /**< Buggy right motor */
  LineSensor* lineSensor; /**< Line sensor module (G030 bypass impl) */
  ConditionMovement *currentMovement = nullptr; /**< Currently executing movement */
  DigitalOut en; /**< Enable pin drive board */
  uint16_t s_buff[6]; /**< Sensor raw output buffer */
  void onCompute(float) override;
  void preCompute(void) override;
};

#endif // STEERINGCONTROLLER_HPP