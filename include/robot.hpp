#pragma once
#include "main.h"
#include "mechanisms/angler.hpp"
#include "mechanisms/intake.hpp"
#include <memory>

enum class screenMode{
  disabled, notification, selection, diagnostic, sans, logo
};

namespace robot {

extern okapi::Controller master;

extern std::shared_ptr<okapi::SkidSteerModel> chassis;
extern std::shared_ptr<mechanism::Intake> intake;
extern std::shared_ptr<mechanism::Angler> angler;


namespace screen{
  extern pros::Task *controller;
  extern volatile screenMode state;
  extern std::string notification;
};

}
