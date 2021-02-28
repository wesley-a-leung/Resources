#pragma once
#include <bits/stdc++.h>
#include "Point3D.h"
#include "Sphere3D.h"
using namespace std;

// Functions for a 3D polyhedron

// Determines twice the vector area of a face of coplanar points
// Function Arguments:
//   face: the coplanar points of the face
// Return Value: twice the vector area of the face, points are in ccw order
//   when looking from above (where up is the direction of the return vector)
// Time Complexity: O(N)
// Memory Complexity: O(1)
pt3 vectorArea2(const vector<pt3> &face) {
  pt3 ret(0, 0, 0); for (int i = 0, n = face.size(); i < n; i++)
    ret += face[i] * face[(i + 1) % n];
  return ret;
}

// Returns the absolute area of a face of coplanar points
// Function Arguments:
//   face: the coplanar points of the face
// Return Value: the area of the face
// Time Complexity: O(N)
// Memory Complexity: O(1)
T faceArea(const vector<pt3> &face) { return abs(vectorArea2(face)) / T(2); }

// Flips some of the faces of a polyhedron such that they are oriented
//   consistently (they will either all be pointed outwards or inwards)
// Function Arguments:
//   faces: a reference to a vector of vector of points representing
//     the faces of the polyhedron to be reoriented
// Time Complexity: O(N) for N total points
// Memory Complexity: O(1)
void reorient(vector<vector<pt3>> &faces) {
  int n = faces.size(); vector<vector<pair<int, bool>>> G(n);
  map<pair<pt3, pt3>, int> seen; for (int v = 0; v < n; v++)
    for (int i = 0, m = faces[v].size(); i < m; i++) {
      pt3 a = faces[v][i], b = faces[v][(i + 1) % m];
      auto it = seen.find(make_pair(a, b)); bool f = true;
      if (it == seen.end()) { it = seen.find(make_pair(b, a)); f = false; }
      if (it != seen.end()) {
        int w = it->second; G[v].emplace_back(w, f); G[w].emplace_back(v, f);
      } else seen[make_pair(a, b)] = v;
    }
  vector<char> flip(n, -1); vector<int> q(n); int front = 0, back = 0;
  flip[q[back++] = 0] = 0; while (front < back) {
    int v = q[front++]; for (auto &&e : G[v]) if (flip[e.first] == -1)
      flip[q[back++] = e.first] = flip[v] ^ e.second;
  }
  for (int v = 0; v < n; v++) {
    assert(flip[v] != -1);
    if (flip[v]) reverse(faces[v].begin(), faces[v].end());
  }
}

// Returns 6 times the signed volume of a polyhedron
// Function Arguments:
//   faces: a vector of vector of points representing
//     the faces of the polyhedron with consistent orientation
// Return Value: 6 times the signed volume of the polygon, positive if
//   all vector areas point outwards, negative if inwards
// Time Complexity: O(N) for N total points
// Memory Complexity: O(1)
T getVolume6(const vector<vector<pt3>> &faces) {
  T vol6 = 0; for (auto &&face : faces) vol6 += (vectorArea2(face) | face[0]);
  return vol6;
}

// Determines if a point is inside a polyhedron or not
// Points on the polyhedron are considered to be inside
// Function Arguments:
//   faces: a vector of vector of points representing
//     the faces of the polyhedron with consistent orientation
//   p: the point to check
// Return Value: -1 if inside the polyhedron, 0 if on the face, 1 if outside
// Time Complexity: O(N) for N total points
// Memory Complexity: O(1)
int isInside(const vector<vector<pt3>> &faces, ref3 p) {
  T sum = 0, PI = acos(T(-1)); Sphere3D s(p, 1); for (auto &&face : faces) {
    pt3 a = face[0], b = face[1], c = face[2], n = (b - a) * (c - a);
    if (eq((n | p) - (n | a), 0)) return 0;
    sum += remainder(s.surfaceAreaOnSph(face), 4 * PI);
  }
  return bool(round(sum / (4 * PI))) ? 1 : -1;
}
