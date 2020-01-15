#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

uint16_t selection = 0;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "#000000 Disable#",
  [](screen::ttField *field) {
    field->finishDrawing(); 
  },
  [](){

  }
),

AutonomousRoutine(
  "#ff00ff Cube Push#",
  [](screen::ttField *field){
    field->draw(screen::scoringZone::farRed, screen::color::orange, 1);
    field->finishDrawing();
  },
  [](){
    robot::chassis->getModel()->setMaxVelocity(100);
    robot::chassis->moveDistance(12_in);

    robot::chassis->moveDistance(-12_in);
    
    robot::chassis->getModel()->setMaxVelocity(200);
  }
),

AutonomousRoutine(
  "#ff0000 Red Small Zone 5",
  [](screen::ttField *field){
    field->finishDrawing();
  },
  [](){

  }
),

AutonomousRoutine(
  "#0000ff Blue Small Zone 5",
  [](screen::ttField *field){
    field->finishDrawing();
  },
  [](){

  }
)

};
