#pragma once
#include "main.h"
#include "robotUnderlying.hpp"

namespace robot {

enum class IntakeStatus {
  off, on, stacking
};

class Intake {
public:
  Intake(std::shared_ptr<okapi::AbstractMotor> imotor,
         int ionVoltage,
         double istackingDeltaPos);

  void setStatus(IntakeStatus istatus);

private:
  std::shared_ptr<okapi::AbstractMotor> motor;
  int onVoltage;
  double stackingDeltaPos;
};

}
