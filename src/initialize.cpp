#include "main.h"
#include "robot.hpp"
#include "robotUnderlying.hpp"

extern void screenControllerFN(void* param);

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

	robot::intake = std::make_shared<mechanism::Intake>(
		std::make_shared<okapi::MotorGroup>(okapi::MotorGroup({robot::underlying::rIntake, robot::underlying::lIntake})),
		12000,
		90
	);

	robot::angler = std::make_shared<mechanism::Angler>(
		std::make_unique<okapi::AsyncPosIntegratedController>(
			robot::underlying::angler, 100, okapi::TimeUtilFactory::create()
		),
		600,
		1100
	);

	robot::screen::controller = new pros::Task(screenControllerFN,
                                                           NULL,
                                          TASK_PRIORITY_DEFAULT,
                                       TASK_STACK_DEPTH_DEFAULT,
                                                       "Screen");
  robot::screen::state = screenMode::notification;
}

void disabled() {

}
