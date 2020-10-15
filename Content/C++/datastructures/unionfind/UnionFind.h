#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find by size with path compression
// Indices are 0-indexed
// Constructor Arguments:
//   N: the number of elements in the set
// Fields:
//   UF: a vector of integers representing the parent of each element in the
//     tree, or the negative of the size of the set if that element is a root
//   cnt: the current number of disjoint sets
// Functions:
//   find(v): finds the root of the set containing element v
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge, false otherwise
//   connected(v, w): returns true if v and w are in the same set,
//     false otherwise
//   getSize(v): returns the size of the set containing element v
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize: O(alpha N) amortized, O(log N) worse case
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://judge.yosupo.jp/problem/unionfind
//   https://dmoj.ca/problem/ds2
//   https://atcoder.jp/contests/abc177/tasks/abc177_d
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
