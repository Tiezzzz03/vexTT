#pragma once
#include "main.h"
#include "mechanism/tilter.hpp"
#include "mechanism/lift.hpp"
#include <atomic>

enum class screenMode{
  disabled, notification, selection, ez, NUMPANELS
};

namespace robot {
extern okapi::Controller controller;

extern std::shared_ptr<Lift> lift;
extern std::shared_ptr<Tilter> tilter;
extern std::shared_ptr<okapi::MotorGroup> intake;
extern std::shared_ptr<okapi::Motor> flDrive;
extern std::shared_ptr<okapi::Motor> rlDrive;
extern std::shared_ptr<okapi::Motor> frDrive;
extern std::shared_ptr<okapi::Motor> rrDrive;

extern std::shared_ptr<pros::Imu> imu;

extern std::shared_ptr<okapi::XDriveModel> chassis;
extern std::shared_ptr<okapi::ChassisController> chassisController;
extern std::shared_ptr<okapi::AsyncMotionProfileController> chassisProfiler;

namespace screen {
  extern pros::Task *controller;
  extern volatile std::atomic<screenMode> state;
  extern std::string notification;
}
}
