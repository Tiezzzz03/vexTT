#include "robot.hpp"

using namespace okapi::literals;

extern void screenControllerFN(void* param);

namespace robot {

okapi::Controller controller;

std::shared_ptr<Lift> lift;
std::shared_ptr<Tilter> tilter;
std::shared_ptr<okapi::MotorGroup> intake;
std::shared_ptr<okapi::Motor> flDrive;
std::shared_ptr<okapi::Motor> rlDrive;
std::shared_ptr<okapi::Motor> frDrive;
std::shared_ptr<okapi::Motor> rrDrive;

std::shared_ptr<pros::Imu> imu;

std::shared_ptr<okapi::XDriveModel> chassis;
std::shared_ptr<okapi::ChassisController> chassisController;
std::shared_ptr<okapi::AsyncMotionProfileController> chassisProfiler;

namespace screen {
  pros::Task *controller;
  volatile std::atomic<screenMode> state;
  std::string notification;
}

}

std::atomic_int Lift::restingPos     = 0;
std::atomic_int Lift::lowTowerPos    = 1400;
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
    okapi::IterativePosPIDController::Gains({0.0005, 0, 0, 0}));
  
  robot::lift = std::make_shared<Lift>(
    std::make_shared<okapi::Motor>(20),
    std::make_shared<okapi::ADIButton>(1), std::make_shared<okapi::ADIButton>(2),
    okapi::IterativePosPIDController::Gains({55, 0, 1, 0}));

  robot::intake = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({ 10, -18}));
  
  robot::flDrive = std::make_shared<okapi::Motor>(5);
  robot::rlDrive = std::make_shared<okapi::Motor>(4);
  robot::frDrive = std::make_shared<okapi::Motor>(-2);
  robot::rrDrive = std::make_shared<okapi::Motor>(-3);

  robot::imu = std::make_shared<pros::Imu>(11);
  robot::imu->reset();
  uint32_t calibrationTime = pros::millis() + 2200;

  robot::chassis = std::make_shared<okapi::XDriveModel>(robot::flDrive, robot::frDrive, robot::rrDrive, robot::rlDrive,
                                                        std::make_shared<okapi::ADIEncoder>(0,0), std::make_shared<okapi::ADIEncoder>(0,0),
                                                        200, 12000);

  robot::chassisController = std::make_shared<okapi::ChassisControllerIntegrated>(
    okapi::TimeUtilFactory::createDefault(),
    robot::chassis,
    std::make_unique<okapi::AsyncPosIntegratedController>(
      std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::flDrive, robot::rlDrive})),
      okapi::AbstractMotor::gearset::green,
      200,
      okapi::TimeUtilFactory::createDefault(),
      okapi::Logger::getDefaultLogger()),
    std::make_unique<okapi::AsyncPosIntegratedController>(
      std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::frDrive, robot::rrDrive})),
      okapi::AbstractMotor::gearset::green,
      200,
      okapi::TimeUtilFactory::createDefault(),
      okapi::Logger::getDefaultLogger()),
    okapi::AbstractMotor::gearset::green,
    okapi::ChassisScales({4_in, 10_in}, okapi::imev5GreenTPR),
    okapi::Logger::getDefaultLogger());

  robot::chassisProfiler = okapi::AsyncMotionProfileControllerBuilder()
                      .withOutput(robot::chassisController)
                      .withLimits({1, 2, 10})
                      .buildMotionProfileController();


  robot::intake->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  robot::tilter->getMotor()->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  robot::lift->getMotor()->setGearing(okapi::AbstractMotor::gearset::red);

  robot::tilter->startThread();
  robot::lift->startThread();
  robot::screen::controller = new pros::Task(screenControllerFN, NULL, "Screen");
  robot::screen::notification = "Get Your Stickers!";
  while(robot::imu->is_calibrating()) {pros::delay(100);}
}
