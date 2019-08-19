#include "main.h"
#include "robot.hpp"
#include "robotUnderlying.hpp"

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
	robot::underlying::angler = std::make_shared<okapi::Motor>(9);

	//config motors / sensors
	robot::intake->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	robot::lift->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	robot::underlying::angler->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

	robot::lift->setGearing(okapi::AbstractMotor::gearset::red);
	robot::underlying::angler->setGearing(okapi::AbstractMotor::gearset::red);

	robot::chassis->resetSensors();
	robot::lift->tarePosition();
	robot::underlying::angler->tarePosition();

	//make mechanism wrappers
	robot::angler = std::make_shared<robot::Angler>(
		std::make_unique<okapi::AsyncPosIntegratedController>(
			robot::underlying::angler, okapi::TimeUtilFactory::create()
		),
		600,
		1100
	);
}

void disabled() {

}

void competition_initialize() {

}
