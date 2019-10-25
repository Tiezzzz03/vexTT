#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

using namespace okapi::literals;

void disabled() {}

void competition_initialize() {
  robot::screen::state = screenMode::selection;
}

void autonomous() {
  routines[selection].run();
}

void opcontrol() {
  robot::screen::state = screenMode::notification;

  if (robot::chassisProfiler) robot::chassisProfiler->flipDisable(true);

  while(true){
    if(robot::controller.getDigital(okapi::ControllerDigital::A)){
      robot::chassis->getModel()->arcade(robot::controller.getAnalog(okapi::ControllerAnalog::leftY) * 0.5, 0);
    }else{
      robot::chassis->getModel()->tank(robot::controller.getAnalog(okapi::ControllerAnalog::leftY),
                                       robot::controller.getAnalog(okapi::ControllerAnalog::rightY));
    }

    if(robot::controller.getDigital(okapi::ControllerDigital::R1)){
      robot::intake->moveVoltage(12000);
    }else if(robot::controller.getDigital(okapi::ControllerDigital::R2)){
      robot::intake->moveVoltage(6000);
    }else if(robot::controller.getDigital(okapi::ControllerDigital::Y)){
      robot::intake->moveVoltage(-6000);
    }else{
      robot::intake->moveVoltage(0);
    }

    if(robot::controller.getDigital(okapi::ControllerDigital::right)){
      robot::angler->stack();
    }else if(robot::controller.getDigital(okapi::ControllerDigital::L1)){
      robot::angler->reset();
    }

    pros::delay(10);
  }
}
