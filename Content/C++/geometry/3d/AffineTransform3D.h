#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
using namespace std;

// Affine Transformations in 3D
// Functions:
//   prependMatrix(m2, b2): sets m = m2 * m and b = m2 * b + b2
//   transform(t): applies the AffineTransformation t to this
//   scale(p): scales the x, y, z coordinates by p.x, p.y, p.z respectively
//   translate(p): translates the point by p
//   rotate(axis, theta): rotates the point theta radians around the line from
//     the origin in the direction of axis
//   reflect(norm): reflects the point across the plane passing through the
//     origin with the normal vector norm
//   project(norm): projects the point across the plane passing through the
//     origin with the normal vector norm
//   applyTransform(p): applies the transformation to the point p
//   inverse(): returns the inverse of this transformation,
//     determinant of m must nonzero
// Time Complexity:
//   constructor, prependMatrix, transform, scale, translate, rotate, reflect,
//     project, applyTransform, inverse: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/problemsets/acmsguru/problem/99999/265
struct AffineTransform3D {
  static T determinant(const array<array<T, 3>, 3> &A) {
    T a = A[0][0], b = A[0][1], c = A[0][2];
    T d = A[1][0], e = A[1][1], f = A[1][2];
    T g = A[2][0], h = A[2][1], k = A[2][2];
    return a * (e * k - f * h) - b * (d * k - f * g) + c * (d * h - e * g);
  }
  static array<array<T, 3>, 3> inverse(const array<array<T, 3>, 3> &A) {
    T a = A[0][0], b = A[0][1], c = A[0][2];
    T d = A[1][0], e = A[1][1], f = A[1][2];
    T g = A[2][0], h = A[2][1], k = A[2][2];
    T det = determinant(A); array<array<T, 3>, 3> ret;
    ret[0][0] = (e * k - f * h) / det;
    ret[0][1] = -(b * k - c * h) / det;
    ret[0][2] = (b * f - c * e) / det;
    ret[1][0] = -(d * k - f * g) / det;
    ret[1][1] = (a * k - c * g) / det;
    ret[1][2] = -(a * f - c * d) / det;
    ret[2][0] = (d * h - e * g) / det;
    ret[2][1] = -(a * h - b * g) / det;
    ret[2][2] = (a * e - b * d) / det;
    return ret;
  }
  array<array<T, 3>, 3> m; array<T, 3> b;
  AffineTransform3D() {
    for (int i = 0; i < 3; i++) {
      b[i] = T(0); for (int j = 0; j < 3; j++) m[i][j] = i == j ? T(1) : T(0);
    }
  }
  void prependMatrix(const array<array<T, 3>, 3> &m2, const array<T, 3> &b2) {
    array<array<T, 3>, 3> resm; array<T, 3> resb;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
      resm[i][j] = T(0);
      for (int k = 0; k < 3; k++) resm[i][j] += m2[i][k] * m[k][j];
    }
    for (int i = 0; i < 3; i++) {
      resb[i] = b2[i]; for (int j = 0; j < 3; j++) resb[i] += m2[i][j] * b[j];
    }
    m = resm; b = resb;
  }
  void transform(const AffineTransform3D &t) { prependMatrix(t.m, t.b); }
  void scale(pt3 p) {
    prependMatrix({array<T, 3>{p.x, T(0), T(0)},
                              {T(0), p.y, T(0)},
                              {T(0), T(0), p.z}},
                  {T(0), T(0), T(0)});
  }
  void translate(pt3 p) {
    prependMatrix({array<T, 3>{T(1), T(0), T(0)},
                              {T(0), T(1), T(0)},
                              {T(0), T(0), T(1)}},
                  {p.x, p.y, p.z});
  }
  void rotate(pt3 axis, T theta) {
    axis = unit(axis);
    T x = axis.x, y = axis.y, z = axis.z, c = cos(theta), s = sin(theta);
    prependMatrix({array<T, 3>{x * x * (1 - c) + c,
                               x * y * (1 - c) - z * s,
                               x * z * (1 - c) + y * s},
                              {y * x * (1 - c) + z * s,
                               y * y * (1 - c) + c,
                               y * z * (1 - c) - x * s},
                              {z * x * (1 - c) - y * s,
                               z * y * (1 - c) + x * s,
                               z * z * (1 - c) + c}},
                  {T(0), T(0), T(0)});
  }
  void reflect(pt3 norm) {
    norm = unit(norm); T a = norm.x, b = norm.y, c = norm.z;
    prependMatrix({array<T, 3>{1 - 2 * a * a, -2 * a * b, -2 * a * c},
                              {-2 * b * a, 1 - 2 * b * b, -2 * b * c},
                              {-2 * c * a, -2 * c * b, 1 - 2 * c * c}},
                  {T(0), T(0), T(0)});
  }
  void project(pt3 norm) {
    norm = unit(norm); T a = norm.x, b = norm.y, c = norm.z;
    prependMatrix({array<T, 3>{b * b + c * c, -a * b, -a * c},
                              {-b * a, a * a + c * c, -b * c},
                              {-c * a, -c * b, a * a + b * b}},
                  {T(0), T(0), T(0)});
  }
  pt3 applyTransform(pt3 p) {
    return pt3(m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + b[0],
               m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + b[1],
               m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + b[2]);
  }
  AffineTransform3D inverse() const {
    AffineTransform3D ret;
    ret.prependMatrix({array<T, 3>{T(1), T(0), T(0)},
                                  {T(0), T(1), T(0)},
                                  {T(0), T(0), T(1)}},
                      {-b[0], -b[1], -b[2]});
    assert(!eq(determinant(m), 0));
    ret.prependMatrix(inverse(m), {T(0), T(0), T(0)}); return ret;
  }
};
