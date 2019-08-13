#include "main.h"
#include "robot.hpp"

namespace robot {
  okapi::Controller master;

  std::shared_ptr<okapi::SkidSteerModel> chassis;
  std::shared_ptr<okapi::MotorGroup> intake;
  std::shared_ptr<okapi::Motor> lift;
  std::shared_ptr<okapi::Motor> angler;

  namespace underlying {
    okapi::Motor leftDriveF(-10);
    okapi::Motor leftDriveR(5);
    okapi::Motor rightDriveF(20);
    okapi::Motor rightDriveR(-15);
    okapi::Motor lIntake(-8);
    okapi::Motor rIntake(18);
  }
}
