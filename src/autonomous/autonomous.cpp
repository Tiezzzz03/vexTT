#include "main.h"
#include "robot.hpp"
#include "autonomous/routines.hpp"

int selection = 0;

void competition_initialize() {
  robot::screen::state = screenMode::selection;
}

void autonomous() {
  autonomousRoutines.at(selection).run();
}
