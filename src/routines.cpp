#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

uint16_t selection = 0;

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
