#include "main.h"
#include "robot.hpp"
#include "robotUnderlying.hpp"

void initialize() {
	//init motors / sensors

	robot::underlying::angler = std::make_shared<okapi::Motor>(0);

	//config motors / sensors
	robot::underlying::angler->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

	robot::underlying::angler->setGearing(okapi::AbstractMotor::gearset::red);

	robot::chassis->resetSensors();
	robot::underlying::angler->tarePosition();

	//make mechanism wrappers
	robot::chassis = std::make_shared<okapi::SkidSteerModel>(
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::leftDriveF,  robot::underlying::leftDriveR})),
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::rightDriveF, robot::underlying::rightDriveR})),
		200, 12000
	);
	
	robot::intake = std::make_shared<robot::Intake>(
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::rIntake, robot::underlying::lIntake})),
		12000,
		90
	);

	robot::angler = std::make_shared<robot::Angler>(
		std::make_unique<okapi::AsyncPosIntegratedController>(
			robot::underlying::angler, 100, okapi::TimeUtilFactory::create()
		),
		600,
		1100
	);
}

void disabled() {

}

void competition_initialize() {

}
