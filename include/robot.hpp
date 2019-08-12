#pragma once
#include "main.h"
#include <memory>

namespace robot {
  extern okapi::Controller master;

  extern std::shared_ptr<okapi::Motor> leftDriveF;
  extern std::shared_ptr<okapi::Motor> leftDriveR;
  extern std::shared_ptr<okapi::Motor> rightDriveF;
  extern std::shared_ptr<okapi::Motor> rightDriveR;
  extern std::shared_ptr<okapi::Motor> lift;
  extern std::shared_ptr<okapi::Motor> angler;
  extern std::shared_ptr<okapi::Motor> lIntake;
  extern std::shared_ptr<okapi::Motor> rIntake;
}
