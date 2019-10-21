#pragma once
#include "screen/api.hpp"
#include <functional>
#include <string>
#include <vector>

struct AutonomousRoutine {
public:
  const char* title;
  lv_style_t* textStyle;

  std::function<void(screen::ttField*)> print;
  std::function<void()> init;
  std::function<void()> run;

  AutonomousRoutine(const char* ititle, lv_style_t* istyle,
                    std::function<void(screen::ttField*)> iprint,
                    std::function<void()> iinit,
                    std::function<void()> irun
                    ):
                    title(ititle), textStyle(istyle),
                    print(iprint), init(iinit), run(irun)
  {}
};

extern std::vector<AutonomousRoutine> routines;
extern int selection;
