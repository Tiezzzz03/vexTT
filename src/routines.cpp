#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

uint16_t selection = 2;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "#000000 Disable#",
  [](screen::ttField *field){
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
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.5_ft, 0_ft, 0_deg}}, "CubeApproach", {0.5,0.5,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "ZoneApproach", {0.5,0.5,10});
    //okapi::IterativePosPIDController turningController({.001, 0, 0, 0}, okapi::TimeUtilFactory::createDefault());

    robot::lift->moveMidTower();
    robot::chassis->getModel()->driveVectorVoltage(0.5, 0);
    pros::delay(500);

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(500);

    robot::lift->reset();
    pros::delay(500);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(12000);
    pros::delay(500);

    robot::chassisProfiler->setTarget("CubeApproach");
    robot::chassisProfiler->waitUntilSettled();

    //robot::gyro.reset();
    //turningController.setTarget(150);
    //while(!turningController.isSettled()){
      //robot::chassis->getModel()->rotate(turningController.step(robot::gyro.get_rotation()));
      //pros::delay(10);
    //}
    robot::chassis->getModel()->setMaxVelocity(40);
    robot::chassis->turnAngle(162_deg);
    robot::chassis->getModel()->setMaxVelocity(200);

    robot::chassisProfiler->setTarget("ZoneApproach");
    robot::chassisProfiler->waitUntilSettled();

    robot::intake->moveVoltage(0);
    robot::tilter->stack();
    pros::delay(3000);

    robot::chassis->getModel()->forward(-0.4);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
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
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.5_ft, 0_ft, 0_deg}}, "CubeApproach", {0.5,0.5,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "ZoneApproach", {0.5,0.5,10});
    //okapi::IterativePosPIDController turningController({.001, 0, 0, 0}, okapi::TimeUtilFactory::createDefault());

    robot::lift->moveMidTower();
    robot::chassis->getModel()->driveVectorVoltage(0.5, 0);
    pros::delay(500);

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(500);

    robot::lift->reset();
    pros::delay(500);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(12000);
    pros::delay(500);

    robot::chassisProfiler->setTarget("CubeApproach");
    robot::chassisProfiler->waitUntilSettled();

    //robot::gyro.reset();
    //turningController.setTarget(150);
    //while(!turningController.isSettled()){
      //robot::chassis->getModel()->rotate(turningController.step(robot::gyro.get_rotation()));
      //pros::delay(10);
    //}
    robot::chassis->getModel()->setMaxVelocity(40);
    robot::chassis->turnAngle(-162_deg);
    robot::chassis->getModel()->setMaxVelocity(200);

    robot::chassisProfiler->setTarget("ZoneApproach");
    robot::chassisProfiler->waitUntilSettled();

    robot::intake->moveVoltage(0);
    robot::tilter->stack();
    pros::delay(3000);

    robot::chassis->getModel()->forward(-0.4);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
  }
)

};
