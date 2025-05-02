#pragma once
#include <string>
#include <vector>

using namespace std;

struct Box {
  string name;
  int x, y, width, height;
};

struct Triangle {
  string name;
  int x1, y1, x2, y2, x3, y3;
};

class Creator {
public:
  void box(string name, int x, int y, int width, int height);
  void triangle(string name, int x1, int y1, int x2, int y2, int x3, int y3);

  vector<Box> boxes;
  vector<Triangle> triangles;

  vector<string> shape_Box_Names;
  vector<string> shape_Triangle_Names;
};

extern Creator create;
