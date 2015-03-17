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
PointLight::PointLight() {
  vector = Vector(); color = Color(); type = "point";
}

PointLight::PointLight(float xVal, float yVal, float zVal, float red, float green, float blue) {
  vector = Vector(xVal,yVal,zVal); color = Color(red,green,blue); type = "point";
}

void PointLight::generateLightRay(LocalGeo& local, Ray& lray, Color& lcolor) {
  Vector direction = vector.sub(local.getPosition());
  tMax = direction.norm();
  direction.normalize();
  lray = Ray(local.getPosition(), direction, tMax);
  lcolor = color;
}

DirecLight::DirecLight() {
  vector = Vector(); color = Color(); type = "directional";
}

DirecLight::DirecLight(float xVal, float yVal, float zVal, float red, float green, float blue) {
  vector = Vector(xVal,yVal,zVal); color = Color(red,green,blue); type = "directional";
}

void DirecLight::generateLightRay(LocalGeo& local, Ray& lray, Color& lcolor) {
  Vector dir = direction;
  dir.scale(-dir.norm());
  lray = Ray(local.getPosition(), dir);
  lcolor = color;
}
