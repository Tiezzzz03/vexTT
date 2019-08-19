#pragma once
#include "main.h"
#include "mechanisms/angler.hpp"
#include "mechanisms/intake.hpp"
#include <memory>

namespace robot {
namespace config {
}

extern okapi::Controller master;

extern std::shared_ptr<okapi::SkidSteerModel> chassis;
extern std::shared_ptr<Intake> intake;
extern std::shared_ptr<okapi::Motor> lift;
extern std::shared_ptr<Angler> angler;

}
