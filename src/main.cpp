#include "main.h"
#include "robot.hpp"

using namespace okapi::literals;

void initialize() {
  robot::logFile = fopen("/usd/TT/log.txt", "w");

  robot::angler = std::make_shared<okapi::Motor>(-1);
  robot::intake = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({-2,10}));
  robot::lDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({ 8, 9}));
  robot::rDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({-3,-4}));

  robot::lEnc = std::make_shared<okapi::ADIEncoder>(0,0);
  robot::rEnc = std::make_shared<okapi::ADIEncoder>(0,0);
  robot::mEnc = std::make_shared<okapi::ADIEncoder>(0,0);

  robot::chassis = okapi::ChassisControllerBuilder()
                    .withMotors(robot::lDrive, robot::rDrive)
                    .withSensors(robot::lEnc, robot::rEnc, robot::mEnc)
                    .withOdometry()
                    .withGearset(okapi::AbstractMotor::gearset::green)
                    .withDimensions(okapi::ChassisScales({2.75_in, 10.125_in, 0_in, 2.75_in}, 360))
                    .withLogger(std::make_shared<okapi::Logger>(okapi::TimeUtilFactory::create().getTimer(), robot::logFile, okapi::Logger::LogLevel::debug))
                    .buildOdometry();

  pros::delay(10);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {

  while(true){

    pros::delay(10);
  }
}
