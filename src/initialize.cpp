#include "main.h"
#include "robot.hpp"

void initialize() {
	robot::leftDriveF = std::make_shared<okapi::Motor>(-10);
	robot::leftDriveR = std::make_shared<okapi::Motor>(5);
	robot::rightDriveF = std::make_shared<okapi::Motor>(20);
	robot::rightDriveR = std::make_shared<okapi::Motor>(-15);

	robot::lift = std::make_shared<okapi::Motor>(19);
	robot::angler = std::make_shared<okapi::Motor>(9);

	robot::lIntake = std::make_shared<okapi::Motor>(8);
  robot::rIntake = std::make_shared<okapi::Motor>(-18);
}

void disabled() {

}

void competition_initialize() {

}
