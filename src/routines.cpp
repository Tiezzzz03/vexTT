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
    robot::intake->moveVoltage(-12000);
    pros::delay(1000);

    robot::intake->moveVoltage(0);
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
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {36_in, 0_in, 0_deg}}, "cubeAdvance", {0.4, 1, 5.0});
    robot::chassis->moveDistance(6_in);
    robot::chassis->moveDistance(-2_in);

    robot::chassisProfiler->setTarget("cubeAdvance");
    robot::intake->moveVoltage(12000);
    robot::chassisProfiler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(60);
    robot::chassis->turnAngleAsync(152_deg);
    //robot::chassisProfiler->loadPath("/usd/tt/paths/smallZoneAdvance", "zoneAdvance"); //60% speed, low accel, ~48_in
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {48_in, 0_in, 0_deg}}, "zoneAdvance", {0.6, 0.8, 5.0});

    robot::chassis->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(200);
    robot::chassisProfiler->setTarget("zoneAdvance");
    robot::intake->moveVoltage(0);
    robot::angler->prime();
    robot::chassisProfiler->waitUntilSettled();

    robot::angler->stack();
    robot::angler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(80);
    robot::chassis->moveDistance(-6_in);
    robot::angler->prime();

    robot::chassis->getModel()->setMaxVelocity(200);
  }
),

AutonomousRoutine(
  "Large Zone Blue",
  &screen::resources::redText,
  [](screen::ttField *field) {},
  [](){
    robot::intake->moveVoltage(-12000);
    pros::delay(500);

    //robot::chassisProfiler->loadPath("/usd/tt/paths/cubeAdvance", "cubeAdvance"); //60% speed, low accel, ~40_in
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {36_in, 0_in, 0_deg}}, "cubeAdvance", {1, 2, 10});
    robot::chassis->getModel()->setMaxVelocity(100);
    robot::chassis->moveDistance(9_in);
    robot::chassis->moveDistance(-4_in);

    robot::chassis->getModel()->setMaxVelocity(200);
    robot::angler->prime();
    robot::chassisProfiler->setTarget("cubeAdvance");
    robot::intake->moveVoltage(6000);
    robot::chassisProfiler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(60);
    robot::chassis->turnAngleAsync(130_deg);
    robot::intake->moveVoltage(12000);
    //robot::chassisProfiler->loadPath("/usd/tt/paths/smallZoneAdvance", "zoneAdvance"); //60% speed, low accel, ~48_in
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {40.5_in, 0_in, 0_deg}}, "zoneAdvance", {0.6, 0.8, 5.0});

    robot::chassis->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(200);
    robot::chassisProfiler->setTarget("zoneAdvance");
    robot::angler->prime();
    robot::chassisProfiler->waitUntilSettled();
    robot::intake->moveVoltage(0);

    robot::angler->stack();
    pros::delay(100);
    robot::angler->waitUntilSettled();

    robot::chassis->getModel()->setMaxVelocity(80);
    robot::chassis->moveDistance(-6_in);
    robot::angler->prime();

    robot::chassis->getModel()->setMaxVelocity(200);
  }
)

};
