#include "mechanisms/intake.hpp"

namespace mechanism {

Intake::Intake(std::shared_ptr<okapi::AbstractMotor> imotor,
               int ionVoltage,
               double istackingDeltaPos):
               motor(imotor), onVoltage(ionVoltage),
               stackingDeltaPos(istackingDeltaPos)
{
  motor->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
}

void Intake::setStatus(IntakeStatus istatus){
  switch(istatus){
    case IntakeStatus::off:
      motor->moveVoltage(0);
      break;
    case IntakeStatus::on:
      motor->moveVoltage(onVoltage);
      break;
    case IntakeStatus::stacking:
      motor->moveRelative(-stackingDeltaPos, 30);
      break;
  }
}

}
