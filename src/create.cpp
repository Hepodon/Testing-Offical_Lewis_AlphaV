#include "lewis/create.hpp"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/lvgl.h"
#include "liblvgl/widgets/line/lv_line.h"

void Creator::box(int x, int y, int width, int height) {
  lv_obj_t *rect = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(rect, x, y);
  lv_obj_set_size(rect, width, height);
}

void Creator::triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  const lv_point_precise_t line1_points[2] = {{x1, y1}, {x2, y2}};
  const lv_point_precise_t line2_points[2] = {{x2, y2}, {x3, y3}};
  const lv_point_precise_t line3_points[2] = {{x3, y3}, {x1, y1}};

  lv_obj_t *line1 = lv_line_create(lv_screen_active());
  lv_line_set_points(line1, line1_points, 2);

  lv_obj_t *line2 = lv_line_create(lv_screen_active());
  lv_line_set_points(line2, line2_points, 2);

  lv_obj_t *line3 = lv_line_create(lv_screen_active());
  lv_line_set_points(line3, line3_points, 2);

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_line_width(&style, 2);
  lv_style_set_line_color(&style, lv_color_hex(0x00FF00)); // Green
  lv_style_set_line_rounded(&style, false);

  lv_obj_add_style(line1, &style, 0);
  lv_obj_add_style(line2, &style, 0);
  lv_obj_add_style(line3, &style, 0);
}

Creator create;
