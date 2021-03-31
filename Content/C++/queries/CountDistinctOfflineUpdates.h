#pragma once
#include <bits/stdc++.h>
#include "../datastructures/WaveletMatrixOfflineUpdates.h"
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for an array A of length N where the sequence of all update
//   indices and values are known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   C: a container representing a mapping from type T to a set of integers
// Constructor Arguments:
//   A: a vector of type T of the initial values in the array
//   updates: a vector of pairs of type int and T with the first element
//     representing the index being updated, and the second element being the
//     the value it is updated to
//   ...args: arguments to pass to the constructor of an instance of type C
// Functions:
//   processNextUpdate(): processes the next update (starting with the intial
//     array and proceeding in the order given by the updates vector)
//   query(l, r): returns the number of distinct values in the range [l, r] in
//     the current array
// In practice, has a small constant, slgithly faster than the online version
// Time Complexity:
//   constructor: O((N + U) log (N + U)) for U total updates
//   query: O(log(N + U) log((N + U) / 64)) for U total updates
// Memory Complexity: O(N + ((N + U) log (N + U)) / 64) for U total updates
// Tested:
//   https://dmoj.ca/problem/mnyc17p6
template <class T, class C = map<T, set<int>>>
struct CountDistinctOfflineUpdates {
  C M; int N, cur, ucnt; vector<int> en; vector<pair<int, int>> wupds;
  WaveletMatrixOfflineUpdates<int> wm;
  void add(int i, const T &v, vector<pair<int, int>> &wus) {
    set<int> &mv = M[v]; auto it = mv.upper_bound(i);
    int j = it == mv.begin() ? -1 : *prev(it); wus.emplace_back(i, j);
    if (it != mv.end()) wus.emplace_back(*it, i);
    mv.insert(it, i);
  }
  void rem(int i, const T &v, vector<pair<int, int>> &wus) {
    set<int> &mv = M[v]; auto it = mv.erase(mv.find(i));
    int j = it == mv.begin() ? -1 : *prev(it); wus.emplace_back(i, N);
    if (it != mv.end()) wus.emplace_back(*it, j);
  }
  vector<pair<int, int>> init1(vector<T> A,
                              const vector<pair<int, T>> &updates) {
    vector<pair<int, int>> ret;
    for (int i = 0; i < int(A.size()); i++) add(i, A[i], ret);
    cur = ret.size(); for (int i = 0; i < int(updates.size()); i++) {
      int j = updates[i].first; rem(j, A[j], ret);
      add(j, A[j] = updates[i].second, ret); en[i] = ret.size();
    }
    return ret;
  }
  WaveletMatrixOfflineUpdates<int> init2(const vector<T> &A) {
    int N = A.size(); vector<vector<int>> vals(N);
    for (auto &&wu : wupds) vals[wu.first].push_back(wu.second);
    return WaveletMatrixOfflineUpdates<int>(vector<int>(N, N), vals);
  }
  template <class ...Args>
  CountDistinctOfflineUpdates(const vector<T> &A,
                              const vector<pair<int, T>> &updates,
                              Args &&...args)
      : M(forward<Args>(args)...), N(A.size()), cur(0), ucnt(0),
        en(updates.size(), 0), wupds(init1(A, updates)), wm(init2(A)) {
    for (int k = 0; k < cur; k++) wm.updateToNextVal(wupds[k].first);
  }
  void processNextUpdate() {
    for (; cur < en[ucnt]; cur++) wm.updateToNextVal(wupds[cur].first);
    ucnt++;
  }
  int query(int l, int r) { return wm.rank(l, r, l); }
};
