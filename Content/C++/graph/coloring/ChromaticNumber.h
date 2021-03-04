#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum number of colors required to color a simple graph such
//   that no two adjacent vertices share the same color
// Vertices are 0-indexed
// Function Arguments:
//   V: the number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the simple graph (no self loops or parallel edges)
//     between vertices v and w
// Return Value: the chromatic number (minimum number of colors)
// In practice, has a small constant
// Time Complexity: O(V 2^V)
// Memory Complexity: O(2^V)
// Tested:
//   https://judge.yosupo.jp/problem/chromatic_number
int chromaticNumber(int V, const vector<pair<int, int>> &edges) {
  int ans = V, PV = 1 << V; vector<int> mask(V, 0), ind(PV), aux(PV);
  for (auto &&e : edges) {
    mask[e.first] |= 1 << e.second; mask[e.second] |= 1 << e.first;
  }
  for (int d : {7, 9, 21, 33, 87, 93, 97}) {
    long long mod = 1e9 + d; fill(ind.begin(), ind.end(), 0); ind[0] = 1;
    fill(aux.begin(), aux.end(), 1); for (int i = 0; i < PV; i++) {
      int v = __builtin_ctz(i);
      ind[i] = ind[i ^ (1 << v)] + ind[(i ^ (1 << v)) & ~mask[v]];
    }
    for (int k = 1; k < ans; k++) {
      long long chi = 0; for (int i = 0; i < PV; i++) {
        int j = i ^ (i >> 1); aux[j] = (long long) aux[j] * ind[j] % mod;
        chi += (i & 1) ? aux[j] : -aux[j];
      }
      if (chi % mod != 0) ans = k;
    }
  }
  return ans;
}
