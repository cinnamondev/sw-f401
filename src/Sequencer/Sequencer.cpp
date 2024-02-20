#include "Sequencer.hpp"

#include <utility>

Sequencer::Sequencer(Motor* l, Motor* r) :  l(l), r(r) {}
Sequencer::Sequencer(Motor* l, Motor* r, std::vector<Movement*> movements) : movements(movements), l(l), r(r) {}

void Sequencer::add(Movement *movement) {
  movements.push_back(movement);
}

void Sequencer::startNMovement() {
  movements[n]->onStepsGoal(callback(this,&Sequencer::advance));
  movements[n]->start(l,r,callback(this,&Sequencer::advance));
}
void Sequencer::advance() {
  n++;
  if (n < movements.size()) { startNMovement(); } else {
    finishedCallback();
    if(doesRepeat) { play(); }
  }
}

void Sequencer::play(void) {
  n = 0;
  startNMovement();
}

void Sequencer::repeats(bool repeat) {
  doesRepeat = repeat;
}