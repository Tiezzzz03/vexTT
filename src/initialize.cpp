#include "main.h"
#include "robot.hpp"

void initialize() {
	//init motors / sensors
	robot::chassis = std::make_shared<okapi::SkidSteerModel>(
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::leftDriveF,  robot::underlying::leftDriveR})),
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::rightDriveF, robot::underlying::rightDriveR})),
		200, 12000
	);

	robot::intake = std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({
			robot::underlying::lIntake,
			robot::underlying::rIntake
	}));

	robot::lift = std::make_shared<okapi::Motor>(19);
	robot::angler = std::make_shared<okapi::Motor>(9);

	//config motors / sensors
	robot::lift->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	robot::angler->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

	robot::lift->setGearing(okapi::AbstractMotor::gearset::red);
	robot::angler->setGearing(okapi::AbstractMotor::gearset::red);

	robot::chassis->resetSensors();
	robot::lift->tarePosition();
	robot::angler->tarePosition();
}

void disabled() {

}

void competition_initialize() {

}
