#include "main.h"
#include "lewis/create.hpp"
#include "liblvgl/display/lv_display.h"

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  create.box("name", 10, 10, 100, 100);
  create.triangle("Name", 1, 1, 1, 1, 1, 1);
}