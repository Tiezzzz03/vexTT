#include "main.h"

using namespace okapi::literals;

void initialize() {}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  okapi::Motor angler(-1);

  auto chassis = okapi::ChassisControllerFactory::create(
    {8,9}, {-3,-4},
    okapi::AbstractMotor::gearset::green,
    {4.125_in, 10.125_in}
  );

  okapi::MotorGroup intake({-2,10});

  okapi::Controller controller;

  while(true){
    chassis.tank(controller.getAnalog(okapi::ControllerAnalog::leftY),
                 controller.getAnalog(okapi::ControllerAnalog::rightY));

    if(controller.getDigital(okapi::ControllerDigital::R1)){
      intake.moveVoltage(12000);
    }else if(controller.getDigital(okapi::ControllerDigital::Y)){
      intake.moveVoltage(-12000);
    }else{
      intake.moveVoltage(0);
    }

    if(controller.getDigital(okapi::ControllerDigital::L1)){
      angler.moveVoltage(12000);
    }else if(controller.getDigital(okapi::ControllerDigital::right)){
      angler.moveVoltage(-12000);
    }else{
      angler.moveVoltage(0);
    }

    pros::delay(10);
  }
}
