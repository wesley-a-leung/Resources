#pragma once
#include <bits/stdc++.h>
using namespace std;

// ignore next line length violation
// Based on https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/MaximumClique.h,
//   which itself is based on
//   https://gitlab.com/janezkonc/mcqd/blob/master/mcqd.h,
//   which has a GPL3 license
// Computes the clique with the maximum number of vertices in a graph
// Can be used to compute the maximum independent set by finding the maximum
//   clique on the complement graph
// Vertices are 0-indexed
// Constructor Arguments:
//   matrix: a vector of vector of booleans representing the adjacency matrix
//     of the graph, must be symmetric
// Fields:
//   maximumClique: a vertex of vertices in the maximum clique
// In practice, has a very small constant
// Time Complexity: exponential, much faster in practice
// Memory Complexity: O(V^2)
// Tested:
//   https://dmoj.ca/problem/clique
//   https://judge.yosupo.jp/problem/maximum_independent_set
struct MaximumClique {
  static constexpr const double limit = 0.025;
  struct Vertex { int i, d; Vertex(int i) : i(i), d(0) {} };
  vector<vector<bool>> matrix; vector<vector<int>> C;
  vector<int> maximumClique, q, S, old; double pk;
  void init(vector<Vertex> &r) {
    for (auto &&v: r) v.d = 0;
    for (auto &&v : r) for (auto &&w : r) v.d += matrix[v.i][w.i];
    sort(r.begin(), r.end(), [&] (const Vertex &v, const Vertex &w) {
      return v.d > w.d;
    });
    int mxD = r[0].d;
    for (int i = 0; i < int(r.size()); i++) r[i].d = min(i, mxD) + 1;
  }
  void expand(vector<Vertex> &R, int lvl = 1) {
    S[lvl] += S[lvl - 1] - old[lvl]; old[lvl] = S[lvl - 1];
    while (!R.empty()) {
      if (int(q.size()) + R.back().d <= int(maximumClique.size())) return;
      q.push_back(R.back().i); vector<Vertex> T;
      for (auto &&v : R) if (matrix[R.back().i][v.i]) T.emplace_back(v.i);
      if (!T.empty()) {
        if (S[lvl]++ / ++pk < limit) init(T);
        int mnk = max(int(maximumClique.size()) - int(q.size()) + 1, 1);
        int j = 0, mxk = 1; C[1].clear(); C[2].clear(); for (auto &&v : T) {
          int k = 1;
          auto f = [&] (int i) { return matrix[v.i][i]; };
          while (any_of(C[k].begin(), C[k].end(), f)) k++;
          if (k > mxk) C[(mxk = k) + 1].clear();
          if (k < mnk) T[j++].i = v.i;
          C[k].push_back(v.i);
        }
        if (j > 0) T[j - 1].d = 0;
        for (int k = mnk; k <= mxk; k++)
          for (int i : C[k]) { T[j].i = i; T[j++].d = k; }
        expand(T, lvl + 1);
      } else if (q.size() > maximumClique.size()) maximumClique = q;
      q.pop_back(); R.pop_back();
    }
  }
  MaximumClique(const vector<vector<bool>> &matrix)
      : matrix(matrix), C(matrix.size() + 1), S(C.size()), old(S), pk(0) {
    vector<Vertex> V; V.reserve(matrix.size());
    for (int i = 0; i < int(matrix.size()); i++) V.emplace_back(i);
    init(V); expand(V);
  }
};
