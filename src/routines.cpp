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
    robot::chassis->moveDistance(4_in);
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
  [](){},
  [](){
    robot::chassis->getModel()->setMaxVelocity(75);
    robot::chassis->moveDistance(-5_ft);
    pros::delay(500);
    robot::chassis->moveDistance(1_ft);

    /*robot::angler->getTask()->suspend();
    robot::angler->getMotor()->moveVoltage(12000);
    robot::intake->moveVoltage(-10000);
    robot::chassis->moveDistance(4_in);
    while(robot::angler->getMotor()->getPosition() + 100 < Angler::verticalPos) {pros::delay(10);}

    robot::angler->getTask()->resume();
    robot::angler->reset();
    robot::intake->moveVoltage(10000);
    while(!robot::angler->isSettled()) {pros::delay(10);}

    robot::chassis->setMaxVelocity(100);
    robot::chassis->moveDistance(4_ft);

    robot::intake->moveVoltage(0);
    robot::chassis->turnAngle()

    robot::chassisProfiler->setTarget("nearZoneApproach");
    robot::chassisProfiler->waitUntilSettled();

    robot::angler->stack();
    while(!robot::angler->isSettled()) {pros::delay(10);}

    robot::chassis->moveDistance(-1_ft);*/
  }
)

};
