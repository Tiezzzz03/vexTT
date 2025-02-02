#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

using namespace okapi::literals;

// defined in pathGen.cpp, will generate and rewrite all paths on the sd card
extern void generatePaths();

void disabled() {}

void competition_initialize() {
  /* this funciton runs immediatly after initialization when plugged into a
   * competetion / tournament switch
   *
   * setting this will bring the thread defined in screenController.cpp to
   * autonomous selection mode
  **/
  robot::screen::state = screenMode::selection;
}

void autonomous() {
  /* this function is run when directed by a competition / tournament switch
   * this will follow the function pointer "run" to the selected
   * AutonomousRoutines object. All of these routines are defined in routines.cpp
   * 
   * This also records the time taken and displays the time to the screen, for diagnostic purposes
  **/

  uint32_t startTime = pros::millis();

  routines[selection].run();

  robot::screen::notification = "Routine Duration: " + std::to_string(pros::millis() - startTime);
  robot::screen::state = screenMode::notification;
}

void opcontrol() {
  // this function runs when directed by a competition / tournament switch
  // or when not connected to either
  
  // ensure there is no velocity cap on the chassis
  robot::chassis->getModel()->setMaxVelocity(200);

  // ensure the brake mode is set to coast
  robot::chassis->getModel()->setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  
  // set the screen to display ez logo gif
  robot::screen::state = screenMode::ez;

  // disable the profile controller, as the operator will control it manually
  if (robot::chassisProfiler) robot::chassisProfiler->flipDisable(true);

  // in order to read rising/falling cases, button objects are needed
  okapi::ControllerButton buttonL1 = robot::controller[okapi::ControllerDigital::L1];
  okapi::ControllerButton buttonL2 = robot::controller[okapi::ControllerDigital::L2];
  okapi::ControllerButton buttonL3 = robot::controller[okapi::ControllerDigital::right];
  okapi::ControllerButton buttonDown = robot::controller[okapi::ControllerDigital::down];
  okapi::ControllerButton buttonB = robot::controller[okapi::ControllerDigital::B];

  // in order to control tray with a toggle, its state is stored here
  bool trayDown = true;

  while(true){

    robot::screen::notification = std::to_string(robot::imu->get_rotation());

    // If A is pressed, lock control to forwards/backwards at 40% speed, otherwise do standard tank control
    if(robot::controller.getDigital(okapi::ControllerDigital::A)){
      robot::chassis->getModel()->arcade(robot::controller.getAnalog(okapi::ControllerAnalog::leftY) * 0.4, 0);

    }else{
      robot::chassis->getModel()->tank(robot::controller.getAnalog(okapi::ControllerAnalog::leftY),
                                       robot::controller.getAnalog(okapi::ControllerAnalog::rightY)); 
    }

    /*
     * Intake control
     * R1 -> full power intake
     * R2 -> half power intake
     * R3/Y -> half power outtake
    **/
    if(robot::controller.getDigital(okapi::ControllerDigital::R1)){
      robot::intake->moveVoltage(12000);

    }else if(robot::controller.getDigital(okapi::ControllerDigital::R2)){
      robot::intake->moveVoltage(4000);

    }else if(robot::controller.getDigital(okapi::ControllerDigital::Y) || 
              (robot::controller.getAnalog(okapi::ControllerAnalog::leftY) < -0.05 && robot::controller.getDigital(okapi::ControllerDigital::A))){
      robot::intake->moveVoltage(-6000);
      
    }else{
      robot::intake->moveVoltage(0);
    }

    /*
     * Lift control
     * The lift will go up while the respective button is pressed and held
     * 
     * L1 -> middle height tower
     * L2 -> low/alliance height tower
    **/
    if(buttonL1.changed()){
      if(buttonL1.isPressed()){
        robot::lift->moveMidTower();
        robot::tilter->readyLift();
      }else{
        robot::lift->reset();
        robot::tilter->reset();
        trayDown = true;
      }
    }

    if(buttonL2.changed()){
      if(buttonL2.isPressed()){
        robot::lift->moveLowTower();
        robot::tilter->readyLift();
      }else{
        robot::lift->reset();
        robot::tilter->reset();
        trayDown = true;
      }
    }

    /*
     * Tilter control
     * While the lift is up, control is disabled
     * 
     * L3/right -> toggle tray position
    **/
    if(!buttonL1.isPressed() && !buttonL2.isPressed()){

      if(robot::controller.getDigital(okapi::ControllerDigital::up)){
        robot::tilter->prime();
      }

      if(buttonL3.changedToPressed()){
      
        trayDown = !trayDown;
        if(trayDown){
          robot::tilter->reset();
        }else{
          robot::tilter->stack();
        }

      }else if(buttonDown.changed()){
        if(buttonDown.isPressed()){
          robot::tilter->getTask()->suspend();
          robot::tilter->getMotor()->moveVoltage(-6000);

        }else{
          robot::tilter->getTask()->resume();
          robot::tilter->tare();
          robot::tilter->reset();
        }
      }
    }

    // overly complicated macro to avoid accidental triggering
    // generate paths and save to SD card
    if(robot::controller.getDigital(okapi::ControllerDigital::X) && 
       robot::controller.getDigital(okapi::ControllerDigital::B) &&
       robot::controller.getDigital(okapi::ControllerDigital::down) &&
       robot::controller.getDigital(okapi::ControllerDigital::left)){
      generatePaths();
    }

    // 100 Hz
    pros::delay(10);
  }
}
