#include "routines.hpp"
#include "robot.hpp"

using namespace okapi::literals;

uint16_t selection = 6;

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
    robot::chassis->setMaxVelocity(100);
    robot::chassisController->moveDistance(12_in);

    robot::chassisController->moveDistance(-12_in);
    
    robot::chassis->setMaxVelocity(200);
  }
),

AutonomousRoutine(
  "#ff0000 Red Unprot 6",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::tower::near, screen::color::none, 0b1110);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 6);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.15_ft, 0_ft, 0_deg}}, "1A", {0.8, 0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {0.75_ft, 0_ft, 0_deg}}, "1B", {0.45,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.4_ft, -1.5_ft, -45_deg}}, "1C", {0.8,0.65,10});
    okapi::IterativePosPIDController turningController({0.031, 0, 0.002, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3, 10, 10_ms));
    
    robot::lift->moveMidTower();

    robot::chassis->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->stop();
    robot::lift->reset();
    pros::delay(250);
    robot::lift->waitUntilSettled();

    robot::chassisProfiler->setTarget("1A");
    robot::intake->moveVoltage(12000);   
    robot::chassisProfiler->waitUntilSettled();
    
    turningController.setTarget(-25);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -10);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1B");
    robot::chassisProfiler->waitUntilSettled();

    robot::chassisProfiler->setTarget("1B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(180);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 130);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1C", false, true);
    robot::intake->moveVoltage(-4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    robot::tilter->prime();
    robot::chassisProfiler->waitUntilSettled();
    
    robot::chassis->driveVectorVoltage(1,0);
    robot::tilter->stack();
    pros::delay(1000);

    robot::chassis->stop();
    robot::tilter->waitUntilSettled();

    robot::intake->moveVoltage(-6000);
    pros::delay(500);

    robot::chassis->driveVectorVoltage(-0.4, 0);
    robot::tilter->reset();
    pros::delay(1000);

    robot::chassis->stop();
    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "#ff0000 Red Prot 4",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::farLeft, 0);
    field->draw(screen::tower::left, screen::color::none, 0b0011);
    field->draw(screen::scoringZone::farRed, screen::color::orange, 4);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "2A", {1,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.6_ft, 0_ft, 0_deg}}, "2B", {0.85,0.8,10});
    okapi::IterativePosPIDController turningController({0.022, 0, 0.0007, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));

    robot::lift->moveMidTower();

    robot::chassis->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->stop();
    robot::lift->reset();
    pros::delay(250);
    robot::lift->waitUntilSettled();
    robot::intake->moveVoltage(12000);

    robot::chassisProfiler->setTarget("2A");
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(90);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 45);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("2B", true);
    robot::intake->moveVoltage(-4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    robot::tilter->prime();
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(230);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 180);
    robot::chassis->stop();

    robot::chassis->driveVectorVoltage(0.5, 0);
    pros::delay(750);

    robot::chassis->stop();
    robot::chassis->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
    robot::tilter->stack();
    pros::delay(500);
    robot::tilter->waitUntilSettled();

    robot::intake->moveVoltage(-6000);
    pros::delay(1000);

    robot::chassis->driveVectorVoltage(-0.4, 0);
    robot::tilter->reset();
    pros::delay(1000);

    robot::chassis->stop();
    robot::intake->moveVoltage(0);

    robot::chassis->setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  }
),

AutonomousRoutine(
  "#0000ff Blue Unprot 6",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::right4, 0);
    field->draw(screen::tower::near, screen::color::none, 0b1011);
    field->draw(screen::scoringZone::nearBlue, screen::color::green, 6);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.15_ft, 0_ft, 0_deg}}, "1A", {0.8, 0.8,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {0.75_ft, 0_ft, 0_deg}}, "1B", {0.45,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.4_ft, 1.5_ft, 45_deg}}, "1C", {0.8,0.65,10});
    okapi::IterativePosPIDController turningController({0.031, 0, 0.002, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3, 10, 10_ms));
    
    robot::lift->moveMidTower();

    robot::chassis->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->stop();
    robot::lift->reset();
    pros::delay(250);
    robot::lift->waitUntilSettled();

    robot::chassisProfiler->setTarget("1A");
    robot::intake->moveVoltage(12000);   
    robot::chassisProfiler->waitUntilSettled();
    
    turningController.setTarget(25);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 10);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1B");
    robot::chassisProfiler->waitUntilSettled();

    robot::chassisProfiler->setTarget("1B", true);
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-180);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -130);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("1C", false, true);
    robot::intake->moveVoltage(-4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    robot::tilter->prime();
    robot::chassisProfiler->waitUntilSettled();
    
    robot::chassis->driveVectorVoltage(1,0);
    robot::tilter->stack();
    pros::delay(1000);

    robot::chassis->stop();
    robot::tilter->waitUntilSettled();

    robot::intake->moveVoltage(-6000);
    pros::delay(500);

    robot::chassis->driveVectorVoltage(-0.4, 0);
    robot::tilter->reset();
    pros::delay(1000);

    robot::chassis->stop();
    robot::intake->moveVoltage(0);
  }
),

AutonomousRoutine(
  "#0000ff Blue Prot 4",
  [](screen::ttField *field){
    field->draw(screen::cubeGroup::farRight, 0);
    field->draw(screen::tower::right, screen::color::none, 0b0110);
    field->draw(screen::scoringZone::farBlue, screen::color::green, 4);
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "2A", {1,1,10});
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.6_ft, 0_ft, 0_deg}}, "2B", {0.85,0.8,10});
    okapi::IterativePosPIDController turningController({0.022, 0, 0.0007, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3));

    robot::lift->moveMidTower();

    robot::chassis->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->stop();
    robot::lift->reset();
    pros::delay(250);
    robot::lift->waitUntilSettled();
    robot::intake->moveVoltage(12000);

    robot::chassisProfiler->setTarget("2A");
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-90);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -45);
    robot::chassis->stop();

    robot::chassisProfiler->setTarget("2B");
    robot::chassisProfiler->waitUntilSettled();

    pros::delay(500);

    robot::chassisProfiler->setTarget("2B", true);
    robot::intake->moveVoltage(-4000);
    pros::delay(500);

    robot::intake->moveVoltage(0);
    robot::tilter->prime();
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(-230);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() > -180);
    robot::chassis->stop();

    robot::chassis->driveVectorVoltage(0.5, 0);
    pros::delay(750);

    robot::chassis->stop();
    robot::chassis->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
    robot::tilter->stack();
    pros::delay(500);
    robot::tilter->waitUntilSettled();

    robot::intake->moveVoltage(-6000);
    pros::delay(1000);

    robot::chassis->driveVectorVoltage(-0.4, 0);
    robot::tilter->reset();
    pros::delay(1000);

    robot::chassis->stop();
    robot::intake->moveVoltage(0);

    robot::chassis->setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  }
),

AutonomousRoutine(
  "#ff8800 Skills",
  [](screen::ttField *field){
    field->finishDrawing();
  },
  [](){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3_ft, 0_ft, 0_deg}, {5_ft, -0.5_ft, 0_deg}, {7_ft, 0_ft, 0_deg}, {9_ft, 0_ft, 0_deg}}, "SA", {0.5,1,10}); 
    okapi::IterativePosPIDController turningController({0.031, 0, 0.002, 0}, okapi::TimeUtilFactory::withSettledUtilParams(3, 10, 10_ms));

    robot::lift->moveMidTower();

    robot::chassis->driveVectorVoltage(-0.5, 0);
    pros::delay(1000);

    robot::chassis->stop();
    robot::lift->reset();
    pros::delay(250);
    robot::lift->waitUntilSettled();
    robot::intake->moveVoltage(12000);

    robot::chassisProfiler->setTarget("SA");
    robot::chassisProfiler->waitUntilSettled();

    turningController.setTarget(45);
    do{
      robot::chassis->driveVectorVoltage(0, boostVoltage(turningController.step(robot::imu->get_rotation()), 0.2));
      pros::delay(20);
    }while(!turningController.isSettled() || robot::imu->get_rotation() < 20);
    robot::chassis->stop();

    robot::intake->moveVoltage(-4000);
    robot::chassis->driveVectorVoltage(0.5,0);
    pros::delay(400);

    robot::intake->moveVoltage(0);    
    robot::tilter->stack();
    pros::delay(1000);

    robot::chassis->stop();
    robot::tilter->waitUntilSettled();
    pros::delay(500);

    robot::intake->moveVoltage(-6000);
    pros::delay(500);

    robot::chassis->driveVectorVoltage(-0.4, 0);
    robot::tilter->reset();
    pros::delay(1000);

    robot::chassis->stop();
    robot::intake->moveVoltage(0);
  }
)

};
