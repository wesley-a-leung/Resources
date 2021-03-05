#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Circle.h"
#include "DelaunayTriangulation.h"
using namespace std;

// Computes the Voronoi Diagram and Delaunay Triangulation of a set of
//   distinct points
// If all points are collinear points, there is triangulation or diagram
// If there are 4 or more points on the same circle, the triangulation is
//   ambiguous, otherwise it is unique
// Each circumcircle does not completely contain any of the input points
// The Voronoi Diagram is represented as a graph with the first tri.size()
//   vertices being the circumcenters of each triangle in the triangulation
//   and additional vertices being added to represent infinty points in a
//   certain direction
// An edge is added between (v, w) if either triangles
//   v and w share an edge, or if v is a triangle that has an edge that is
//   not shared with any other triangle and rayDir[w - tri.size()] is the
//   direction of the ray from the circumcenter of v passing perpendicular
//   to the edge of the triangle of v that is not shared
// Constructor Arguments:
//   P: the distinct points
// Fields:
//   tri: a vector of arrays of 3 points, representing a triangle in
//     the triangulation, points given in ccw order
//   circumcircles: the circumcircles of each triangle
//   rayDir: for edges in the Voronoi Diagram that are infinity, this vector
//     contains the direction vector of those edges
//   G: the adjacency list of each vertex in the Voronoi Diagram
// In practice, has a moderate constant
// Time Complexity:
//   construction: O(N log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://icpc.kattis.com/problems/pandapreserve
struct VoronoiDiagram {
  struct PairPtCmp {
    bool operator () (const pair<pt, pt> &a, const pair<pt, pt> &b) const {
      return pt_eq()(a.first, b.first) ? pt_lt()(a.second, b.second)
                                       : pt_lt()(a.first, b.first);
    }
  };
  vector<array<pt, 3>> tri; vector<Circle> circumcircles; vector<pt> rayDir;
  vector<vector<int>> G;
  VoronoiDiagram(const vector<pt> &P)
      : tri(DelaunayTriangulation(P).tri), G(tri.size()) {
    map<pair<pt, pt>, int, PairPtCmp> seen; circumcircles.reserve(tri.size());
    for (auto &&t : tri)
      circumcircles.push_back(circumcircle(t[0], t[1], t[2]));
    for (int h = 0; h < 2; h++) for (int i = 0; i < int(tri.size()); i++)
      for (int k = 0; k < 3; k++) {
        pt p1 = tri[i][k], p2 = tri[i][(k + 1) % 3];
        auto it = seen.find(make_pair(p1, p2)); if (h == 0) {
          if (it == seen.end()) it = seen.find(make_pair(p2, p1));
          if (it == seen.end()) seen[make_pair(p1, p2)] = i;
          else {
            int j = it->second; G[i].push_back(j); G[j].push_back(i);
            seen.erase(it);
          }
        } else if (it != seen.end()) {
          int j = G.size(); G.emplace_back(); rayDir.push_back(perp(p1 - p2));
          G[i].push_back(j); G[j].push_back(i);
        }
      }
  }
};
