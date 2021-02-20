#pragma once
#include <bits/stdc++.h>
#include "../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Graphic Matroid for spanning forests
// Constructor Arguments:
//   V: the number of vertices in the simple graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the graph between vertices v and w
// Functions:
//   clear(): clears all edges in the independent set
//   add(i): adds the ith edge to the independent set
//   independent(i): returns whether adding edge i to the current
//     existing independent set would still result in an independent set
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V + E)
//   clear: O(K) for K edges added
//   add, independent: O(log V)
// Memory Complexity: O(V + E)
// Tested:
//   https://www.spoj.com/problems/COIN/
struct GraphicMatroid {
  int V; UnionFindUndo uf; vector<pair<int, int>> edges;
  GraphicMatroid(int V, const vector<pair<int, int>> &edges)
      : V(V), uf(V), edges(edges) {}
  void clear() { while (!uf.history.empty()) uf.undo(); }
  void add(int i) { uf.join(edges[i].first, edges[i].second); }
  bool independent(int i) {
    return !uf.connected(edges[i].first, edges[i].second);
  }
};

// Colorful Matroid
// Constructor Arguments:
//   color: the colors of each element in the ground set
// Functions:
//   clear(): clears all elements in the independent set
//   add(i): adds the ith element to the independent set
//   independent(i): returns whether adding element i to the current
//     existing independent set would still result in an independent set
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N) for N elements in the ground set
//   clear: O(C) for C colors
//   add, independent: O(1)
// Memory Complexity: O(N + C) for N elements in the ground set and C colors
// Tested:
//   https://codeforces.com/gym/102156/problem/D
//   https://www.spoj.com/problems/COIN/
struct ColorfulMatroid {
  vector<bool> in; vector<int> color;
  ColorfulMatroid(const vector<int> &color)
      : in(*max_element(color.begin(), color.end()) + 1, false),
        color(color) {}
  void clear() { fill(in.begin(), in.end(), false); }  
  void add(int i) { in[color[i]] = true; }
  bool independent(int i) { return !in[color[i]]; }  
};

// Matroid for vectors in Z2
// Template Arguments:
//   BITS: the number of bits
// Constructor Arguments:
//   vec: the vectors in the ground set
// Functions:
//   clear(): clears all vector in the independent set
//   add(i): adds the ith vector to the independent set
//   independent(i): returns whether adding vector i to the current
//     existing independent set would still result in an independent set
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N BITS) for N vectors in the ground set
//   clear: O(BITS)
//   add, independent: O(BITS^2 / 64)
// Memory Complexity: O(N BITS) for N vectors in the ground set
// Tested:
//   https://codeforces.com/gym/102156/problem/D
template <const int BITS> struct Z2Matroid {
  vector<bitset<BITS>> basis, vec;
  Z2Matroid(const vector<bitset<BITS>> &vec) : basis(BITS), vec(vec) {}
  void clear() { fill(basis.begin(), basis.end(), bitset<BITS>()); }
  void add(int i) {
    bitset<BITS> v = vec[i]; for (int j = 0; j < BITS; j++) if (v[j]) {
      if (basis[j].none()) { basis[j] = v; return; }
      v ^= basis[j];
    }
  }
  bool independent(int i) {
    bitset<BITS> v = vec[i]; for (int j = 0; j < BITS; j++) if (v[j]) {
      if (basis[j].none()) return true;
      v ^= basis[j];
    }
    return false;
  }
};

// Linear Matroid for vectors
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   vec: the vectors in the ground set
//   EPS: a value for epsilon
// Functions:
//   clear(): clears all vector in the independent set
//   add(i): adds the ith vector to the independent set
//   independent(i): returns whether adding vector i to the current
//     existing independent set would still result in an independent set
// In practice, has a small constant
// Time Complexity:
//   constructor: O(NV) for N vectors of size V in the ground set
//   clear: O(V) for vectors of size V
//   add, independent: O(V^2) for vectors of size V
// Memory Complexity: O(NV) for N vectors of size V in the ground set
template <class T> struct LinearMatroid {
  static T abs(T a) { return a >= 0 ? a : -a; }
  int vars; T EPS; vector<vector<T>> basis, vec;
  LinearMatroid(const vector<vector<T>> &vec, T EPS = T(1e-9))
      : vars(vec.empty() ? 0 : vec[0].size()), EPS(EPS),
        basis(vars), vec(vec) {}
  void clear() { fill(basis.begin(), basis.end(), vector<T>()); }
  void add(int i) {
    vector<T> v = vec[i]; for (int j = 0; j < vars; j++) if (abs(v[j]) > EPS) {
      if (basis[j].empty()) { basis[j] = v; return; }
      T alpha = v[j] / basis[j][j];
      for (int k = j; k < vars; k++) v[j] -= alpha * basis[j][k];
    }
  }
  bool independent(int i) {
    vector<T> v = vec[i]; for (int j = 0; j < vars; j++) if (abs(v[j]) > EPS) {
      if (basis[j].empty()) return true;
      T alpha = v[j] / basis[j][j];
      for (int k = j; k < vars; k++) v[j] -= alpha * basis[j][k];
    }
    return false;
  }
};

// Find the largest subset of a ground set of N elements that is
//   independent in both matroids
// A matroid must have the basic properties of:
//   - the empty set is independent
//   - any subset of an independent set is empty
//   - if independent set A is smaller than independent set B, then there is
//       at least 1 element of B can be added to A without loss of independency
// Elements are 0-indexed
// Template Arguments:
//   Matroid1: the type of the first matroid
//     Required Functions:
//       clear(): clears all elements in the independent set of this matroid
//       add(i): adds element i to the independent set of this matroid
//       independent(i): returns whether adding element i to the current
//         existing independent set would still result in an independent set
//   Matroid2: the type of the second matroid
//     Required Functions: same as Matroid1
// Constructor Arguments:
//   N: the number of elements in the ground set
//   m1: an instance of the first matroid
//   m2: an instance of the second matroid
// Fields:
//   N: the number of elements in the ground set
//   inSet: a vector of booleans indicating whether each element is in the
//     largest independent set or not
//   independentSet: a vector of the indices of the elements in the largest
//     independent set
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(NI sqrt I) * (time complexity of m1.add, m2.add,
//     m1.independent, and m2.independent) where I is the size of the largest
//     independent set
// Memory Complexity: O(N + M1 + M2) where M1 and M2 are the memory
//   complexities of m1 and m2
// Tested:
//   https://codeforces.com/gym/102156/problem/D
//   https://www.spoj.com/problems/COIN/
template <class Matroid1, class Matroid2> struct MatroidIntersection {
  int N; vector<bool> inSet; vector<int> independentSet;
  bool augment(Matroid1 &m1, Matroid2 &m2) {
    vector<int> par(N + 1, -1); queue<int> q; q.push(N); while (!q.empty()) {
      int v = q.front(); q.pop(); if (inSet[v]) {
        m1.clear(); for (int i = 0; i < N; i++) if (inSet[i] && i != v)
          m1.add(i);
        for (int i = 0; i < N; i++)
          if (!inSet[i] && par[i] == -1 && m1.independent(i)) {
            par[i] = v; q.push(i);
          }
      } else {
        auto backE = [&] {
          m2.clear(); for (int c = 0; c < 2; c++) for (int i = 0; i < N; i++)
            if ((v == i || inSet[i]) && (par[i] == -1) == c) {
              if (!m2.independent(i)) {
                if (c) { par[i] = v; q.push(i); return i; }
                else return -1;
              }
              m2.add(i);
            }
          return N;
        };
        for (int w = backE(); w != -1; w = backE()) if (w == N) {
          for (; v != N; v = par[v]) inSet[v] = !inSet[v];
          return true;
        } 
      }
    }
    return false;
  }
  MatroidIntersection(int N, Matroid1 m1, Matroid2 m2)
      : N(N), inSet(N + 1, false) {
    m1.clear(); m2.clear(); inSet[N] = true; for (int i = N - 1; i >= 0; i--)
      if (m1.independent(i) && m2.independent(i)) {
        inSet[i] = true; m1.add(i); m2.add(i);
      }
    while (augment(m1, m2));
    inSet.pop_back();
    for (int i = 0; i < N; i++) if (inSet[i]) independentSet.push_back(i);
  }
};
