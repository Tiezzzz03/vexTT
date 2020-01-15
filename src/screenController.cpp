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
  lv_obj_set_style(panel[1], &screen::resources::redText);


  // Selection init
  lv_obj_set_style(panel[2], &lv_style_plain);
  lv_obj_set_pos(panel[2], 480, 0);

  lv_obj_t *selectionList = lv_btnm_create(panel[2], NULL);
  const char *buttonMap[2 * numberOfRoutines];
  for(uint i = 0; i < numberOfRoutines; i++){
    buttonMap[2*i] = routines[i].title;
    buttonMap[2*i+1] = (i + 1 < numberOfRoutines) ? "\n" : "";
  }
  lv_btnm_set_map(selectionList, buttonMap);
  lv_btnm_set_style(selectionList, LV_BTNM_STYLE_BG, &screen::resources::listStyle);
  lv_btnm_set_style(selectionList, LV_BTNM_STYLE_BTN_REL, &screen::resources::listStyle);
  lv_btnm_set_style(selectionList, LV_BTNM_STYLE_BTN_PR, &screen::resources::pressedButton);
  lv_btnm_set_style(selectionList, LV_BTNM_STYLE_BTN_TGL_REL, &screen::resources::pressedButton);
  lv_btnm_set_style(selectionList, LV_BTNM_STYLE_BTN_TGL_PR, &screen::resources::pressedButton);
  lv_btnm_set_toggle(selectionList, true, selection);
  lv_btnm_set_recolor(selectionList, true);
  lv_obj_set_size(selectionList, 240, 240);
  screen::ttField field(panel[2]);
  field.finishDrawing();
  field.setPos(240, 0);
  uint16_t toggledBtn;


  // EZ Screen init
  static lv_style_t rainbowStyle;
  lv_style_copy(&rainbowStyle, &lv_style_plain);
  lv_obj_set_style(panel[3], &rainbowStyle);
  lv_obj_set_pos(panel[3], 480, 0);

  lv_obj_t *gifContainer = lv_cont_create(panel[3], NULL);
  lv_obj_set_style(gifContainer, &lv_style_transp);
  lv_obj_set_size(gifContainer, 240, 240);
  lv_obj_set_pos(gifContainer, 120, 0);
  Gif ezgif("/usd/EZ/EZlogo.gif", gifContainer);
  uint16_t colorSeed = 0;
  

  pros::delay(500);
  LOG_INFO(std::string("ScreenController: Initialized"));

  while(true){
    if(robot::screen::state == lastScreenState){
      switch(robot::screen::state){
        case screenMode::disabled:
          break;

        case screenMode::notification:
          lv_label_set_text(notificationLabel, robot::screen::notification.c_str());
          break;

        case screenMode::selection:
          toggledBtn = lv_btnm_get_toggled(selectionList);

          if(selection != toggledBtn){
            field.clean();
            routines[toggledBtn].print(&field);
            selection = toggledBtn;
          }

          break;

        case screenMode::ez:
          rainbowStyle.body.main_color = getRainbowColorFromSeed(colorSeed);
          rainbowStyle.body.grad_color = getRainbowColorFromSeed(colorSeed - 0xFF);

          if(colorSeed < 0x5F8){
            colorSeed += 8;
          }else{
            colorSeed = 0;
          }

          lv_obj_refresh_style(panel[3]);

          break;

        default:
          throw std::invalid_argument("robot::screen::state has been set to an undefined mode");
      }
    }else{
      switch(lastScreenState){
        case screenMode::disabled:
          LOG_INFO(std::string("ScreenController: Cleaning up mode disabled"));
          lv_obj_set_pos(panel[0], 480, 0);
          break;

        case screenMode::notification:
          LOG_INFO(std::string("ScreenController: Cleaning up mode notification"));
          lv_obj_set_pos(panel[1], 480, 0);
          break;

        case screenMode::selection:
          LOG_INFO(std::string("ScreenController: Cleaning up mode selection"));
          lv_obj_set_pos(panel[2], 480, 0);

          break;
        case screenMode::ez:
          LOG_INFO(std::string("ScreenController: Cleaning up mode ez"));
          lv_obj_set_pos(panel[3], 480, 0);
          ezgif.pause();

          break;
        default:
          throw std::invalid_argument("robot::screen::state has been set to an undefined mode");
      }

      switch(robot::screen::state){
        case screenMode::disabled:
          LOG_INFO(std::string("ScreenController: Entering mode disabled"));
          lv_obj_set_pos(panel[0], 0, 0);
          break;

        case screenMode::notification:
          LOG_INFO(std::string("ScreenController: Entering mode notification"));
          lv_obj_set_pos(panel[1], 0, 0);
          break;

        case screenMode::selection:
          LOG_INFO(std::string("ScreenController: Entering mode selection"));
          lv_obj_set_pos(panel[2], 0, 0);
          break;

        case screenMode::ez:
          LOG_INFO(std::string("ScreenController: Entering mode ez"));
          lv_obj_set_pos(panel[3], 0, 0);
          ezgif.resume();
          break;

        default:
          throw std::invalid_argument("robot::screen::state has been set to an undefined mode");
      }
      lastScreenState = robot::screen::state;
    }

    pros::delay(50);
  }
}
