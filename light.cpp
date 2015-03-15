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
Color operator*(const Color& u, float k) {
}

Color operator*(float k, const Color& u) {
  Color temp = Color(u.r * k, u.g * k, u.b * k);
  return temp;
}

Color operator*(const Color& u, const Color& v) {
  Color temp = Color(u.r * v.r, u.g * v.g, u.b * v.b);
  return temp;
}

Color operator+(const Color& u, const Color& v) {
  Color temp = Color(u.r + v.r, u.g + v.g, u.b + v.b);
  return temp;
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
  os << "[r,g,b] = [" << c.r << ", " << c.g << ", " << c.b << "] \n";
  return os;
}*/

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
/*
std::ostream& operator<<(std::ostream& os, const Light& l) {
  os << l.type << " light: " << l.vector << " , " << l.color;
  return os;
}*/
