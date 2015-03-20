#ifndef MATRIX_H
#define MATRIX_H
#include "matrix.h"
#endif
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

Matrix::Matrix(Point point) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (x == y) {
        if (x == 0) {
          array[y][x] = point.x;
        }
        else if (x == 1) {
          array[y][x] = point.y;
        }
        else if (x == 2) {
          array[y][x] = point.z;
        }
        else {
          array[y][x] = 1;
        }
      }
      else {
        array[y][x] = 0;
      }
    }
  }
}

Matrix::Matrix(Vector vector) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (x == y) {
        if (x == 0) {
          array[y][x] = vector.x;
        }
        else if (x == 1) {
          array[y][x] = vector.y;
        }
        else if (x == 2) {
          array[y][x] = vector.z;
        }
        else {
          array[y][x] = 1;
        }
      }
      else {
        array[y][x] = 0;
      }
    }
  }
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

Matrix Matrix::rotate(Ray ray) {
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
