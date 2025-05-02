#pragma once

class Creator {
public:
  void box(int x, int y, int width, int height);
  void triangle(int x1, int y1, int x2, int y2, int x3, int y3);
};

extern Creator create;
