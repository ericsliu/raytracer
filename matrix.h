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
    void printMatrix();
    Matrix mul(Matrix); //equivalent to AB, where A is this matrix
    Matrix inv();
};
