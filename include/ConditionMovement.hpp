//
// Created by cinnamon on 26/04/2024.
//

#ifndef SW_F401_CONDITIONMOVEMENT_HPP
#define SW_F401_CONDITIONMOVEMENT_HPP

#include "mbed.h"
#include "BuggyControl.hpp"

struct ConditionMovement {
  // note to fellow devs... `friend` means BuggyController can access the
  // private access level elements in here :) but others can't! very
  // sneaky!
  friend class BuggyController;

private:
  float left;                  /**< Speed to run left motor at */
  float right;                 /**< Speed to run right motor at */
  bool timedCondition = false; /**< Stores state of `condition` */
  union {
    int distance;
    std::chrono::microseconds timerEvent;
  } condition; /**< Condition to use */
public:
  /**
   * @brief Create a Movement with a distance (steps) condition.
   *
   * The distance event's implementation depends upstream, though
   * in BuggyController we will use left encoder.
   *
   * @param speedLeft Speed to set left wheel steps/tick
   * @param speedRight Speed to set right wheel in steps/tick
   * @param distance Distance to travel in steps
   */
  ConditionMovement(float speedLeft, float speedRight, float distance)
      : left(speedLeft), right(speedRight), timedCondition(false) {
    condition.distance = distance;
  }
  /**
   * @brief Create a Movement with a TIMED condition.
   *
   * The distance event's implementation depends upstream, though
   * in BuggyController we will use left encoder.
   *
   * @param speedLeft Speed to set left wheel steps/tick
   * @param speedRight Speed to set right wheel in steps/tick
   * @param distance Time to run the wheels at speed left/right for.
   */
  ConditionMovement(float speedLeft, float speedRight,
                    std::chrono::milliseconds time)
      : left(speedLeft), right(speedRight), timedCondition(true) {
    condition.timerEvent = time;
  }
};

static  ConditionMovement turnAround(10, 10, 1000ms);

#endif // SW_F401_CONDITIONMOVEMENT_HPP
