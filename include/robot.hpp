#pragma once
#include "main.h"
#include "mechanisms/angler.hpp"
#include "mechanisms/intake.hpp"
#include <memory>

namespace robot {

extern okapi::Controller master;

extern std::shared_ptr<okapi::SkidSteerModel> chassis;
extern std::shared_ptr<mechanism::Intake> intake;
extern std::shared_ptr<mechanism::Angler> angler;

}
