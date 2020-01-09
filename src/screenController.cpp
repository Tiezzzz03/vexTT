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
    lv_obj_set_size(panel[i], 480, 240);
  }


  // Disabled init
  lv_obj_set_style(panel[0], &lv_style_plain);


  // Notification init
  lv_obj_set_style(panel[1], &lv_style_plain);
  lv_cont_set_layout(panel[1], LV_LAYOUT_CENTER);
  lv_obj_set_pos(panel[1], 480, 0);

  lv_obj_t *notificationLabel = lv_label_create(panel[1], NULL);
  lv_label_set_align(notificationLabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style(notificationLabel, &screen::resources::redText);


  // Selection init
  lv_obj_set_style(panel[2], &lv_style_plain);
  lv_obj_set_pos(panel[2], 480, 0);

  lv_obj_t *mainList = lv_btnm_create(panel[2], NULL);
  const char *buttonMap[2 * numberOfRoutines];
  for(uint i = 0; i < numberOfRoutines; i++){
    buttonMap[2*i] = routines[i].title;
    buttonMap[2*i+1] = (i + 1 < numberOfRoutines) ? "\n" : "";
  }
  lv_btnm_set_map(mainList, buttonMap);
  lv_obj_set_size(mainList, 240, 240);
  screen::ttField field(panel[2]);
  field.setPos(240, 0);


  // EZ Screen init
  static lv_style_t rainbowStyle;
  lv_style_copy(&rainbowStyle, &lv_style_plain);
  lv_obj_set_style(panel[3], &rainbowStyle);
  lv_obj_set_pos(panel[3], 480, 0);

  lv_obj_t *gifContainer = lv_cont_create(panel[3], NULL);
  lv_obj_set_size(gifContainer, 240, 240);
  lv_obj_set_pos(gifContainer, 120, 0);
  Gif ezgif("/usd/EZ/EZlogo.gif", gifContainer);
  uint16_t colorSeed = 0;
  

  pros::delay(500);
  LOG_INFO(std::string("ScreenController: Initialized"));

  while(true){
    switch(robot::screen::state){
      case screenMode::disabled:
        break;
      case screenMode::notification:
        break;
      case screenMode::selection:
        break;
      case screenMode::ez:
        break;
      default:
        throw std::invalid_argument("robot::screen::state has been set to an undefined mode");
    }

    pros::delay(50);
  }
}
