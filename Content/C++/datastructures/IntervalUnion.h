#pragma once
#include <bits/stdc++.h>
using namespace std;

// Help struct to compare two pairs
template <class Cmp> struct PairCmp {
  template <class T>
  bool operator () (const pair<T, T> &a, const pair<T, T> &b) const {
    if (Cmp()(a.first, b.first)) return true;
    if (Cmp()(b.first, a.first)) return false;
    return Cmp()(a.second, b.second);
  } 
};

// Adding and removing intervals from a set such that the combined intervals
//   are non-overlapping
// Template Arguments:
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   addInterval(L, R): adds an interval [L, R] to the set
//   removeInterval(L, R): removes the interval [L, R] from the set
// In practice, has a moderate constant
// Time Complexity:
//   addInterval, removeInterval: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/art6
template <class T, class Cmp = less<T>>
struct IntervalUnion : public set<pair<T, T>, PairCmp<Cmp>> {
  typename set<pair<T, T>, PairCmp<Cmp>>::iterator addInterval(T L, T R) {
    if (!Cmp()(L, R) && !Cmp()(R, L)) return this->end();
    auto it = this->lower_bound(make_pair(L, R)), before = it;
    while (it != this->end() && !Cmp()(R, it->first)) {
      R = max(R, it->second, Cmp()); before = it = this->erase(it);
    }
    if (it != this->begin() && !Cmp()((--it)->second, L)) {
      L = min(L, it->first, Cmp()); R = max(R, it->second, Cmp());
      this->erase(it);
    }
    return this->emplace_hint(before, L, R);
  }
  void removeInterval(T L, T R) {
    if (!Cmp()(L, R) && !Cmp()(R, L)) return;
    auto it = addInterval(L, R); auto r2 = it->second;
    if (!Cmp()(it->first, L) && !Cmp()(L, it->first)) this->erase(it);
    else (T &) it->second = L;
    if (Cmp()(R, r2) || Cmp()(r2, R)) this->emplace(R, r2);
  }
};

// Given a set of intervals (by the PairCmp struct), combine
//   them into disjoint intervals of the form [L, R]
// Range is modified in-place
// Template Arguments:
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   A: a reference to a vector of pairs with the first element being the
//     inclusive left bound of the interval and the second element being the
//     inclusive right bound of the interval
//   cmp: an instance of the Cmp struct
// Return Value: a reference to the modified vector
// In practice, has a small constant
// Time Complexity: O(N log N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/art6
//   https://open.kattis.com/problems/drawingcircles
template <class T, class Cmp = less<T>>
vector<pair<T, T>> &intervalUnion(vector<pair<T, T>> &A, Cmp cmp = Cmp()) {
  sort(A.begin(), A.end(), PairCmp<Cmp>());
  int i = 0; for (int l = 0, r = 0, N = A.size(); l < N; l = r, i++) {
    A[i] = A[l]; for (r = l + 1; r < N && !cmp(A[i].second, A[r].first); r++)
      A[i].second = max(A[i].second, A[r].second, cmp);
  }
  A.erase(A.begin() + i, A.end()); return A;
}
