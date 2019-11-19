#include "mechanism/towerBar.hpp"

TowerBar::TowerBar(std::shared_ptr<okapi::AbstractMotor> imotor, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter):
    motor(imotor), controller(std::make_unique<okapi::IterativePosPIDController>(igains, okapi::TimeUtilFactory::create(), std::move(iderivativeFilter))){
        
    tare();
}

TowerBar::~TowerBar(){
    thread->remove();
    delete thread;
}

void TowerBar::intake(){
    controller->setTarget(minimumPos);
}

void TowerBar::raise(){
    controller->setTarget(raisedPos);
}

void TowerBar::phaseReset(){
    if(motor->getPosition() >= threshold){
        raise();
    }else{
        lower();
    }
}

void TowerBar::lower(){
    controller->setTarget(restingPos);
}

bool TowerBar::isSettled(){
    return controller->isSettled();
}

void TowerBar::waitUntilSettled(){
  while(!isSettled()){
    pros::delay(50);
  }
}

void TowerBar::trim(int trimDistance){
    minimumPos += trimDistance;
    restingPos += trimDistance;
    threshold += trimDistance;
    raisedPos += trimDistance;
}

void TowerBar::tare(){
    motor->tarePosition();
}

std::shared_ptr<okapi::AbstractMotor> TowerBar::getMotor(){
    return motor;
}

void TowerBar::startThread(){
    thread = new pros::Task(TowerBar::trampoline, this, "Tower Bar");
}

pros::Task *TowerBar::getTask(){
    return thread;
}

void TowerBar::trampoline(void *towerBar){
    static_cast<TowerBar*>(towerBar)->loop();
}

void TowerBar::loop(){
    while(true){
        if(controller->getTarget() == minimumPos && isSettled()){
            raise();
        }

        motor->controllerSet(controller->step(motor->getPosition()));

        pros::delay(10);
    }
}