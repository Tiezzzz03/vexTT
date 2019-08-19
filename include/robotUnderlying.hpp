#pragma once
#include "main.h"
#include <memory>

namespace robot {
namespace underlying {
  extern okapi::Motor leftDriveF;
  extern okapi::Motor leftDriveR;
  extern okapi::Motor rightDriveF;
  extern okapi::Motor rightDriveR;
  extern okapi::Motor lIntake;
  extern okapi::Motor rIntake;

  //extern okapi::ADIEncoder leftEnc;
  //extern okapi::ADIEncoder middleEnc;
  //extern okapi::ADIEncoder rightEnc;

  extern std::shared_ptr<okapi::Motor> angler;
  extern std::shared_ptr<okapi::MotorGroup> intake;
}
}
