#include "main.h"
#include "robot.hpp"

void opcontrol() {
	while(true){
		auto left = robot::master.getAnalog(okapi::ControllerAnalog::leftY);
		auto right = robot::master.getAnalog(okapi::ControllerAnalog::rightY);

		robot::leftDriveF->controllerSet(left);
		robot::leftDriveR->controllerSet(left);

		robot::rightDriveF->controllerSet(right);
		robot::rightDriveR->controllerSet(right);

		if(robot::master.getDigital(okapi::ControllerDigital::R1)){
			robot::lift->controllerSet(1);
		} else if (robot::master.getDigital(okapi::ControllerDigital::R2)){
			robot::lift->controllerSet(-1);
		} else {
			robot::lift->controllerSet(0);
		}

		pros::delay(10);
	}
}
