#include "main.h"
#include "robot.hpp"

void opcontrol() {
	auto buttonLeft = robot::master[okapi::ControllerDigital::left];
	auto buttonRight = robot::master[okapi::ControllerDigital::right];

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
				robot::intake->setStatus(robot::IntakeStatus::off);
			} else if (robot::master.getDigital(okapi::ControllerDigital::L2)){
				robot::angler->setStatus(robot::AnglerStatus::stacking);
				robot::intake->setStatus(robot::IntakeStatus::stacking);
			}

			if(robot::master.getDigital(okapi::ControllerDigital::A)){
				robot::intake->setStatus(robot::IntakeStatus::on);
			} else if(robot::master.getDigital(okapi::ControllerDigital::B)){
				robot::intake->setStatus(robot::IntakeStatus::off);
			}
		}

		pros::delay(10);
	}
}
