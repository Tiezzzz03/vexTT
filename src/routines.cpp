#include "routines.hpp"

int selection = 0;

std::vector<AutonomousRoutine> routines = {

AutonomousRoutine(
  "Disable",
  &lv_style_plain,
  [](screen::ttField *field) { field->finishDrawing(); },
  [](){},
  [](){}
),

AutonomousRoutine(
  "Small Zone Red",
  &screen::resources::redAlliance,
  [](screen::ttField *field) {
    field->draw(screen::cubeGroup::left4, 0);
    field->draw(screen::scoringZone::nearRed, screen::color::orange, 5);
    field->finishDrawing();
  },
  [](){},
  [](){}
)

};
