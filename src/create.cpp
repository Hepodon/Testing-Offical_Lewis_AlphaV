#include "lewis/create.hpp"
#include "liblvgl/core/lv_obj.h" // Ensure lv_scr_act is declared
#include "liblvgl/lvgl.h"

void Creator::box(int x, int y, int width, int height) {
  lv_obj_t *rect = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(rect, x, y);
  lv_obj_set_size(rect, width, height);
}

Creator create;
