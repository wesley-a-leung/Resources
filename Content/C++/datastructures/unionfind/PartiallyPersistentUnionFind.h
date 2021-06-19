#pragma once
#include <bits/stdc++.h>
#include "../../search/BinarySearch.h"
using namespace std;

// Union Find by size supporting queries for a past version of
//   the data structure but only allowed for updates to the latest version
// Constructor Arguments:
//   N: the number of elements in the set
// Fields:
//   UF: a vector of vectors of pairs containing the time that the parent or
//     size has changed for an element, and the parent or size of the element
//     at that time; the second element in the pair represents the parent of
//     that element in the tree, or the negative of the size of the set if that
//     element is a root
//   cnt: a vector of pairs containing the time that the number of disjoint
//     sets changes and the number of disjoint sets at that time
//   curTime: the current time, begins at -1 before any joins, and increases
//     by 1 after every join call
// Functions:
//   find(t, v): finds the root of the set containing element v at time t
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge, false otherwise
//   connected(t, v, w): returns true if v and w are in the same set,
//     at time t, false otherwise
//   getSize(t, v): returns the size of the set containing element v at time t
//   getCnt(t): returns the number of disjoint sets at time t
//   getFirst(v, w): returns the first time that elements v and w are in the
//     same set, -1 if v == w and curTime + 1 if they are never in the
//     same set
// In practice, find, join, connected, getSize, getCnt have a small constant,
//   getFirst has a very small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize, getCnt: O(log N)
//   getFirst: O(log N log Q) after Q join operations
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1253/problem/F
struct PartiallyPersistentUnionFind {
  vector<vector<pair<int, int>>> UF; vector<pair<int, int>> cnt; int curTime;
  PartiallyPersistentUnionFind(int N)
      : UF(N, vector<pair<int, int>>{make_pair(curTime, -1)}),
        cnt{make_pair(curTime, N)}, curTime(-1) {}
  int find(int t, int v) {
    return UF[v].back().second < 0 || UF[v].back().first > t
           ? v : find(t, UF[v].back().second); 
  }
  bool join(int v, int w) {
    curTime++;
    if ((v = find(curTime, v)) == (w = find(curTime, w))) return false;
    int ufv = UF[v].back().second, ufw = UF[w].back().second;
    if (ufv > ufw) { swap(v, w); swap(ufv, ufw); }
    UF[v].emplace_back(curTime, ufv + ufw); UF[w].emplace_back(curTime, v);
    cnt.emplace_back(curTime, cnt.back().second - 1); return true;
  }
  bool connected(int t, int v, int w) { return find(t, v) == find(t, w); }
  int getSize(int t, int v) {
    v = find(t, v);
    return -prev(upper_bound(UF[v].begin(), UF[v].end(),
                             make_pair(t, INT_MAX)))->second;
  }
  int getCnt(int t) {
    return prev(upper_bound(cnt.begin(), cnt.end(),
                            make_pair(t, INT_MAX)))->second;
  }
  int getFirst(int v, int w) {
    return bsearch<FIRST>(-1, curTime + 1, [&] (int x) {
      return connected(x, v, w);
    });
  }
};
