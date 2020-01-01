#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

int selection = 3;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "Disable",
  &lv_style_plain,
  [](screen::ttField *field) {
    field->finishDrawing(); 
  },
  [](){

  }
),

AutonomousRoutine(
  "Cube Push",
  &lv_style_plain,
  [](screen::ttField *field){
    field->finishDrawing();
  },
  [](){
    robot::chassis->getModel()->setMaxVelocity(100);
    robot::chassis->moveDistance(12_in);

    robot::chassis->moveDistance(-12_in);
    
    robot::chassis->getModel()->setMaxVelocity(200);
  }
)

};
