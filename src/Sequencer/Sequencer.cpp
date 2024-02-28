#include "Sequencer.hpp"

#include <utility>

Sequencer::Sequencer(Motor* l, Motor* r, DigitalOut* en) :  driver_en(en), l(l), r(r) {}
Sequencer::Sequencer(Motor* l, Motor* r,  DigitalOut* en, std::vector<Movement*> movements) : driver_en(en), movements(std::move(movements)), l(l), r(r) {}

void Sequencer::add(Movement *movement) {
  movements.push_back(movement);
}

void Sequencer::startNMovement() {
  driver_en->write(1);
  movements[n]->onStepsGoal(callback(this,&Sequencer::advance));
  movements[n]->start(l,r,callback(this,&Sequencer::advance));
}
void Sequencer::advance() {
  driver_en->write(0);
  wait_us(500000); // TODO: Sequencer needs to be refactored to become 'Threaded'.
                   // ^^ Would allow for delays to be performed without spin-lock.
  r->encoder->reset();
  if (++n < movements.size()) { startNMovement(); } else {
    if (finishedCallback != nullptr) finishedCallback();
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