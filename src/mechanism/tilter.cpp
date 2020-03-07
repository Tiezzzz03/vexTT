#include "tilter.hpp"

using namespace okapi::literals;

Tilter::Tilter(std::shared_ptr<okapi::AbstractMotor> imotor, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter):
  motor(imotor), controller(std::make_unique<okapi::IterativePosPIDController>(igains, okapi::TimeUtilFactory::withSettledUtilParams(100, 1000, 0_ms), std::move(iderivativeFilter))){

  target.store(0);
  tare();
  controller->setOutputLimits(0.5, -1);
}

Tilter::~Tilter(){
  thread->remove();
  delete thread;
}

void Tilter::stack(){
  target.store(verticalPos);
  controller->setTarget(target);
}

void Tilter::prime(){
  target.store(primePos);
  controller->setTarget(target);
}

void Tilter::readyLift(){
  target.store(readyLiftPos);
  controller->setTarget(target);
}

void Tilter::reset(){
  target.store(restingPos);
  controller->setTarget(target);
}

bool Tilter::isSettled(){
  auto currentPos = motor->getPosition();

  if(target <= restingPos){
    return currentPos < restingPos;

  }else{
    return currentPos > pidThreshold && controller->isSettled();
  }
}

void Tilter::waitUntilSettled(){
  while(!isSettled()){
    pros::delay(50);
  }
}

void Tilter::trim(int trimDistance){
  restingPos   += trimDistance;
  pidThreshold += trimDistance;
  verticalPos  += trimDistance;
}

void Tilter::tare(){
  motor->tarePosition();
}

std::shared_ptr<okapi::AbstractMotor> Tilter::getMotor(){
  return motor;
}

void Tilter::startThread(){
  thread = new pros::Task(Tilter::trampoline, this, "Tilter");
}

pros::Task *Tilter::getTask(){
  return thread;
}

void Tilter::trampoline(void *tilter){
  static_cast<Tilter*>(tilter)->loop();
}

void Tilter::loop(){
  int currentPos = 0;
  double power = 0;

  while(true){
    currentPos = motor->getPosition();

    if(target >= pidThreshold && currentPos <= pidThreshold){
      power = 1;
    }else{
      power = controller->step(currentPos);
    }

    motor->controllerSet(power);

    pros::delay(10);
  }
}
