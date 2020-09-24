#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find by size supporting undos
// Indices are 0-indexed
// Constructor Arguments:
//   N: the number of elements in the set
// Fields:
//   UF: a vector of integers representing the parent of each element in the
//     tree, or the negative of the size of the set if that element is a root
//   cnt: the current number of disjoint sets
//   history: a vector of tuples storing the history of all join calls that
//     returned true
// Functions:
//   find(v): finds the root of the set containing element v
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge, false otherwise
//   undo(): undoes the call to join that returned true by popping from the
//     history stack
//   connected(v, w): returns true if v and w are in the same set,
//     false otherwise
//   getSize(v): returns the size of the set containing element v
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize: O(log N)
//   undo: O(1)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/apio19p2
struct UnionFindUndo {
  vector<int> UF; int cnt; vector<tuple<int, int, int>> history;
  UnionFindUndo(int N) : UF(N, -1), cnt(N) { history.reserve(N); }
  int find(int v) { return UF[v] < 0 ? v : find(UF[v]); }
  bool join(int v, int w) {
    if ((v = find(v)) == (w = find(w))) return false;
    if (UF[v] > UF[w]) swap(v, w);
    history.emplace_back(v, w, UF[w]);
    UF[v] += UF[w]; UF[w] = v; cnt--; return true;
  }
  void undo() {
    int v, w, ufw; tie(v, w, ufw) = history.back(); history.pop_back();
    UF[w] = ufw; UF[v] -= UF[w]; cnt++;
  }
  bool connected(int v, int w) { return find(v) == find(w); }
  int getSize(int v) { return -UF[find(v)]; }
};
