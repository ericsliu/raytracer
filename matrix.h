#ifndef LIGHT_H
#define LIGHT_H
#include "light.h"
#endif
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#include <string>

class Matrix;

/*
 * Class Matrix
 * converts Points, Vectors, into 4d matrices for transformation
 */
class Matrix {
  public:
    float array[4][4];
    Matrix();
    Matrix(Point);
    Matrix(Vector);
    Matrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float t0=0, float t1=0, float t2=0)
    void printMatrix();
    Matrix mul(Matrix); //equivalent to AB, where A is this matrix
    Vector mul(Vector); //apply transformations to a vector
    Point mul(Point); //apply transformations to a point
    Ray mul(Ray); //apply transformations to a ray
    Matrix inv();
};
