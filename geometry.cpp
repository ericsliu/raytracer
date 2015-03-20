#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <limits>
#include <math.h>
#include <stdio.h>

#define EPSILON 1e-6

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

void Point::add(Vector v) {
  x+=v.x;
  y+=v.y;
  z+=v.z;
}

void Point::sub(Vector v) {
  x-=v.x;
  y-=v.y;
  z-=v.z;
}

Vector Point::sub(Point p) {
  return Vector(x-p.x, y-p.y, z-p.z);
}

/*
 * Class Vector
 */
Vector::Vector() {
  x = 1;
  y = 1;
  z = 1;
  this->normalize();
}

Vector::Vector(float xVal, float yVal, float zVal) {
  x = xVal;
  y = yVal;
  z = zVal;
}

Vector::Vector(Point point) {
  x = point.x;
  y = point.y;
  z = point.z;
}

void Vector::scale(float k) {
  x = x * k;
  y = y * k;
  z = z * k;
}

void Vector::normalize() {
  float l2norm = norm();
  l2norm = 1.0 / l2norm;
  scale(l2norm);
}

float Vector::norm() {
  return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

float Vector::dot(Vector vector) {
  return x * vector.x + y * vector.y + z * vector.z;
}

Vector Vector::cross(Vector v) {
  return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

void Vector::add(Vector vector) {
  x = x + vector.x;
  y = y + vector.y;
  z = z + vector.z;
}

void Vector::sub(Vector vector) {
  x = x - vector.x;
  y = y - vector.y;
  z = z - vector.z;
}

void Vector::add(Point point) {
  x = x + point.x;
  y = y + point.y;
  z = z + point.z;
}

void Vector::sub(Point point) {
  x = x - point.x;
  y = y - point.y;
  z = z - point.z;
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
  dir.normalize();
  tMin = EPSILON;
  tMax = std::numeric_limits<float>::infinity();
}

Ray::Ray(Point orig, Vector direction, float tMaxVal) {
  origin = orig;
  dir = direction;
  dir.normalize();
  tMin = EPSILON;
  tMax = tMaxVal;
}

Ray::Ray(Point orig, Vector direction, float tMinVal, float tMaxVal) {
  origin = orig;
  dir = direction;
  dir.normalize();
  tMin = tMinVal;
  tMax = tMaxVal;
}

Point Ray::at(float t) {
  if (t < tMin) t = tMin;
  else if (t > tMax) t = tMax;
  Point point = origin;
  Vector direction = dir;
  direction.scale(t);
  point.add(direction);
  return point;
}

LocalGeo::LocalGeo() {
  pos = Point();
  normal = Vector();
}

LocalGeo::LocalGeo(Point p, Vector n) {
  pos = p;
  normal = n;
  normal.normalize();
}

Point LocalGeo::getPosition() {
  return pos;
}

Vector LocalGeo::getNormal() {
  return normal;
}

void LocalGeo::setPosition(Point p) {
  pos = p;
}

void LocalGeo::setNormal(Vector n) {
  normal = n;
  normal.normalize();
}

Sphere::Sphere() {
  center = Point();
  radius = 1;
}

Sphere::Sphere(Point c, float r) {
  center = c;
  radius = r;
}

float Sphere::intersect(Ray& ray) {
  Vector temp = Vector(ray.origin);
  temp.sub(center);
  float a = ray.dir.dot(temp) * -1;
  float b = ray.dir.dot(temp) * 2;
  float c = temp.dot(temp) - pow(radius,2);
  float discriminant = pow(b / 2, 2) - c;
  printf("discrim = %f\n", discriminant);

  if (discriminant > 0) {
    printf("discrim > 0\n");
    //float t1 = (-b+sqrt(discriminant))/(a*2);
    //float t2 = (-b-sqrt(discriminant))/(a*2);
    float t1 = a - sqrt(discriminant);
    float t2 = a + sqrt(discriminant);
    printf("t1 = %f\n", t1);
    printf("t2 = %f\n", t2);
    if (t1 >= 0.0) {
      return t1;
    } else return t2;
  }
  else if (discriminant == 0) {
    if (a >= 0.0) {
      return a;
    } else return -1.0;
  } else return -1.0;
}

float Sphere::intersect(Ray& ray, LocalGeo* local) {
  float intersection = intersect(ray);
  if (intersection != -1.0) {
    Point point = ray.at(intersection);
    Vector normal = Vector(point);
    local->setPosition(point);
    normal.sub(center);
    normal.normalize();
    local->setNormal(normal);
  }
  return intersection;
}

Triangle::Triangle() {
  vertex = Point();
  edgeOne = Vector();
  edgeTwo = Vector();
}

Triangle::Triangle(Point point1, Point point2, Point point3) {
  vertex = point1;
  edgeOne = point2.sub(point1);
  edgeTwo = point3.sub(point1);
}

float Triangle::intersect(Ray& ray) {
  // Citation:
  // http://tinyurl.com/2zve8a
  Vector pVec = ray.dir.cross(edgeTwo);
  float det = pVec.dot(edgeOne);
  if (det > -EPSILON && det < EPSILON) return -1.0;
  Vector tVec = ray.origin;
  tVec.sub(vertex);
  float u, v;
  u = tVec.dot(pVec)/det;
  if (u>1.0 || u<0.0) return -1.0;
  Vector qVec = tVec.cross(edgeOne);
  v = ray.dir.dot(qVec)/det;
  if (v+u>1.0 || v<0.0) return -1.0;
  float t = edgeTwo.dot(qVec)/det;
  if (t<ray.tMin || t>ray.tMax) return -1.0;
  return t;
}

float Triangle::intersect(Ray& ray, LocalGeo* local) {
  float intersection = intersect(ray);
  if (intersection != -1.0) {
    local->setPosition(ray.at(intersection));
    Vector triNormal = Vector(edgeOne.cross(edgeTwo));
    triNormal.normalize();
    Vector geomNormal;
    if (ray.dir.dot(triNormal) > 0.0){
      geomNormal = Vector(edgeTwo.cross(edgeOne));
    } else {
      geomNormal = Vector(edgeOne.cross(edgeTwo));
    }
    geomNormal.normalize();
    local->setNormal(geomNormal);
  }
  return intersection;
}
