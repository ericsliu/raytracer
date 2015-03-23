#ifndef LIGHT_H
#define LIGHT_H
#include "geometry.h"
#include <string>
#include <vector>
#include "matrix.h"

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
    Color(Vector);
    void cutOff();
    void scale(float k);
    void add(Color color);
    void sub(Color color);
    void mul(Color color);
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

class AmbieLight: public Light {
  public:
    AmbieLight();
    AmbieLight(float,float,float,float,float,float);
    void generateLightRay(LocalGeo*, Ray&, Color&);
};

/*
 * Class Object - contains a shape and BRDF
 */
class Object {
  public:
    std::vector< Shape* > shape;
    Matrix transform;
    Color ambient;
    Color diffuse;
    Color specular;
    Color reflective;
    Object();
    Object(std::vector< Shape* >);
    Object(std::vector< Shape* >, Color, Color, Color, float, Color);
    Object(std::vector< Shape* >, float, float, float, float, float, float, float, float, float, float, float, float, float);
    Object(Shape*);
    Object(Shape*, Color, Color, Color, float, Color);
    Object(Shape*, float, float, float, float, float, float, float, float, float, float, float, float, float);
    float intersect(Ray&);
    float intersect(Ray&, LocalGeo*);
    void setTransform(Matrix&);
};

#endif
