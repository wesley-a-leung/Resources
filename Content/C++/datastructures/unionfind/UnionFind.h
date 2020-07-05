#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find / Disjoint Sets by size with path compression
// Indices are 0-indexed
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize: O(alpha N) amortized,
//                                   O(log N) worse case
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://judge.yosupo.jp/problem/unionfind
//   https://dmoj.ca/problem/ds2
struct UnionFind {
  vector<int> UF; int cnt; UnionFind(int N) : UF(N, -1), cnt(N) {}
  int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
  bool join(int v, int w) {
    if ((v = find(v)) == (w = find(w))) return false;
    if (UF[v] > UF[w]) swap(v, w);
    UF[v] += UF[w]; UF[w] = v; cnt--; return true;
  }
  bool connected(int v, int w) { return find(v) == find(w); }
  int getSize(int v) { return -UF[find(v)]; }
};
