#include "main.h"
#include "robot.hpp"
#include "robotUnderlying.hpp"

namespace robot {

okapi::Controller master;

std::shared_ptr<okapi::SkidSteerModel> chassis;
std::shared_ptr<mechanism::Intake> intake;
std::shared_ptr<mechanism::Angler> angler;

namespace underlying {

okapi::Motor leftDriveF(0);
okapi::Motor leftDriveR(0);
okapi::Motor rightDriveF(0);
okapi::Motor rightDriveR(0);
okapi::Motor lIntake(0);
okapi::Motor rIntake(0);

std::shared_ptr<okapi::Motor> angler;


//okapi::ADIEncoder leftEnc(1,2);
//okapi::ADIEncoder middleEnc(5,6);
//okapi::ADIEncoder rightEnc(7,8);

}
}
