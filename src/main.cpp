#include "main.h"
#include "robot.hpp"

using namespace okapi::literals;

void initialize() {
  robot::logFile = fopen("/usd/TT/log.txt", "w");

  robot::angler = std::make_shared<okapi::Motor>(-1);
  robot::intake = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({-2,10}));
  robot::lDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({ 8, 9}));
  robot::rDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({-3,-4}));

  robot::lEnc = std::make_shared<okapi::ADIEncoder>(7,8, true);
  robot::rEnc = std::make_shared<okapi::ADIEncoder>(1,2, true);
  robot::mEnc = std::make_shared<okapi::ADIEncoder>(5,6);

  robot::chassis = okapi::ChassisControllerBuilder()
                    .withMotors(robot::lDrive, robot::rDrive)
                    .withSensors(robot::lEnc, robot::rEnc, robot::mEnc)
                    .withOdometry()
                    .withGearset(okapi::AbstractMotor::gearset::green)
                    .withDimensions(okapi::ChassisScales({2.75_in, 10.125_in, 0_in, 2.75_in}, 360))
                    .withLogger(std::make_shared<okapi::Logger>(okapi::TimeUtilFactory::create().getTimer(), "/ser/cout", okapi::Logger::LogLevel::debug))
                    .build();//Odometry();

  robot::intake->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);


  pros::delay(10);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {

  while(true){
    robot::chassis->getModel()->tank(robot::controller.getAnalog(okapi::ControllerAnalog::leftY),
                                     robot::controller.getAnalog(okapi::ControllerAnalog::rightY));

    if(robot::controller.getDigital(okapi::ControllerDigital::R1)){
      robot::intake->moveVoltage(12000);
    }else if(robot::controller.getDigital(okapi::ControllerDigital::Y )){
      robot::intake->moveVoltage(-12000);
    }else{
      robot::intake->moveVoltage(0);
    }

    if(robot::controller.getDigital(okapi::ControllerDigital::right)){
      robot::angler->moveVoltage(6000);
    }else if(robot::controller.getDigital(okapi::ControllerDigital::L1)){
      robot::angler->moveVoltage(-12000);
    }else{
      robot::angler->moveVoltage(0);
    }


    pros::delay(10);
  }
}
