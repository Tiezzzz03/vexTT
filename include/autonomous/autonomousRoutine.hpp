#ifndef AUTONOMOUSROUTINE_HPP_
#define AUTONOMOUSROUTINE_HPP_
#include "main.h"
#include "screen/field.hpp"
#include <functional>
#include <string>


struct autonomousRoutine{
public:
  const char* title;
  lv_style_t* textStyle;

  std::function<void()> run;
  std::function<void(screen::Field&)> print;

  autonomousRoutine(const char* ititle, lv_style_t* istyle,
                    std::function<void()> irun,
                    std::function<void(screen::Field&)> iprint):
                    title(ititle), textStyle(istyle),
                    run(irun), print(iprint)
  {}
};

#endif
