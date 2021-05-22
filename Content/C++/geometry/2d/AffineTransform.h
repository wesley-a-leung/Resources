#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Affine Transformations in 2D
// Functions:
//   prependMatrix(m2, b2): sets m = m2 * m and b = m2 * b + b2
//   transform(t): applies the AffineTransformation t to this
//   scaleX(a): scales the x coordinates by a
//   scaleY(a): scales the y coordinates by a
//   scale(a): scales the x and y coordinates by a
//   translate(p): translates the point by p
//   rotate(theta): rotates the point theta radians around the origin
//   reflect(dir): reflects the point across the line passing through the
//     origin with direction dir
//   project(dir): projects the point onto the line passing through the
//     origin with direction dir
//   applyTransform(p): applies the transformation to the point p
//   inverse(): returns the inverse of this transformation,
//     determinant of m must nonzero
// Time Complexity:
//   constructor, prependMatrix, transform, scaleX, scaleY, scale, translate,
//     rotate, reflect, applyTransform, inverse: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/tree3
struct AffineTransform {
  array<array<T, 2>, 2> m; array<T, 2> b;
  AffineTransform() {
    for (int i = 0; i < 2; i++) {
      b[i] = T(0); for (int j = 0; j < 2; j++) m[i][j] = i == j ? T(1) : T(0);
    }
  }
  void prependMatrix(const array<array<T, 2>, 2> &m2, const array<T, 2> &b2) {
    array<array<T, 2>, 2> resm; array<T, 2> resb;
    for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) {
      resm[i][j] = T(0);
      for (int k = 0; k < 2; k++) resm[i][j] += m2[i][k] * m[k][j];
    }
    for (int i = 0; i < 2; i++) {
      resb[i] = b2[i]; for (int j = 0; j < 2; j++) resb[i] += m2[i][j] * b[j];
    }
    m = resm; b = resb;
  }
  void transform(const AffineTransform &t) { prependMatrix(t.m, t.b); }
  void scaleX(T a) {
    prependMatrix({array<T, 2>{a, T(0)}, {T(0), T(1)}}, {T(0), T(0)});
  }
  void scaleY(T a) {
    prependMatrix({array<T, 2>{T(1), T(0)}, {T(0), a}}, {T(0), T(0)});
  }
  void scale(T a) {
    prependMatrix({array<T, 2>{a, T(0)}, {T(0), a}}, {T(0), T(0)});
  }
  void translate(pt p) {
    prependMatrix({array<T, 2>{T(1), T(0)}, {T(0), T(1)}}, {p.x, p.y});
  }
  void rotate(T theta) {
    T c = cos(theta), s = sin(theta);
    prependMatrix({array<T, 2>{c, -s}, {s, c}}, {T(0), T(0)});
  }
  void reflect(pt dir) {
    T a = dir.x * dir.x - dir.y * dir.y, b = T(2) * dir.x * dir.y;
    T n = norm(dir); a /= n; b /= n;
    prependMatrix({array<T, 2>{a, b}, {b, -a}}, {T(0), T(0)});
  }
  void project(pt dir) {
    T a = dir.x * dir.x, b = dir.x * dir.y, c = dir.y * dir.y;
    T n = norm(dir); a /= n; b /= n; c /= n;
    prependMatrix({array<T, 2>{a, b}, {b, c}}, {T(0), T(0)});
  }
  pt applyTransform(pt p) {
    return pt(m[0][0] * p.x + m[0][1] * p.y + b[0],
              m[1][0] * p.x + m[1][1] * p.y + b[1]);
  }
  AffineTransform inverse() const {
    AffineTransform ret;
    ret.prependMatrix({array<T, 2>{T(1), T(0)}, {T(0), T(1)}}, {-b[0], -b[1]});
    T det = m[0][0] * m[1][1] - m[0][1] * m[1][0]; assert(!eq(det, 0));
    ret.prependMatrix({array<T, 2>{m[1][1] / det, -m[0][1] / det},
                                  {-m[1][0] / det, m[0][0] / det}},
                      {T(0), T(0)});
    return ret;
  }
};
