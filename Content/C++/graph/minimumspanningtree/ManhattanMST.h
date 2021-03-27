#pragma once
#include <bits/stdc++.h>
#include "KruskalMST.h"
using namespace std;

// Computes the minimum spanning tree of a complete graph of N points where
//   the edge weights is equal to the Manhattan distance between the
//   points |x_i - x_j| + |y_i - y_j|
// Generates up to 4N candidate edges with each point connected to its nearest
//   neighbour in each octant
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the coordinates of the points
// Constructor Arguments:
//   P: a vector of pairs of type T representing the points
// Fields:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of tuples of the edges in the mst with the vertices
//     corresponding to the original indices of P
// In practice, has a moderate constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/manhattanmst
template <class T> struct ManhattanMST : public KruskalMST<T> {
  using Edge = typename KruskalMST<T>::Edge;
  static vector<Edge> generateCandidates(vector<pair<T, T>> P) {
    vector<int> id(P.size()); iota(id.begin(), id.end(), 0); vector<Edge> ret;
    ret.reserve(P.size() * 4); for (int h = 0; h < 4; h++) {
      sort(id.begin(), id.end(), [&] (int i, int j) {
        return P[i].first - P[j].first < P[j].second - P[i].second;
      });
      map<T, int> M; for (int i : id) {
        auto it = M.lower_bound(-P[i].second);
        for (; it != M.end(); it = M.erase(it)) {
          int j = it->second;
          T dx = P[i].first - P[j].first, dy = P[i].second - P[j].second;
          if (dy > dx) break;
          ret.emplace_back(i, j, dx + dy);
        }
        M[-P[i].second] = i;
      }
      for (auto &&p : P) {
        if (h % 2) p.first = -p.first;
        else swap(p.first, p.second);
      }
    }
    return ret;
  }
  ManhattanMST(const vector<pair<T, T>> &P)
      : KruskalMST<T>(P.size(), generateCandidates(P)) {}
};
