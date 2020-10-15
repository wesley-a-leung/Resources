#pragma once
#include <bits/stdc++.h>
#include "../trees/PersistentArray.h"
using namespace std;

// Persistent Union Find by size where copy assignment/constructor creates
//   a new version of the data structure
// Indices are 0-indexed
// Constructor Arguments:
//   N: the number of elements in the set
// Fields:
//   UF: a vector of integers representing the parent of each element in the
//     tree, or the negative of the size of the set if that element is a root
//   cnt: the current number of disjoint sets
// Functions:
//   find(v): finds the root of the set containing element v in the
//     current version
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge in the current version,
//     false otherwise
//   connected(v, w): returns true if v and w are in the same set in the
//     current version, false otherwise
//   getSize(v): returns the size of the set containing element v in the
//     current version
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize: O((log N)^2)
// Memory Complexity: O(N log N)
// Tested:
//   https://judge.yosupo.jp/problem/persistent_unionfind
struct PersistentUnionFind {
  PersistentArray<int> UF; int cnt;
  PersistentUnionFind(int N) : UF(N, -1), cnt(N) {}
  int find(int v) { int ufv = UF.get(v); return ufv < 0 ? v : find(ufv); }
  bool join(int v, int w) {
    if ((v = find(v)) == (w = find(w))) return false;
    int ufv = UF.get(v), ufw = UF.get(w);
    if (ufv > ufw) { swap(v, w); swap(ufv, ufw); }
    UF.set(v, ufv + ufw); UF.set(w, v); cnt--; return true;
  }
  bool connected(int v, int w) { return find(v) == find(w); }
  int getSize(int v) { return -UF.get(find(v)); }
};
