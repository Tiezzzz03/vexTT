#include "main.h"
#include "robot.hpp"

void opcontrol() {
	auto buttonLeft = robot::master[okapi::ControllerDigital::left];
	auto buttonRight = robot::master[okapi::ControllerDigital::right];

	robot::IntakeStatus intakeStatus = robot::IntakeStatus::off;

	while(true){
		robot::chassis->tank(
			robot::master.getAnalog(okapi::ControllerAnalog::leftY),
			robot::master.getAnalog(okapi::ControllerAnalog::rightY));

		if(robot::master.getDigital(okapi::ControllerDigital::X)){
			if(buttonLeft.changedToPressed()){
				robot::angler->trim(-5);
			}else if(buttonRight.changedToPressed()){
				robot::angler->trim(5);
			}else if(robot::master.getDigital(okapi::ControllerDigital::L1) && robot::master.getDigital(okapi::ControllerDigital::L2)){
				robot::angler->reset();
			}
		}else{
			if(robot::master.getDigital(okapi::ControllerDigital::R1)){
				robot::lift->controllerSet(1);
			} else if (robot::master.getDigital(okapi::ControllerDigital::R2)){
				robot::lift->controllerSet(-1);
			} else {
				robot::lift->controllerSet(0);
			}

			if(robot::master.getDigital(okapi::ControllerDigital::L1)){
				robot::angler->setStatus(robot::AnglerStatus::active);
				intakeStatus = robot::IntakeStatus::off;
			} else if (robot::master.getDigital(okapi::ControllerDigital::L2)){
				robot::angler->setStatus(robot::AnglerStatus::stacking);
				intakeStatus = robot::IntakeStatus::stacking;
			}

			if(robot::master.getDigital(okapi::ControllerDigital::A)){
				intakeStatus = robot::IntakeStatus::on;
			} else if(robot::master.getDigital(okapi::ControllerDigital::B)){
				intakeStatus = robot::IntakeStatus::off;
			}
		}

		switch(intakeStatus){
			case robot::IntakeStatus::on:
				robot::intake->moveVoltage(12000);
				break;
			case robot::IntakeStatus::off:
				robot::intake->moveVoltage(0);
				break;
			case robot::IntakeStatus::stacking:
				robot::intake->moveVoltage(0);
				break;
		}
		
		pros::delay(10);
	}
}
