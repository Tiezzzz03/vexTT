#pragma once
#include "main.h"
#include "robotUnderlying.hpp"

namespace robot {

enum class AnglerStatus {
  initial, active, stacking
};

class Angler {
public:
  Angler(std::unique_ptr<okapi::AsyncPositionController<double, double>> icontroller,
         double iactivePos,
         double istackingPos);

  void reset();

  void setStatus(AnglerStatus istatus);

  bool isStacking();

  void trim(double delta);

private:
  void setTarget();

  std::unique_ptr<okapi::AsyncPositionController<double, double>> controller;
  double activePos;
  double stackingPos;
  AnglerStatus status;
};

}
