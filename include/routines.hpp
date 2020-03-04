#pragma once
#include "screen/api.hpp"
#include <functional>
#include <string>
#include <vector>

struct AutonomousRoutine {
public:
  const char *title;

  std::function<void(screen::ttField*)> print;
  std::function<void()> run;

  AutonomousRoutine(const char *ititle,
                    std::function<void(screen::ttField*)> iprint,
                    std::function<void()> irun):
                    
                    title(ititle), print(iprint), run(irun)
  {}
};

extern std::vector<AutonomousRoutine> routines;
extern uint16_t selection;
