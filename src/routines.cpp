#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

uint16_t selection = 2;

double boostVoltage(double ivoltage, double boost){
  return ivoltage > 0 ? ivoltage + boost :
         ivoltage < 0 ? ivoltage - boost :
         0;
}

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
  "#ff0000 Red Unprot 6",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 5);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.1_ft, 0_ft, 0_deg}}, "1A", {0.77, 0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "1B", {0.15,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "1C", {1,0.5,10});
    okapi::IterativePosPIDController turningController({0.03, 0, 0.0015, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));
    
    robot::lift->moveMidTower();

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::lift->reset();
    robot::lift->waitUntilSettled();

    robot::chassisProfiler->setTarget("1A");
    robot::intake->moveVoltage(12000);   
    robot::chassisProfiler->waitUntilSettled();
    
    turningController.setTarget(-23);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled());
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("1B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(155);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      std::cout << robot::imu->get_rotation();
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 130);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1C");
    robot::intake->moveVoltage(-4000);
    pros::delay(750);
    
    robot::intake->moveVoltage(0);
    robot::chassisProfiler->waitUntilSettled();
    
    robot::tilter->stack();
    pros::delay(1500);

    robot::intake->moveVoltage(4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    pros::delay(1000);

    robot::tilter->reset();
    robot::intake->moveVoltage(-6000);
    robot::chassis->getModel()->forward(-0.3);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "#ff0000 Red Prot 4",
  [](screen::ttField *field){
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "2A", {1,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "2B", {0.77,0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.2_ft, 0_ft, 0_deg}}, "2C", {1,1,10});
    okapi::IterativePosPIDController turningController({0.03, 0, 0.0015, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));

    robot::lift->moveMidTower();

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::lift->reset();
    robot::lift->waitUntilSettled();

    robot::intake->moveVoltage(12000);

    robot::chassisProfiler->setTarget("2A");
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(90);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled());
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("2B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(230);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 180);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2C");
    robot::intake->moveVoltage(-4000);
    pros::delay(750);
    
    robot::intake->moveVoltage(0);
    robot::chassisProfiler->waitUntilSettled();

    robot::tilter->stack();
    pros::delay(1500);

    robot::intake->moveVoltage(4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    pros::delay(1000);

    robot::tilter->reset();
    robot::intake->moveVoltage(-6000);
    robot::chassis->getModel()->forward(-0.3);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "#0000ff Blue Unprot 6",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::right4, 0);
    field->draw(screen::scoringZone::nearBlue, screen::color::green, 5);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.1_ft, 0_ft, 0_deg}}, "1A", {0.77, 0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "1B", {0.15,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "1C", {1,0.5,10});
    okapi::IterativePosPIDController turningController({0.03, 0, 0.0015, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));
    
    robot::lift->moveMidTower();

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::lift->reset();
    robot::lift->waitUntilSettled();

    robot::chassisProfiler->setTarget("1A");
    robot::intake->moveVoltage(12000);   
    robot::chassisProfiler->waitUntilSettled();
    
    turningController.setTarget(23);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled());
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("1B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-155);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      std::cout << robot::imu->get_rotation();
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -130);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1C");
    robot::intake->moveVoltage(-4000);
    pros::delay(750);
    
    robot::intake->moveVoltage(0);
    robot::chassisProfiler->waitUntilSettled();

    robot::tilter->stack();
    pros::delay(1500);

    robot::intake->moveVoltage(4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    pros::delay(1000);

    robot::tilter->reset();
    robot::intake->moveVoltage(-6000);
    robot::chassis->getModel()->forward(-0.3);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "#0000ff Blue Prot 4",
  [](screen::ttField *field){
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "2A", {1,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}}, "2B", {0.77,0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.2_ft, 0_ft, 0_deg}}, "2C", {1,1,10});
    okapi::IterativePosPIDController turningController({0.03, 0, 0.0015, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));

    robot::lift->moveMidTower();

    robot::chassis->getModel()->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::lift->reset();
    robot::lift->waitUntilSettled();

    robot::intake->moveVoltage(12000);

    robot::chassisProfiler->setTarget("2A");
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-90);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled());
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("2B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-230);
    do{
      robot::chassis->getModel()->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -180);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2C");
    robot::intake->moveVoltage(-4000);
    pros::delay(750);
    
    robot::intake->moveVoltage(0);
    robot::chassisProfiler->waitUntilSettled();

    robot::tilter->stack();
    pros::delay(1500);

    robot::intake->moveVoltage(4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    pros::delay(1000);

    robot::tilter->reset();
    robot::intake->moveVoltage(-6000);
    robot::chassis->getModel()->forward(-0.3);
    pros::delay(1000);

    robot::chassis->getModel()->stop();
    robot::intake->moveVoltage(0);
  }
),

};
