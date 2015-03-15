#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <math.h>

/*
 * Class Point
 */
Point::Point() {
  x = 0;
  y = 0;
  z = 0;
}

Point::Point(float xVal, float yVal, float zVal) {
  x = xVal;
  y = yVal;
  z = zVal;
}

/*
 * Class Vector
 */
Vector::Vector() {
  dir.x = 1;
  dir.y = 1;
  dir.z = 1;
  this->normalize();
}

Vector::Vector(float xVal, float yVal, float zVal) {
  dir.x = xVal;
  dir.y = yVal;
  dir.z = zVal;
}

void Vector::setX(float x) {
  dir.x = x;
}

void Vector::setY(float y) {
  dir.y = y;
}

void Vector::setZ(float z) {
  dir.z = z;
}

float Vector::getX() {
  return dir.x;
}

float Vector::getY() {
  return dir.y;
}

float Vector::getZ() {
  return dir.z;
}

void Vector::scale(float k) {
  setX(getX() * k);
  setY(getY() * k);
  setZ(getZ() * k);
}

void Vector::normalize() {
  float l2norm = sqrt(pow(dir.x,2) + pow(dir.y,2) + pow(dir.z,2));
  l2norm = 1.0 / l2norm;
  scale(l2norm);
}

float Vector::dot(Vector vector) {
  return getX() * vector.getX() + getY() * vector.getY() + getZ() * vector.getZ();
}

void Vector::add(Vector vector) {
  setX(getX() + vector.getX());
  setY(getY() + vector.getY());
  setZ(getZ() + vector.getZ());
}

void Vector::sub(Vector vector) {
  setX(getX() - vector.getX());
  setY(getY() - vector.getY());
  setZ(getZ() - vector.getZ());
}

void Vector::add(Point point) {
  setX(getX() + point.x);
  setY(getY() + point.y);
  setZ(getZ() + point.z);
}

void Vector::getDist(Point a, Point b) {
  setX(b.x - a.x);
  setY(b.y - a.y);
  setZ(b.z - a.z);
}

/*
 * Class Ray
 */
Ray::Ray() {
  origin = Point();
  dir = Vector();
}

Ray::Ray(Point orig, Vector direction) {
  origin = orig;
  dir = direction;
}
