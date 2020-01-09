#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "robot.hpp"
#include "routines.hpp"
#include <vector>
#include <string.h>
#include <iostream>

//extern const lv_img_t logo;

lv_color_t getRainbowColorFromSeed(uint16_t colorSeed){
  return LV_COLOR_MAKE(

  static_cast<uint8_t>(
    colorSeed <= 0x0FF ? 0xFF :
    colorSeed <= 0x1FF ? 0x1FF - colorSeed :
    colorSeed <= 0x3FF ? 0 :
    colorSeed <= 0x4FF ? colorSeed - 0x400 :
    0xFF
  ),

  static_cast<uint8_t>(
    colorSeed <= 0x0FF ? colorSeed :
    colorSeed <= 0x2FF ? 0xFF :
    colorSeed <= 0x3FF ? 0x3FF - colorSeed :
    0
  ),

  static_cast<uint8_t>(
    colorSeed <= 0x1FF ? 0 :
    colorSeed <= 0x2FF ? colorSeed - 0x200 :
    colorSeed <= 0x4FF ? 0xFF :
    0x5FF - colorSeed
  ));
}

void screenControllerFN(void* param){
  auto logger = okapi::Logger::getDefaultLogger();

  screen::resources::initialize();

  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  const int numberOfRoutines = routines.size();
  const int numberOfPanels   = static_cast<int>(screenMode::NUMPANELS);

  screenMode lastScreenState = screenMode::disabled;

  // Screen Panels
  lv_obj_t *panel[numberOfPanels];
  for(uint i = 0; i < numberOfPanels; i++){
    panel[i] = lv_cont_create(scr, NULL);
  }

  
}
