#include "mechanisms/angler.hpp"

namespace robot {

Angler::Angler(std::unique_ptr<okapi::AsyncPositionController<double, double>> icontroller,
               double iactivePos,
               double istackingPos):
               controller(std::move(icontroller)),
               activePos(iactivePos),
               stackingPos(istackingPos),
               status(robot::AnglerStatus::initial)
{}

void Angler::reset(){
  controller->flipDisable();
  underlying::angler->moveVoltage(-4000);
  do {
    pros::delay(1000);
  } while(underlying::angler->getActualVelocity() > 0);

  underlying::angler->tarePosition();
  controller->flipDisable();
  setStatus(robot::AnglerStatus::active);
}

void Angler::setStatus(robot::AnglerStatus istatus){
  status = istatus;
  setTarget();
}

bool Angler::isStacking(){
  return
    (status == robot::AnglerStatus::stacking) &&
    (abs(controller->getError()) < 10);
}

void Angler::trim(double delta){
  activePos   += delta;
  stackingPos += delta;
  setTarget();
}

void Angler::setTarget(){
  switch(status){
    case robot::AnglerStatus::initial:
      controller->setTarget(0);
      break;
    case robot::AnglerStatus::active:
      controller->setTarget(activePos);
      break;
    case robot::AnglerStatus::stacking:
      controller->setTarget(stackingPos);
      break;
  }
}

};
