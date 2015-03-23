#ifndef LIGHT_H
#define LIGHT_H
#include "light.h"
#endif
#include <math.h>
#include <vector>
#include "matrix.h"

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

Color::Color(Vector vector) {
  Vector temp = vector;
  temp.normalize();
  r = temp.x;
  g = temp.y;
  b = temp.z;
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

void Color::mul(Color color) {
  r = r * color.r;
  b = b * color.b;
  g = g * color.g;
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

void PointLight::generateLightRay(LocalGeo* local, Ray& lray, Color& lcolor) {
  Vector direction = vector;
  direction.sub(local->getPosition());
  float tMax = direction.norm();
  direction.normalize();
  lray = Ray(local->getPosition(), direction, tMax);
  lcolor = color;
}

DirecLight::DirecLight() {
  vector = Vector();
  color = Color();
  type = "directional";
}

DirecLight::DirecLight(float xVal, float yVal, float zVal, float red, float green, float blue) {
  vector = Vector(xVal,yVal,zVal);
  vector.normalize();
  color = Color(red,green,blue);
  type = "directional";
}

void DirecLight::generateLightRay(LocalGeo* local, Ray& lray, Color& lcolor) {
  Vector dir = vector;
  dir.scale(-1);
  lray = Ray(local->getPosition(), dir);
  lcolor = color;
}

AmbieLight::AmbieLight() {
  vector = Vector();
  color = Color();
  type = "ambient";
}

AmbieLight::AmbieLight(float red, float green, float blue) {
  vector = Vector();
  color = Color(red,green,blue);
  type = "ambient";
}

void AmbieLight::generateLightRay(LocalGeo* local, Ray& lray, Color& lcolor) {
  lray = Ray(local->getPosition(), Vector(), 0.0, 0.0);
  lcolor = color;
}

Object::Object() {
}

Object::Object(std::vector< Shape* > inShape) {
  shape = inShape;
  transform = Matrix();
  ambient = Color(0.7, 0.7, 0.7);
  diffuse = Color(0.7, 0.7, 0.7);
  specular = Color(0.7, 0.7, 0.7);
  specularPow = 2;
  reflective = Color(0.7, 0.7, 0.7);
}

Object::Object(std::vector< Shape* > inShape, Color amb, Color dif, Color spec, float specPow, Color ref) {
  shape = inShape;
  transform = Matrix();
  ambient = amb;
  diffuse = dif;
  specular = spec;
  specularPow = specPow;
  reflective = ref;
}

Object::Object(std::vector< Shape* > inShape, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float specPow, float rr, float rg, float rb) {
  shape = inShape;
  transform = Matrix();
  ambient = Color(ar, ag, ab);
  diffuse = Color(dr, dg, db);
  specular = Color(sr, sg, sb);
  specularPow = specPow;
  reflective = Color(rr, rg, rb);
}

Object::Object(Shape* inShape) {
  shape.push_back(inShape);
  transform = Matrix();
  ambient = Color(0.7, 0.7, 0.7);
  diffuse = Color(0.7, 0.7, 0.7);
  specular = Color(0.7, 0.7, 0.7);
  specularPow = 2;
  reflective = Color(0.7, 0.7, 0.7);
}

Object::Object(Shape* inShape, Color amb, Color dif, Color spec, float specPow, Color ref) {
  shape.push_back(inShape);
  transform = Matrix();
  ambient = amb;
  diffuse = dif;
  specular = spec;
  specularPow = specPow;
  reflective = ref;
}

Object::Object(Shape* inShape, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float specPow, float rr, float rg, float rb) {
  shape.push_back(inShape);
  transform = Matrix();
  ambient = Color(ar, ag, ab);
  diffuse = Color(dr, dg, db);
  specular = Color(sr, sg, sb);
  specularPow = specPow;
  reflective = Color(rr, rg, rb);
}

float Object::intersect(Ray& ray) {
  float t = -1;
  Ray objFrameRay = transform.inv().mul(ray);
  float scalar = transform.inv().mul(ray.dir).norm();
  for(int i = 0; i < shape.size(); i++) {
    float temp = shape[i]->intersect(objFrameRay);
    if (temp != -1 and temp < t) t = temp; 
  }
  if (t != -1) {
    t *= scalar;
    return t;
  } else {
    return -1
  }
}

float Object::intersect(Ray& ray, LocalGeo* local) {
  float t = -1;
  unsigned int index = -1;
  Ray objFrameRay = transform.inv().mul(ray);
  float scalar = transform.inv().mul(ray.dir).norm();
  for(int i = 0; i < shape.size(); i++) {
    float temp = shape[i]->intersect(objFrameRay);
    if (temp != -1 and temp < t) {
      t = temp;
      index = i;
    }
  }
  if (index != -1) {
    t = shape[index]->intersect(ray, local);
    t *= scalar;
    local.setPosition(transform.mul(local.getPosition()));
    local.setNormal(transform.inv().transpose().mul(local.getNormal()));
    return t;
  } else {
    return -1
  }
}

void Object::setTransform(Matrix& t) {
  transform = t;
}
