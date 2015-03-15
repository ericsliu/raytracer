#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <string>

/* 
 * Class Color
 * three float for each color channel
 */
class Color {
  public:
    float r;
    float g;
    float b;
    Color();
    Color(float,float,float);
    void cutOff();
    void scale(float k);
    void add(Color color);
    void sub(Color color);
};

/*
 * Class Light  - superclass for point and directional lights
 * vector for direction the light is pointing at
 */
class Light {
  public:
    Vector vector;
    Color color;
    std::string type;
    Light();
    Light(float,float,float,float,float,float, bool);
};
