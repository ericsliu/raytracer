#ifndef LIGHT_H
#define LIGHT_H
#include "light.h"
#endif
#include <math.h>

/*
 * Class Color
 */
Color::Color() {
  r = 0; g = 0; b = 0;
}

Color::Color(float red, float green, float blue) {
  if (r>1){r=1;} if (g>1){g=1;} if (b>1){b=1;}
  r = red; g = green; b = blue;
}

void Color::cutOff() {
  if (r > 1.0) {
    r = 1.0;
  }
  if (g > 1.0) {
    g = 1.0;
  }
  if (b > 1.0) {
    b = 1.0;
  }
}

void Color::scale(float k) {
  r = r * k;
  g = g * k;
  b = b * k;
}

void Color::add(Color color) {
  r = r + color.r;
  b = b + color.b;
  g = g + color.g;
}

void Color::sub(Color color) {
  r = r - color.r;
  b = b - color.b;
  g = g - color.g;
}

/*
 * Class Light
 */
Light::Light() {
  vector = Vector(); color = Color(); type = "point";
}

Light::Light(float xVal, float yVal, float zVal, float red, float green, float blue, bool point) {
  vector = Vector(xVal,yVal,zVal); color = Color(red,green,blue);
  if (point) { type = "point"; } else { type = "directional"; }
}
