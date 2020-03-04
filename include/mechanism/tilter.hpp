#pragma once
#include "main.h"
#include <atomic>

class Tilter {
public:
  Tilter(std::shared_ptr<okapi::AbstractMotor> imotor, okapi::IterativePosPIDController::Gains igains, std::unique_ptr<okapi::Filter> iderivativeFilter = std::make_unique<okapi::PassthroughFilter>());

  ~Tilter();

  static std::atomic_int restingPos;
  static std::atomic_int readyLiftPos;
  static std::atomic_int pidThreshold;
  static std::atomic_int primePos;
  static std::atomic_int verticalPos;
  

  void stack();

  void prime();

  void readyLift();

  void reset();

  bool isSettled();

  void waitUntilSettled();

  void trim(int trimDistance);

  void tare();

  std::shared_ptr<okapi::AbstractMotor> getMotor();

  void startThread();

  pros::Task *getTask();

  static void trampoline(void *tilter);

private:
  void loop();

  std::atomic_int target;

  pros::Task *thread{nullptr};
  std::unique_ptr<okapi::IterativePosPIDController> controller{nullptr};
  std::shared_ptr<okapi::AbstractMotor> motor{nullptr};
};
