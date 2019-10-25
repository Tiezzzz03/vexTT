#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

int selection = 0;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "Disable",
  &lv_style_plain,
  [](screen::ttField *field) { field->finishDrawing(); },
  [](){},
  [](){}
),

AutonomousRoutine(
  "Small Zone Red",
  &screen::resources::redAlliance,
  [](screen::ttField *field) {
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 5);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {4_ft, 0_ft, 0_deg}}, "nearCubeApproach", {0.5, 2, 10});
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {3_ft, -1_ft, -45_deg}}, "nearCubeRecede");
    robot::chassisProfiler->generatePath({{0_in, 0_in, 45_deg}, {0_ft, 2_ft, 135_deg}}, "nearZoneApproach");
  },
  [](){
    robot::intake->moveVoltage(12000);
    robot::chassisProfiler->setTarget("nearCubeApproach");
    robot::chassisProfiler->waitUntilSettled();

    robot::intake->moveVoltage(0);
    robot::chassisProfiler->setTarget("nearCubeRecede", true);
    robot::chassisProfiler->waitUntilSettled();

    robot::chassisProfiler->setTarget("nearZoneApproach");
    robot::chassisProfiler->waitUntilSettled();

    robot::angler->stack();
  }
)

};
