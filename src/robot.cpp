#include "main.h"
#include "robot.hpp"
#include "robotUnderlying.hpp"

namespace robot {
  namespace config {
  }

  okapi::Controller master;

  std::shared_ptr<okapi::SkidSteerModel> chassis;
  std::shared_ptr<okapi::MotorGroup> intake;
  std::shared_ptr<okapi::Motor> lift;
  std::shared_ptr<Angler> angler;

  namespace underlying {
    okapi::Motor leftDriveF(-10);
    okapi::Motor leftDriveR(5);
    okapi::Motor rightDriveF(20);
    okapi::Motor rightDriveR(-15);
    okapi::Motor lIntake(-8);
    okapi::Motor rIntake(18);

    std::shared_ptr<okapi::Motor> angler;

    //okapi::ADIEncoder leftEnc(1,2);
    //okapi::ADIEncoder middleEnc(5,6);
    //okapi::ADIEncoder rightEnc(7,8);
  }
}
