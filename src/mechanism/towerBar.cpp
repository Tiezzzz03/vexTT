#include "mechanism/towerBar.hpp"

TowerBar::TowerBar(std::shared_ptr<okapi::AbstractMotor> imotor, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter):
    motor(imotor), controller(std::make_unique<okapi::IterativePosPIDController>(igains, okapi::TimeUtilFactory::create(), std::move(iderivativeFilter))){
        
    tare();
    controller->setOutputLimits(0.5,-0.5);
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

void TowerBar::lower(){
    controller->setTarget(restingPos);
}

bool TowerBar::isRaised(){
    return motor->getPosition() >= threshold;
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
    double power = 0;

    while(true){
        if(motor->getPosition() < minimumPos){
            raise();
        }

        if(controller->getTarget() == minimumPos){
            power = -1;
        }else{
            power = controller->step(motor->getPosition());
        }

        motor->controllerSet(power);
        pros::delay(10);
    }
}