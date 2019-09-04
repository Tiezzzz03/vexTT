#include "mechanisms/angler.hpp"

namespace mechanism {

Angler::Angler(std::unique_ptr<okapi::AsyncPositionController<double, double>> icontroller,
               double iactivePos,
               double istackingPos):
               controller(std::move(icontroller)),
               activePos(iactivePos),
               stackingPos(istackingPos),
               status(AnglerStatus::initial)
{}

void Angler::reset(){
  controller->flipDisable();
  robot::underlying::angler->moveVoltage(-4000);
  do {
    pros::delay(1000);
  } while(robot::underlying::angler->getActualVelocity() > 0);

  robot::underlying::angler->tarePosition();
  controller->flipDisable();
  setStatus(AnglerStatus::active);
}

void Angler::setStatus(AnglerStatus istatus){
  status = istatus;
  setTarget();
}

bool Angler::isStacking(){
  return
    (status == AnglerStatus::stacking) &&
    (abs(controller->getError()) < 10);
}

void Angler::trim(double delta){
  activePos   += delta;
  stackingPos += delta;
  setTarget();
}

void Angler::setTarget(){
  switch(status){
    case AnglerStatus::initial:
      controller->setTarget(0);
      break;
    case AnglerStatus::active:
      controller->setTarget(activePos);
      break;
    case AnglerStatus::stacking:
      controller->setTarget(stackingPos);
      break;
  }
}

};
