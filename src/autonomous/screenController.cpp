#include "main.h"
#include "robot.hpp"
#include "screen/field.hpp"
#include "autonomous/routines.hpp"
#include <vector>
#include <string.h>
#include <iostream>

//extern const lv_img_t sans;
//extern const lv_img_t logo;

void screenControllerFN(void* param){
  std::cout << "screen controller started\n";

  screen::initializeStyles();

  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  lv_obj_t *window = lv_obj_create(scr, NULL);
  lv_obj_set_style(window, &screen::blankStyle);

  const int number_of_routines = autonomousRoutines.size();

  screenMode lastScreenState = screenMode::disabled;

  //object pointers, may or may not be valid at any given time, depending on if they are in use

  //notification
  lv_obj_t* notification_label;

  //selection
  lv_obj_t* main_list;
  screen::Field field(scr);
  field.hide(true);
  std::vector<lv_obj_t*> routine_buttons;
  std::vector<lv_obj_t*> routine_labels;
  lv_obj_t* confirm_button;
  bool selected;

  //diagnostic

  //sans
  lv_obj_t* sans_text;
  lv_obj_t* sans_text_label;
  const std::vector<std::pair<int, std::string>> script = {{2, "it's a beautiful day outside"},
                                                           {2, "birds are singing\nflowers are blooming"},
                                                           {2, "on days like these\nkids like you"},
                                                           {6, "ARE GONNA HAVE A BAD TIME"}};
  uint count, charIndex, stringIndex;
  bool reading;

  std::cout << "screen controller initialized - entering main control loop\n";

  while(true){
    switch(robot::screen::state){
      case screenMode::notification:
        if(lastScreenState != robot::screen::state){
          std::cout << "screen controller - initializing notification mode\n";
          lv_obj_clean(window);
          lv_obj_set_size(window, 480, 240);

          notification_label = lv_label_create(window, NULL);
          lv_label_set_long_mode(notification_label, LV_LABEL_LONG_BREAK);
          lv_label_set_align(notification_label, LV_LABEL_ALIGN_CENTER);
          lv_obj_set_size(notification_label, 480, 240);
          lv_obj_align(notification_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
          lv_obj_set_style(notification_label, &screen::redText);

          lastScreenState = robot::screen::state;
        }

        lv_label_set_text(notification_label, robot::screen::notification.c_str());

        break;
      case screenMode::selection:
        if(lastScreenState != robot::screen::state){
          std::cout << "screen controller - initializing selection mode\n";
          lv_obj_clean(window);
          lv_obj_set_size(window, 240, 240);

          selected = false;

          main_list = lv_obj_create(window, NULL);
          lv_obj_set_style(main_list, &screen::listStyle);
          lv_obj_set_pos(main_list, 0, 0);
          lv_obj_set_size(main_list, 240, 40 * number_of_routines);
          lv_obj_set_drag(main_list, true);

          routine_buttons.resize(number_of_routines);
          routine_labels.resize(number_of_routines);

          for(int i = 0; i < number_of_routines; i++){

            routine_buttons[i] = lv_btn_create(main_list, NULL);

            lv_obj_set_size(routine_buttons[i], 200, 40);

            lv_btn_set_style(routine_buttons[i], LV_BTN_STYLE_REL, &screen::listStyle);
            lv_btn_set_style(routine_buttons[i], LV_BTN_STYLE_PR, &screen::pressedButton);

            if(i == 0){
              //first term only
              lv_obj_align(routine_buttons[i], NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

            }else{
              lv_obj_align(routine_buttons[i], routine_buttons[i-1], LV_ALIGN_OUT_BOTTOM_MID , 0, 0);
            }


            routine_labels[i] = lv_label_create(routine_buttons[i], NULL);
            lv_obj_set_style(routine_labels[i], autonomousRoutines[i].textStyle);
            lv_label_set_text(routine_labels[i], autonomousRoutines[i].title);
            lv_obj_align(routine_labels[i], NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
          }

          confirm_button = lv_btn_create(main_list, NULL);
          lv_obj_set_style(confirm_button, &screen::greenBox);
          lv_obj_set_size(confirm_button, 40, 40);
          lv_obj_set_pos(confirm_button, 200, 40 * selection);

          field.hide(false);
          field.setX(240);
          autonomousRoutines[selection].print(field);

          lastScreenState = robot::screen::state;
        }

        selected = selected || lv_btn_get_state(confirm_button) == LV_BTN_STATE_PR;

        if(!selected){
          for(int i = 0; i < number_of_routines; i++){
            if(lv_btn_get_state(routine_buttons[i]) == LV_BTN_STATE_PR){
              selection = i;
              lv_obj_set_y(confirm_button, 40 * selection);

              field.clean();
              autonomousRoutines[selection].print(field);
            }
          }

          lv_obj_set_x(main_list, 0);
        }else{
          lv_obj_set_hidden(main_list, true);
          field.setX(120);
        }

        break;
      case screenMode::diagnostic:
        break;
      case screenMode::sans:
        /*if(lastScreenState != robot::screen::state){
          std::cout << "screen controller - initializing sans mode\n";
          lv_obj_clean(window);
          lv_obj_set_size(window, 480, 240);

          lv_obj_t* sans_img = lv_img_create(window, NULL);
          lv_obj_set_pos(sans_img, 80, 10);
          lv_img_set_src(sans_img, &sans);

          sans_text = lv_obj_create(window, NULL);
          lv_obj_set_style(sans_text, &sans_text_box);
          lv_obj_set_size(sans_text, 360, 60);
          lv_obj_set_pos(sans_text, 60, 170);

          sans_text_label = lv_label_create(sans_text, NULL);
          lv_label_set_style(sans_text_label, &screen::whiteText);
          lv_label_set_long_mode(sans_text_label, LV_LABEL_LONG_BREAK);
          lv_obj_set_size(sans_text_label, 300, 60);
          lv_obj_align(sans_text_label, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 6);

          count = 0;
          charIndex = 1;
          stringIndex = 0;
          reading = true;

          lastScreenState = robot::screen::state;
        }

        if(count >= (windowipt.at(stringIndex).second.length() > charIndex ? script.at(stringIndex).first : 20) && reading){
          if(windowipt.at(stringIndex).second.length() == charIndex){
            if(windowipt.size() - 1 == stringIndex){
              reading = false;
            }else{
              charIndex = 1;
              stringIndex++;
            }
          }else{
            charIndex++;
          }
          count = 0;
        }

        lv_label_set_text(sans_text_label, script.at(stringIndex).second.substr(0, charIndex).c_str());

        if(reading){
          count++;
        }*/

        break;
      case screenMode::logo:
        /*if(lastScreenState != robot::screen::state){
          std::cout << "screen controller - initializing logo mode\n";
          lv_obj_clean(window);
          lv_obj_set_size(window, 480, 240);

          lv_obj_t* logo_img = lv_img_create(window, NULL);
          lv_img_set_src(logo_img, &logo);

          lastScreenState = robot::screen::state;
        }*/

        break;
      case screenMode::disabled:
        if(lastScreenState != robot::screen::state){
          std::cout << "screen controller - disable mode\n";
          lv_obj_clean(window);
          lv_obj_set_size(window, 480, 240);

          lastScreenState = robot::screen::state;
        }

        break;
    }
    pros::delay(50);
  }
}
