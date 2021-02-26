#pragma once
#include <bits/stdc++.h>
#include "../utils/EpsCmp.h"
#include "Point3D.h"
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
// Time Complexity: O(F) for F faces
// Memory Complexity: O(1)
void reorient(vector<vector<pt3>> &faces) {
  int n = faces.size(); vector<vector<pair<int, bool>>> G(n);
  map<pair<pt3, pt3>, int> edges; for (int v = 0; v < n; v++)
    for (int i = 0, m = faces[v].size(); i < m; i++) {
      pt3 a = faces[v][i], b = faces[v][(i + 1) % m];
      auto it = edges.find(make_pair(a, b)); bool s = true;
      if (it == edges.end()) { it = edges.find(make_pair(b, a)); s = false; }
      if (it != edges.end()) {
        int w = it->second; G[v].emplace_back(w, s); G[w].emplace_back(v, s);
      } else edges[make_pair(a, b)] = v;
    }
  vector<bool> vis(n, false), flip(n, false); vector<int> q(n);
  int front = 0, back = 0; q[back++] = 0; while (front < back) {
    int v = q[front++]; for (auto &&e : G[v]) if (!vis[e.first]) {
      vis[q[back++] = e.first] = true; flip[e.first] = flip[v] ^ e.second;
    }
  }
  for (int v = 0; v < n; v++) if (flip[v])
    reverse(faces[v].begin(), faces[v].end());
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
