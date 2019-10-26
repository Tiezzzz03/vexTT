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
  [](){
    robot::angler->getTask()->suspend();
    robot::angler->getMotor()->moveVoltage(12000);
    robot::intake->moveVoltage(-10000);
    while(robot::angler->getMotor()->getPosition() + 100 < Angler::verticalPos) {pros::delay(10);}

    robot::angler->getTask()->resume();
    robot::angler->reset();
    robot::intake->moveVoltage(10000);
    while(!robot::angler->isSettled()) {pros::delay(10);}

    robot::intake->moveVoltage(0);
  }
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
