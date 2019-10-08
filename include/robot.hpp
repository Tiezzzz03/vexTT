#pragma once
#include "main.h"

namespace robot {

extern FILE *logFile;

extern std::shared_ptr<okapi::Motor> angler;
extern std::shared_ptr<okapi::MotorGroup> intake;
extern std::shared_ptr<okapi::MotorGroup> lDrive;
extern std::shared_ptr<okapi::MotorGroup> rDrive;

extern std::shared_ptr<okapi::ADIEncoder> lEnc;
extern std::shared_ptr<okapi::ADIEncoder> rEnc;
extern std::shared_ptr<okapi::ADIEncoder> mEnc;

extern std::shared_ptr<okapi::OdomChassisController> chassis;

}
