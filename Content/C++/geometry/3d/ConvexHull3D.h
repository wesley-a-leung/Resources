#pragma once
#include <bits/stdc++.h>
#include "../../utils/Random.h"
#include "Point3D.h"
using namespace std;

// Computes the convex hull of a set of N points 3D points (convex set of
//   minimum points with the minimum volume)
// Function Arguments:
//   P: the vector of points
// Return Value: a vector of faces in the convex hull, with each face being a
//   vector of exactly 3 points, all facing outwards
// In practice, has a large constant
// Time Complexity: O(N log N) expected
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/CH3D/
//   https://open.kattis.com/problems/worminapple
//   https://open.kattis.com/problems/starsinacan
vector<vector<pt3>> convexHull3D(vector<pt3> P) {
  vector<array<int, 3>> hullInd; shuffle(P.begin(), P.end(), rng);
  int n = P.size(); for (int i = 1, num = 1; i < n; i++) {
    if (num == 1) {
      if (P[0] != P[i]) { swap(P[1], P[i]); num++; }
    } else if (num == 2) {
      if (!eq(norm((P[1] - P[0]) * (P[i] - P[0])), 0)) {
        swap(P[2], P[i]); num++;
      }
    } else if (num == 3) {
      if (above(P[0], P[1], P[2], P[i]) != 0) { swap(P[3], P[i]); num++; }
    }
  }
  vector<bool> active; vector<vector<int>> vis(n), rvis;
  vector<array<pair<int, int>, 3>> other; vector<int> label(n, -1);
  auto addFace = [&] (int a, int b, int c) {
    hullInd.push_back({a, b, c}); active.push_back(true);
    rvis.emplace_back(); other.emplace_back();
  };
  auto addEdge = [&] (int a, int b) {
    vis[b].push_back(a); rvis[a].push_back(b);
  };
  auto abv = [&] (int a, int b) {
    array<int, 3> f = hullInd[a];
    return above(P[f[0]], P[f[1]], P[f[2]], P[b]) > 0;
  };
  auto edge = [&] (int f, int s) {
    return make_pair(hullInd[f][s], hullInd[f][(s + 1) % 3]);
  };
  auto glue = [&] (int af, int as, int bf, int bs) {
    other[af][as] = make_pair(bf, bs); other[bf][bs] = make_pair(af, as);
  };
  addFace(0, 1, 2); addFace(0, 2, 1); if (abv(1, 3)) swap(P[1], P[2]);
  for (int i = 0; i < 3; i++) glue(0, i, 1, 2 - i);
  for (int i = 3; i < n; i++) addEdge(abv(1, i), i);
  for (int i = 3; i < n; i++) {
    vector<int> rem; for (auto &&t : vis[i])
      if (active[t]) { active[t] = false; rem.push_back(t); }
    if (rem.empty()) continue;
    int st = -1; for (auto &&r : rem) for (int j = 0; j < 3; j++) {
      int o = other[r][j].first; if (active[o]) {
        int a, b; tie(a, b) = edge(r, j); addFace(a, b, i); st = a;
        int cur = int(rvis.size()) - 1; label[a] = cur; vector<int> tmp;
        set_union(rvis[r].begin(), rvis[r].end(),
                  rvis[o].begin(), rvis[o].end(), back_inserter(tmp));
        for (auto &&x : tmp) if (abv(cur, x)) addEdge(cur, x);
        glue(cur, 0, other[r][j].first, other[r][j].second);
      }
    }
    for (int x = st, y; ; x = y) {
      int lx = label[x]; glue(lx, 1, label[y = hullInd[lx][1]], 2);
      if (y == st) break;
    }
  }
  vector<vector<pt3>> hull;
  for (int i = 0; i < int(hullInd.size()); i++) if (active[i])
    hull.push_back(vector<pt3>{P[hullInd[i][0]], P[hullInd[i][1]],
                               P[hullInd[i][2]]});
  return hull;
}
