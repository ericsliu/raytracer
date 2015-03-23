#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <string>

class Point;
class Vector;
class Ray;
class LocalGeo;
class Shape;
class Sphere;
class Triangle;

/*
 * Class Point
 * Packages three floats together as a single 3-d point
 */
class Point {
  public:
    float x;
    float y;
    float z;
    Point();
    Point(float, float, float);
    void add(Vector);
    void sub(Vector);
    Vector sub(Point);
};

/*
 * Class Vector
 * three floats for 3-d direction
 * normalization function
 */
class Vector {
  public:
    float x;
    float y;
    float z;
    Vector();
    Vector(float, float, float);
    Vector(Point);
    void scale(float k);
    void normalize();
    float norm();
    float dot(Vector);
    Vector cross(Vector);
    void add(Vector);
    void sub(Vector);
    void add(Point);
    void sub(Point);
};

/*
 * Class Ray
 * position of origin
 * vector for direction
 * min and max travel time of ray
 */
class Ray {
  public:
    Point origin;
    Vector dir;
    float tMin;
    float tMax;
    Ray();
    Ray(Point, Vector);
    Ray(Point, Vector, float);
    Ray(Point, Vector, float, float);
    Point at(float);
};

/*
 * Class LocalGeo
 */
class LocalGeo {
  public:
    Point pos;
    Vector normal;
    LocalGeo();
    LocalGeo(Point, Vector);
    void setPosition(Point);
    void setNormal(Vector);
    Point getPosition();
    Vector getNormal();
};


/*
 * Class Shape
 */
class Shape {
  public:
    virtual float intersect(Ray&);
    virtual float intersect(Ray&, LocalGeo*);
};

/*
 * Class Sphere
 */
class Sphere : public Shape {
  public:
    Point center;
    float radius;
    Sphere();
    Sphere(Point, float);
    float intersect(Ray&);
    float intersect(Ray&, LocalGeo*);
    //bool intersect(Ray&, float*);
    //bool intersect(Ray&, float*, LocalGeo*);
};

/*
 * Class Triangle
 */
class Triangle : public Shape {
  public:
    Point vertex;
    Vector edgeOne;
    Vector edgeTwo;
    Triangle();
    Triangle(Point, Point, Point);
    float intersect(Ray&);
    float intersect(Ray&, LocalGeo*);
    //bool intersect(Ray&, float*);
    //bool intersect(Ray&, float*, LocalGeo*);
};

#endif