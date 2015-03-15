#include <string>

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
};

/*
 * Class Vector
 * three floats for 3-d direction
 * normalization function
 */
class Vector {
  public:
    Point dir;
    Vector();
    Vector(float, float, float);
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    float getX();
    float getY();
    float getZ();
    void scale(float k);
    void normalize();
    float dot(Vector vector);
    void add(Vector vector);
    void sub(Vector vector);
    void add(Point point);
    void sub(Point point);
    void getDist(Point a, Point b); // Coordinates of b - a
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
};
