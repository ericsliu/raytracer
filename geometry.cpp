#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <math.h>

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
  float l2norm = norm();
  l2norm = 1.0 / l2norm;
  scale(l2norm);
}

float Vector::norm() {
  return sqrt(pow(dir.x,2) + pow(dir.y,2) + pow(dir.z,2));
}

float Vector::dot(Vector vector) {
  return getX() * vector.getX() + getY() * vector.getY() + getZ() * vector.getZ();
}

Vector cross(Vector v) {
  return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
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
  tMin = EPSILON;
  tMax = std::numeric_limits<float>::infinity();
}

Ray::Ray(Point orig, Vector direction, float tMaxVal) {
  origin = orig;
  dir = direction;
  tMin = EPSILON;
  tMax = tMaxVal;
}

Ray::Ray(Point orig, Vector direction, float tMinVal, float tMaxVal) {
  origin = orig;
  dir = direction;
  tMin = tMinVal;
  tMax = tMaxVal;
}

Point Ray::at(float t) {
  if (t < tMin) t = tMin;
  else if (t > tMax) t = tMax;
  Point point = origin;
  Vector direction = dir;
  return point.add(direction.scale(t));
}

LocalGeo::LocalGeo() {
  point = Point();
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

Circle::Circle() {
  center = Point();
  radius = 1;
}

Circle::Circle(Point c, float r) {
  center = c;
  radius = r;
}

bool Circle::intersect(Ray& ray, double* tHit=NULL) {
  double a = ray.dir.dot(ray.dir);
  Vector temp = ray.origin;
  temp.sub(center);
  double b = ray.dir.dot(temp) * 2;
  double c = temp.dot(temp) - pow(radius,2);
  double discriminant = pow(b,2) - 4*a*c;

  if (discriminant > 0) {
    double t1 = (-b+sqrt(discriminant))/(a*2);
    double t2 = (-b-sqrt(discriminant))/(a*2);
    // Force t1 >= t2
    if (t2 > t1) {
      double temp = t1;
      t1 = t2;
      t2 = temp;
    }
    if (t1 < ray.tMin || t2 > ray.tMax) return false;
    else if (t1 <= ray.tMax) {
      if (tHit) *tHit = t1;
      return true;
    }
    else if (t2 >= ray.tMin) {
      if (tHit) *tHit = t2;
      return true;
    } else return false;
  }
  else if (discriminant == 0) {
    double t = -b/(a*2);
    if (t >= ray.tMin && t <= ray.tMax) {
      if (tHit) *tHit = t;
      return true;
    } else return false;
  } else return false;
}

bool Circle::intersect(Ray& ray, double* tHit, LocalGeo* local) {
  if (intersect(ray,tHit)) {
    Point temp = ray.at(*tHit);
    local->setPosition(temp);
    temp.sub(center);
    temp.normalize()
    local->setNormal(temp);
    return true;
  } else {
    return false;
  }
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

bool Triangle::intersect(Ray& ray, double*tHit=NULL) {
  // Citation:
  // http://tinyurl.com/2zve8a
  Vector pVec = ray.dir.cross(edgeTwo);
  double det = pVec.dot(edgeOne);
  if (det > -EPSILON && det < EPSILON) return false;
  Vector tVec = ray.origin;
  tVec.sub(vertex);
  double u, v;
  u = tVec.dot(pVec)/det;
  if (u>1.0 || u<0.0) return false;
  Vector qVec = tVec.cross(edgeOne);
  v = ray.dir.dot(qVec)/det;
  if (v+u>1.0 || v<0.0) return false;
  double t = edgeTwo.dot(qVec)/det;
  if (t<ray.tMin || t>ray.tMax) return false;
  if (tHit) *tHit = t; // tHit is only changed when intersect is true
  return true;
}

bool Triangle::intersect(Ray& ray, float* thit, LocalGeo* local) {
  if (intersect(ray,tHit)) {
    local->setPosition(ray.at(*tHit));
    Vector triNormal = Vector(edgeOne.cross(edgeTwo));
    tirNormal.normalize();
    Vector geomNormal;
    if (ray.dir.dot(triNormal) > 0.0){
      geomNormal = Vector(edgeTwo.cross(edgeOne));
    } else {
      geomNormal = Vector(edgeOne.cross(edgeTwo));
    }
    geomNormal.normalize();
    local->setNormal(geomNormal);
    return true;
  } else {
    return false;
  }
}