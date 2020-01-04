#pragma once
#include "main.h"

class Lift {
public:
  Lift(std::shared_ptr<okapi::AbstractMotor> imotor, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter = std::make_unique<okapi::PassthroughFilter>());

  ~Lift();

  static std::atomic_int restingPos;
  static std::atomic_int lowTowerPos;
  static std::atomic_int midTowerPos;
  

  void moveLowTower();

  void moveMidTower();

  void reset();

  bool isSettled();

  void waitUntilSettled();

  void trim(int trimDistance);

  void tare();

  std::shared_ptr<okapi::AbstractMotor> getMotor();

  void startThread();

  pros::Task *getTask();

  static void trampoline(void *angler);

private:
  void loop();

  pros::Task *thread{nullptr};
  std::unique_ptr<okapi::IterativePosPIDController> controller{nullptr};
  std::shared_ptr<okapi::AbstractMotor> motor{nullptr};
};
