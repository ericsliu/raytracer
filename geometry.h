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
    LocalGeo(Point, Normal);
    void setPosition(Point);
    void setNormal(Normal);
    Point getPosition();
    Normal getNormal();
};


/*
 * Class Shape
 */
class Shape {
  public:
    virtual bool intersect(Ray&) = 0;
    virtual bool intersect(Ray&, float*, LocalGeo*) = 0;
};

/*
 * Class Circle
 */
class Circle : public Shape {
  public:
    Point center;
    float radius;
    Circle();
    Circle(Point, float);
    bool intersect(Ray&);
    bool intersect(Ray&, float*, LocalGeo*);
    // bool intersect(Ray&, float*);
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
    bool intersect(Ray&);
    bool intersect(Ray&, float*, LocalGeo*);
    // bool intersect(Ray&, float*, float*, float*);
};
