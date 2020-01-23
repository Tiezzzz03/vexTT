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
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 1);
    field->draw(screen::scoringZone::farBlue, screen::color::green, 1);
    field->draw(screen::scoringZone::nearBlue, screen::color::green, 1);
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
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 5);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "CubeApproach", {0.8,1,10});

    robot::intake->moveVoltage(-12000);
    pros::delay(250);

    robot::lift->moveMidTower();
    robot::chassis->moveDistanceAsync(-2_in);
    robot::lift->waitUntilSettled();

    robot::lift->reset();
    pros::delay(250);

    robot::intake->moveVoltage(12000);
    pros::delay(250);

    robot::chassisProfiler->setTarget("CubeApproach");
    robot::chassisProfiler->waitUntilSettled();

    
  }
),

AutonomousRoutine(
  "#0000ff Blue Small Zone 5",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::right4, 0);
    field->draw(screen::scoringZone::nearBlue, screen::color::green, 5);
    field->finishDrawing();
  },
  [](){

  }
)

};
