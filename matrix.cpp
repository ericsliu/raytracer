#include "matrix.h"
#include <math.h>
#include <stdio.h>

/*
 * Class Matrix
 */
Matrix::Matrix() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (x == y) {
        array[y][x] = 1;
      }
      else {
        array[y][x] = 0;
      }
    }
  }
}

Matrix::Matrix(Point point, bool translation) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (x == y) {
        if (x == 0) {
          if (!translation) array[y][x] = point.x;
          else array[y][x] = 1;
        }
        else if (x == 1) {
          if (!translation) array[y][x] = point.y;
          else array[y][x] = 1;
        }
        else if (x == 2) {
          if (!translation) array[y][x] = point.z;
          else array[y][x] = 1;
        }
        else {
          array[y][x] = 1;
        }
      }
      else if (translation and x == 3) {
        if (y == 0) array[y][x] = point.x;
        else if (y == 1) array[y][x] = point.y;
        else if (y == 2) array[y][x] = point.z;
      }
      else {
        array[y][x] = 0;
      }
    }
  }
}

Matrix::Matrix(Vector vector, bool translation) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (x == y) {
        if (x == 0) {
          if (!translation) array[y][x] = vector.x;
          else array[y][x] = 1;
        }
        else if (x == 1) {
          if (!translation) array[y][x] = vector.y;
          else array[y][x] = 1;
        }
        else if (x == 2) {
          if (!translation) array[y][x] = vector.z;
          else array[y][x] = 1;
        }
        else {
          array[y][x] = 1;
        }
      }
      else if (translation and x == 3) {
        if (y == 0) array[y][x] = vector.x;
        else if (y == 1) array[y][x] = vector.y;
        else if (y == 2) array[y][x] = vector.z;
      }
      else {
        array[y][x] = 0;
      }
    }
  }
}

Matrix::Matrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float t0, float t1, float t2) {
  array[0][0] = m00;
  array[0][1] = m01;
  array[0][2] = m02;
  array[1][0] = m10;
  array[1][1] = m11;
  array[1][2] = m12;
  array[2][0] = m20;
  array[2][1] = m21;
  array[2][2] = m22;
  array[3][0] = 0;
  array[3][1] = 0;
  array[3][2] = 0;
  array[3][3] = 1;
  array[0][3] = t0;
  array[1][3] = t1;
  array[2][3] = t2;
}

void Matrix::printMatrix() {
  for (int y = 0; y < 4; y++) {
    printf("[ ");
    for (int x = 0; x < 4; x++) {
      printf("%f ", array[y][x]);
    }
    printf("]\n");
  }
  printf("\n");
}

Matrix Matrix::add(Matrix b) {
  Matrix sum = Matrix();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      sum.array[x][y] = array[x][y] + b.array[x][y];
    }
  }
  return sum;
}

Matrix Matrix::scale(float c) {
  Matrix scaled = Matrix();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      scaled.array[x][y] = array[x][y] * c;
    }
  }
  return scaled;
}

Matrix Matrix::transpose() {
  Matrix transpose = Matrix();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      transpose.array[x][y] = array[y][x];
    }
  }
  return transpose;
}

Matrix Matrix::mul(Matrix b) {
  Matrix product = Matrix();
  for (int y = 0; y < 4; y++) { // row of matrix A to use
    for (int x = 0; x < 4; x++) { // column of matrix B to use
      float sum = 0.0;
      for (int i = 0; i < 4; i++) {
        sum += array[y][i] * b.array[i][x];
      }
      product.array[y][x] = sum;
    }
  }
  return product;
}

Vector Matrix::mul(Vector b) {
  float temp[3];
  for (int y = 0; y < 3; y++) { // row of matrix A to use
    temp[y] = array[y][0]*b.x + array[y][1]*b.y + array[y][2]*b.z;
  }
  return Vector(temp[0],temp[1],temp[2]);
}

Point Matrix::mul(Point b) {
  float temp[3];
  for (int y = 0; y < 3; y++) { // row of matrix A to use
    temp[y] = array[y][0]*b.x + array[y][1]*b.y + array[y][2]*b.z + array[y][3];
  }
  return Point(temp[0],temp[1],temp[2]);
}

Ray Matrix::mul(Ray b) {
  Point o = mul(b.origin);
  Vector d = mul(b.dir);
  return Ray(o,d);
}

Matrix Matrix::inv() {
  Matrix inverse = Matrix();
  // find the determinant
  float determinant = 
    (array[0][0] * array[1][1] * array[2][2] * array[3][3]) +
    (array[0][0] * array[1][2] * array[2][3] * array[3][1]) +
    (array[0][0] * array[1][3] * array[2][1] * array[3][2]) + 
    (array[0][1] * array[1][0] * array[2][3] * array[3][2]) +
    (array[0][1] * array[1][2] * array[2][0] * array[3][3]) +
    (array[0][1] * array[1][3] * array[2][2] * array[3][0]) +
    (array[0][2] * array[1][0] * array[2][1] * array[3][3]) +
    (array[0][2] * array[1][1] * array[2][3] * array[3][0]) +
    (array[0][2] * array[1][3] * array[2][0] * array[3][1]) +
    (array[0][3] * array[1][0] * array[2][2] * array[3][1]) +
    (array[0][3] * array[1][1] * array[2][0] * array[3][2]) +
    (array[0][3] * array[1][2] * array[2][1] * array[3][0]) -
    (array[0][0] * array[1][1] * array[2][3] * array[3][2]) -
    (array[0][0] * array[1][2] * array[2][1] * array[3][3]) -
    (array[0][0] * array[1][3] * array[2][2] * array[3][1]) -
    (array[0][1] * array[1][0] * array[2][2] * array[3][3]) -
    (array[0][1] * array[1][2] * array[2][3] * array[3][0]) -
    (array[0][1] * array[1][3] * array[2][0] * array[3][2]) -
    (array[0][2] * array[1][0] * array[2][3] * array[3][1]) -
    (array[0][2] * array[1][1] * array[2][0] * array[3][3]) -
    (array[0][2] * array[1][3] * array[2][1] * array[3][0]) -
    (array[0][3] * array[1][0] * array[2][1] * array[3][2]) -
    (array[0][3] * array[1][1] * array[2][2] * array[3][0]) -
    (array[0][3] * array[1][2] * array[2][0] * array[3][1]);
  // find the adjunct
  inverse.array[0][0] = (array[1][1] * array[2][2] * array[3][3]) + (array[1][2] * array[2][3] * array[3][1]) + (array[1][3] * array[2][1] * array[3][2]) - (array[1][1] * array[2][3] * array[3][2]) - (array[1][2] * array[2][1] * array[3][3]) - (array[1][3] * array[2][2] * array[3][1]);
  inverse.array[0][1] = (array[0][1] * array[2][3] * array[3][2]) + (array[0][2] * array[2][1] * array[3][3]) + (array[0][3] * array[2][2] * array[3][1]) - (array[0][1] * array[2][2] * array[3][3]) - (array[0][2] * array[2][3] * array[3][1]) - (array[0][3] * array[2][1] * array[3][2]);
  inverse.array[0][2] = (array[0][1] * array[1][2] * array[3][3]) + (array[0][2] * array[1][3] * array[3][1]) + (array[0][3] * array[1][1] * array[3][2]) - (array[0][1] * array[1][3] * array[3][2]) - (array[0][2] * array[1][1] * array[3][3]) - (array[0][3] * array[1][2] * array[3][1]);
  inverse.array[0][3] = (array[0][1] * array[1][3] * array[2][2]) + (array[0][2] * array[1][1] * array[2][3]) + (array[0][3] * array[1][2] * array[2][1]) - (array[0][1] * array[1][2] * array[2][3]) - (array[0][2] * array[1][3] * array[2][1]) - (array[0][3] * array[1][1] * array[2][2]);
  inverse.array[1][0] = (array[1][0] * array[2][3] * array[3][2]) + (array[1][2] * array[2][0] * array[3][3]) + (array[1][3] * array[2][2] * array[3][0]) - (array[1][0] * array[2][2] * array[3][3]) - (array[1][2] * array[2][3] * array[3][0]) - (array[1][3] * array[2][0] * array[3][2]);
  inverse.array[1][1] = (array[0][0] * array[2][2] * array[3][3]) + (array[0][2] * array[2][3] * array[3][0]) + (array[0][3] * array[2][0] * array[3][2]) - (array[0][0] * array[2][3] * array[3][2]) - (array[0][2] * array[2][0] * array[3][3]) - (array[0][3] * array[2][2] * array[3][0]);
  inverse.array[1][2] = (array[0][0] * array[1][3] * array[3][2]) + (array[0][2] * array[1][0] * array[3][3]) + (array[0][3] * array[1][2] * array[3][0]) - (array[0][0] * array[1][2] * array[3][3]) - (array[0][2] * array[1][3] * array[3][0]) - (array[0][3] * array[1][0] * array[3][2]);
  inverse.array[1][3] = (array[0][0] * array[1][2] * array[2][3]) + (array[0][2] * array[1][3] * array[2][0]) + (array[0][3] * array[1][0] * array[2][2]) - (array[0][0] * array[1][3] * array[2][2]) - (array[0][2] * array[1][0] * array[2][3]) - (array[0][3] * array[1][2] * array[2][0]);
  inverse.array[2][0] = (array[1][0] * array[2][1] * array[3][3]) + (array[1][1] * array[2][3] * array[3][0]) + (array[1][3] * array[2][0] * array[3][1]) - (array[1][0] * array[2][3] * array[3][1]) - (array[1][1] * array[2][0] * array[3][3]) - (array[1][3] * array[2][1] * array[3][0]);
  inverse.array[2][1] = (array[0][0] * array[2][3] * array[3][1]) + (array[0][1] * array[2][0] * array[3][3]) + (array[0][3] * array[2][1] * array[3][0]) - (array[0][0] * array[2][1] * array[3][3]) - (array[0][1] * array[2][3] * array[3][0]) - (array[0][3] * array[2][0] * array[3][1]);
  inverse.array[2][2] = (array[0][0] * array[1][1] * array[3][3]) + (array[0][1] * array[1][3] * array[3][0]) + (array[0][3] * array[1][0] * array[3][1]) - (array[0][0] * array[1][3] * array[3][1]) - (array[0][1] * array[1][0] * array[3][3]) - (array[0][3] * array[1][1] * array[3][0]);
  inverse.array[2][3] = (array[0][0] * array[1][3] * array[2][1]) + (array[0][1] * array[1][0] * array[2][3]) + (array[0][3] * array[1][1] * array[2][0]) - (array[0][0] * array[1][1] * array[2][3]) - (array[0][1] * array[1][3] * array[2][0]) - (array[0][3] * array[1][0] * array[2][1]);
  inverse.array[3][0] = (array[1][0] * array[2][2] * array[3][1]) + (array[1][1] * array[2][0] * array[3][2]) + (array[1][2] * array[2][1] * array[3][0]) - (array[1][0] * array[2][1] * array[3][2]) - (array[1][1] * array[2][2] * array[3][0]) - (array[1][2] * array[2][0] * array[3][1]);
  inverse.array[3][1] = (array[0][0] * array[2][1] * array[3][2]) + (array[0][1] * array[2][2] * array[3][0]) + (array[0][2] * array[2][0] * array[3][1]) - (array[0][0] * array[2][2] * array[3][1]) - (array[0][1] * array[2][0] * array[3][2]) - (array[0][2] * array[2][1] * array[3][0]);
  inverse.array[3][2] = (array[0][0] * array[1][2] * array[3][1]) + (array[0][1] * array[1][0] * array[3][2]) + (array[0][2] * array[1][1] * array[3][0]) - (array[0][0] * array[1][1] * array[3][2]) - (array[0][1] * array[1][2] * array[3][0]) - (array[0][2] * array[1][0] * array[3][1]);
  inverse.array[3][3] = (array[0][0] * array[1][1] * array[2][2]) + (array[0][1] * array[1][2] * array[2][0]) + (array[0][2] * array[1][0] * array[2][1]) - (array[0][0] * array[1][2] * array[2][1]) - (array[0][1] * array[1][0] * array[2][2]) - (array[0][2] * array[1][1] * array[2][0]);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      inverse.array[y][x] = inverse.array[y][x] / determinant;
    }
  }
  return inverse;
}


