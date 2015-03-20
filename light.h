#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <string>

class Color;
class Light;
class PointLight;
class DirecLight;
class Object;

/* 
 * Class Color
 * three float for each color channel
 */
class Color {
  public:
    float r;
    float g;
    float b;
    Color();
    Color(float,float,float);
    void cutOff();
    void scale(float k);
    void add(Color color);
    void sub(Color color);
};

/*
 * Class Light  - superclass for point and directional lights
 * vector for direction the light is pointing at
 */
class Light {
  public:
    Vector vector;
    Color color;
    std::string type;
    Light();
    Light(float,float,float,float,float,float);
    virtual void generateLightRay(LocalGeo*, Ray&, Color&) = 0;
};

class PointLight: public Light {
  public:
    PointLight();
    PointLight(float,float,float,float,float,float);
    void generateLightRay(LocalGeo*, Ray&, Color&);
};

class DirecLight: public Light {
  public:
    DirecLight();
    DirecLight(float,float,float,float,float,float);
    void generateLightRay(LocalGeo*, Ray&, Color&);
};

/*
 * Class Object - contains a shape and BRDF
 */
class Object {
  public:
    Shape* shape;
    Color ambient;
    Color diffuse;
    Color specular;
    Color reflective;
    Object();
    Object(Shape*);
    Object(Shape*, float, float, float, float, float, float, float, float, float, float, float, float);
};
