#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

int selection = 0;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "Disable",
  &lv_style_plain,
  [](screen::ttField *field) {
    field->finishDrawing(); 
  },
  [](){
    robot::intake->moveVoltage(-12000);
    pros::delay(1000);

    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "Small Zone Red",
  &screen::resources::redText,
  [](screen::ttField *field) {
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 5);
    field->finishDrawing();
  },
  [](){
    robot::intake->moveVoltage(-12000);
    pros::delay(500);

    //robot::chassisProfiler->loadPath("/usd/tt/paths/cubeAdvance", "cubeAdvance"); //60% speed, low accel, ~40_in
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {40_in, 0_in, 0_deg}}, "cubeAdvance", {0.4, 1, 5.0});
    
    robot::chassisProfiler->setTarget("cubeAdvance");
    pros::delay(500);

    robot::intake->moveVoltage(12000);
    robot::chassisProfiler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(60);
    robot::chassis->turnAngleAsync(115_deg);
    //robot::chassisProfiler->loadPath("/usd/tt/paths/smallZoneAdvance", "zoneAdvance"); //60% speed, low accel, ~48_in
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {48_in, 0_in, 0_deg}}, "zoneAdvance", {0.6, 0.8, 5.0});

    robot::chassis->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(200);
    robot::chassisProfiler->setTarget("zoneAdvance");
    robot::angler->prime();
    robot::chassisProfiler->waitUntilSettled();

    robot::angler->stack();
    robot::angler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(80);
    robot::chassis->moveDistance(-6_in);
    robot::chassis->getModel()->setMaxVelocity(200);
  }
)

};
