#include "BuggyControl.hpp"
#include "logging.h"

#define RANGE 6.00f

void BuggyController::preCompute(void) {
  linePosition = lineSensor->getLinePosition();
  // Check line presence, if it is not present within a few ticks, the
  // Timeout will execute and stop the buggy.
  if (linePosition < 0) {
    movementTimer.attach(callback([this]() {
      stop();
    }), 100ms);
  } else {
    // line present again, prevent timeout from running :)
    movementTimer.detach();
  }

  if (currentMovement == nullptr && buggyRunning) {
    // try and get a new movement
    if (try_get(&currentMovement)) {
      stop();
      // Attach movement distance event, stop buggy when distance event exceed
      motorLeft->setSpeed(currentMovement->left);
      motorRight->setSpeed(currentMovement->right);
      movementTimer.attach(callback([this]() {
                             motorLeft->setSpeed(0);
                             motorRight->setSpeed(0);
                             start();
                             currentMovement = nullptr;
                           }),
                           currentMovement->condition.timerEvent);
      // apply speeds
    } else { currentMovement = nullptr; }
  }
}
void BuggyController::onCompute(float output) {
  if (isRunning()) {
    motorLeft->setSpeed(100 + output);
    motorRight->setSpeed(100 + output);
  }
}

BuggyController::BuggyController(PID::Config cfg, Motor *l, Motor *r,
                                 LineSensor *ls, PinName driverEnable)
    : PID(cfg, 1ms), motorLeft(l), motorRight(r), lineSensor(ls),
      en(driverEnable, 0) {}

void BuggyController::stopBuggy(void) {
  en = buggyEnabled = 0;
}

void BuggyController::startBuggy(void) {
  en = buggyEnabled = 1;
  reset();
  start();
}

bool BuggyController::isEnabled(void) {
  return buggyEnabled;
}