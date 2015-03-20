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
}

Light::Light(float xVal, float yVal, float zVal, float red, float green, float blue) {
}

PointLight::PointLight() {
  vector = Vector();
  color = Color();
  type = "point";
}

PointLight::PointLight(float xVal, float yVal, float zVal, float red, float green, float blue) {
  vector = Vector(xVal,yVal,zVal);
  color = Color(red,green,blue);
  type = "point";
}

void PointLight::generateLightRay(LocalGeo& local, Ray& lray, Color& lcolor) {
  Vector direction = vector;
  direction.sub(local.getPosition());
  float tMax = direction.norm();
  direction.normalize();
  lray = Ray(local.getPosition(), direction, tMax);
  lcolor = color;
}

DirecLight::DirecLight() {
  vector = Vector();
  color = Color();
  type = "directional";
}

DirecLight::DirecLight(float xVal, float yVal, float zVal, float red, float green, float blue) {
  vector = Vector(xVal,yVal,zVal);
  color = Color(red,green,blue);
  type = "directional";
}

void DirecLight::generateLightRay(LocalGeo& local, Ray& lray, Color& lcolor) {
  Vector dir = vector;
  dir.scale(-dir.norm());
  lray = Ray(local.getPosition(), dir);
  lcolor = color;
}

Object::Object() {
}

Object::Object(Shape* inShape) {
  shape = inShape;
  ambr = 0.7;
  ambg = 0.7;
  ambb = 0.7;
  difr = 0.7;
  difg = 0.7;
  difb = 0.7;
  sper = 0.7;
  speg = 0.7;
  speb = 0.7;
  refr = 0.7;
  refg = 0.7;
  refb = 0.7;
}

Object::Object(Shape* inShape, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float rr, float rg, float rb) {
  shape = inShape;
  ambient = Color(ar, ag, ab);
  diffuse = Color(dr, dg, db);
  specular = Color(sr, sg, sb);
  reflective = Color(rr, rg, rb);
}
