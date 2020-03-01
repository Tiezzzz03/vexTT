#include "lift.hpp"

Lift::Lift(std::shared_ptr<okapi::AbstractMotor> imotor, std::shared_ptr<okapi::AbstractButton> ibutton, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter):
  motor(imotor), button(ibutton), controller(std::make_unique<okapi::IterativePosPIDController>(igains, okapi::TimeUtilFactory::createDefault(), std::move(iderivativeFilter))){

  controller->setOutputLimits(12000,-12000);

  tare();
}

Lift::~Lift(){
  thread->remove();
  delete thread;
}

void Lift::moveLowTower(){
  controller->setTarget(lowTowerPos);
}

void Lift::moveMidTower(){
  controller->setTarget(midTowerPos);
}

void Lift::reset(){
  controller->setTarget(restingPos);
}

bool Lift::isSettled(){
  return controller->isSettled() || (controller->getTarget() == restingPos && button->isPressed());
}

void Lift::waitUntilSettled(){
  while(!isSettled()){
    pros::delay(50);
  }
}

void Lift::trim(int trimDistance){
  restingPos  += trimDistance;
  lowTowerPos += trimDistance;
  midTowerPos += trimDistance;
}

void Lift::tare(){
  motor->tarePosition();
}

std::shared_ptr<okapi::AbstractMotor> Lift::getMotor(){
  return motor;
}

void Lift::startThread(){
  thread = new pros::Task(Lift::trampoline, this, "Angler");
}

pros::Task *Lift::getTask(){
  return thread;
}

void Lift::trampoline(void *lift){
  static_cast<Lift*>(lift)->loop();
}

void Lift::loop(){
  double currentPos;
  double power;

  while(true){
    if(button->isPressed()) motor->tarePosition();
    currentPos = motor->getPosition();

    if(controller->getTarget() >= currentPos){
      power = controller->step(currentPos);
    }else{
      power = 0;
    }

    motor->moveVoltage(power);

    pros::delay(10);
  }
}
