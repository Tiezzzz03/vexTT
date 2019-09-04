#include "main.h"
#include "autonomous/autonomousRoutine.hpp"
#include "autonomous/routines.hpp"
#include "screen/field.hpp"
#include "screen/resources.hpp"
#include "robot.hpp"
#include <vector>

void disableAuton(){}

void disabledPrint(screen::Field &field){
  field.finishDrawing();
}

autonomousRoutine disable("Disable - 0S", &screen::whiteText, disableAuton, disabledPrint);

std::vector<autonomousRoutine> autonomousRoutines = {disable};
