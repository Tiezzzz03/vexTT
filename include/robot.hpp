#pragma once
#include "main.h"
#include "mechanisms/angler.hpp"
#include <memory>

namespace robot {
  namespace config {
  }

  extern okapi::Controller master;

  extern std::shared_ptr<okapi::SkidSteerModel> chassis;
  extern std::shared_ptr<okapi::MotorGroup> intake;
  extern std::shared_ptr<okapi::Motor> lift;
  extern std::shared_ptr<Angler> angler;

  enum class IntakeStatus {
    on, off, stacking
  };
}
