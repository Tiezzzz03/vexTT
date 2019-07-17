#include "main.h"
#include "robot.hpp"

namespace robot {
  okapi::Controller master;

  std::shared_ptr<okapi::Motor> leftDriveF;
  std::shared_ptr<okapi::Motor> leftDriveR;
  std::shared_ptr<okapi::Motor> rightDriveF;
  std::shared_ptr<okapi::Motor> rightDriveR;
  std::shared_ptr<okapi::Motor> lift;
  std::shared_ptr<okapi::Motor> angler;
}
