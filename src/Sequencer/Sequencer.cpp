#include "Sequencer.hpp"

Sequencer::Sequencer(Motor* l, Motor* r) :  l(l), r(r) {}
Sequencer::Sequencer(Motor* l, Motor* r, std::vector<Movement*> movements) : l(l), r(r), movements(movements) {}

void Sequencer::add(Movement *movement) {
  movements.push_back(movement);
}

void Sequencer::advance() {
  n++;
  if (n != movements.size()) { play(); } else { finishedCallback(); }
}

void Sequencer::play(void) {
  movements[n]->onStepsGoal(callback(this,&Sequencer::advance));
  movements[n]->start(l,r,callback(this,&Sequencer::advance));
}