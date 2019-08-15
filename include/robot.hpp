#pragma once
#include "main.h"
#include <memory>

namespace robot {
  namespace config {
    extern volatile int anglerActivePos;
    extern volatile int anglerStackingPos;
  }

  extern okapi::Controller master;

  extern std::shared_ptr<okapi::SkidSteerModel> chassis;
  extern std::shared_ptr<okapi::MotorGroup> intake;
  extern std::shared_ptr<okapi::Motor> lift;
  extern std::shared_ptr<okapi::Motor> angler;

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
  }

  enum class IntakeStatus {
    on, off, stacking
  };

  enum class AnglerStatus {
    initial, active, stacking
  };

  void resetAngler();
}
