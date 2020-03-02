#include "robot.hpp"

using namespace okapi::literals;

extern void screenControllerFN(void* param);

namespace robot {

okapi::Controller controller;

std::shared_ptr<Lift> lift;
std::shared_ptr<Tilter> tilter;
std::shared_ptr<okapi::MotorGroup> intake;
std::shared_ptr<okapi::MotorGroup> lDrive;
std::shared_ptr<okapi::MotorGroup> rDrive;

std::shared_ptr<pros::Imu> imu;

std::shared_ptr<okapi::ChassisController> chassis;
std::shared_ptr<okapi::AsyncMotionProfileController> chassisProfiler;

namespace screen {
  pros::Task *controller;
  volatile std::atomic<screenMode> state;
  std::string notification;
}

}

std::atomic_int Lift::restingPos     = 0;
std::atomic_int Lift::lowTowerPos    = 1550;
std::atomic_int Lift::midTowerPos    = 2000;

std::atomic_int Tilter::restingPos   = 50;
std::atomic_int Tilter::readyLiftPos = 50;
std::atomic_int Tilter::pidThreshold = 2200;
std::atomic_int Tilter::primePos     = 2300;
std::atomic_int Tilter::verticalPos  = 3950;


void initialize() {
  okapi::Logger::setDefaultLogger(std::make_shared<okapi::Logger>(std::make_unique<okapi::Timer>(), "/ser/sout", okapi::Logger::LogLevel::debug));

  robot::tilter = std::make_shared<Tilter>(
    std::make_shared<okapi::Motor>(12),
    okapi::IterativePosPIDController::Gains({0.002, 0, 0.00006, 0}));
  
  robot::lift = std::make_shared<Lift>(
    std::make_shared<okapi::Motor>(20),
    std::make_shared<okapi::ADIButton>(2),
    okapi::IterativePosPIDController::Gains({55, 0, 1, 0}));

  robot::intake = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({-17, 18}));
  robot::lDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({  4, -5}));
  robot::rDrive = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({  2, -3}));

  robot::imu = std::make_shared<pros::Imu>(11);
  robot::imu->reset();
  uint32_t calibrationTime = pros::millis() + 2200;

  robot::chassis = okapi::ChassisControllerBuilder()
                      .withMotors(robot::lDrive, robot::rDrive)
                      .withDimensions(okapi::AbstractMotor::gearset::green, okapi::ChassisScales({5.42_in, 10_in}, okapi::imev5GreenTPR))
                      .build();

  robot::chassisProfiler = okapi::AsyncMotionProfileControllerBuilder()
                      .withOutput(robot::chassis)
                      .withLimits({1, 2, 10})
                      .buildMotionProfileController();


  robot::intake->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  robot::tilter->getMotor()->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  robot::lift->getMotor()->setGearing(okapi::AbstractMotor::gearset::red);

  robot::tilter->startThread();
  robot::lift->startThread();
  robot::screen::controller = new pros::Task(screenControllerFN, NULL, "Screen");
  robot::screen::notification = "Get Your Stickers!";
  while(!robot::imu->is_calibrating()) {pros::delay(100);}
}
