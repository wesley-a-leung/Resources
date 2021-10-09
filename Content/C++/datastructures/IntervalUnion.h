#pragma once
#include <bits/stdc++.h>
using namespace std;

// Helper struct to compare two pairs
template <class Cmp> struct PairCmp {
  Cmp cmp; PairCmp(Cmp cmp = Cmp()) : cmp(cmp) {}
  template <class T>
  bool operator () (const pair<T, T> &a, const pair<T, T> &b) const {
    if (cmp(a.first, b.first)) return true;
    if (cmp(b.first, a.first)) return false;
    return cmp(a.second, b.second);
  }
};

// Struct representing no operation
struct NoOp {
  template <class T> void operator () (const T &, const T &) const {}
};

// Adding and removing half-open intervals from a set such that the combined
//   intervals are disjoint
// Template Arguments:
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   Add: a struct with a callback function (can also be of type
//       std::function<T(T, T)>); in practice, custom struct is faster than
//       std::function
//     Required Functions:
//       operator (l, r): adds the interval [l, r) to the set of
//         disjoint intervals
//   Rem: a struct with a callback function (can also be of type
//       std::function<T(T, T)>); in practice, custom struct is faster than
//       std::function
//     Required Functions:
//       operator (l, r): removes the interval [l, r) from the set of
//         disjoint intervals
// Constructor Arguments:
//   cmp: an instance of Cmp
//   add: an instance of Add
//   rem: an instance of Rem
// Functions:
//   addInterval(L, R): adds an interval [L, R) to the set
//   removeInterval(L, R): removes the interval [L, R) from the set
// In practice, has a moderate constant
// Time Complexity:
//   addInterval, removeInterval: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/art6
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=973
template <class T, class Cmp = less<T>, class Add = NoOp, class Rem = NoOp>
struct IntervalUnion : public set<pair<T, T>, PairCmp<Cmp>> {
  Cmp cmp; Add add; Rem rem;
  IntervalUnion(Cmp cmp = Cmp(), Add add = Add(), Rem rem = Rem())
      : set<pair<T, T>, PairCmp<Cmp>>(PairCmp<Cmp>(cmp)),
        cmp(cmp), add(add), rem(rem) {}
  typename set<pair<T, T>, PairCmp<Cmp>>::iterator addInterval(T L, T R) {
    if (!cmp(L, R) && !cmp(R, L)) return this->end();
    auto it = this->lower_bound(make_pair(L, R)), before = it;
    while (it != this->end() && !cmp(R, it->first)) {
      R = max(R, it->second, cmp); rem(it->first, it->second);
      before = it = this->erase(it);
    }
    if (it != this->begin() && !cmp((--it)->second, L)) {
      L = min(L, it->first, cmp); R = max(R, it->second, cmp);
      rem(it->first, it->second); this->erase(it);
    }
    add(L, R); return this->emplace_hint(before, L, R);
  }
  void removeInterval(T L, T R) {
    if (!cmp(L, R) && !cmp(R, L)) return;
    auto it = addInterval(L, R); auto r2 = it->second;
    if (!cmp(it->first, L) && !cmp(L, it->first)) {
      rem(it->first, it->second); this->erase(it);
    } else (T &) it->second = L;
    if (cmp(R, r2) || cmp(r2, R)) { add(R, r2); this->emplace(R, r2); }
  }
};

// Given a set of intervals (by the PairCmp struct), combine
//   them into disjoint half-open intervals of the form [L, R)
// Range is modified in-place
// Template Arguments:
//   T: the type of the endpoints of the intervals
//   Cmp: the comparator to compare two points
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   A: a reference to a vector of pairs with the first element being the
//     inclusive left bound of the interval and the second element being the
//     exclusive right bound of the interval
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
  sort(A.begin(), A.end(), [&] (const pair<T, T> &a, const pair<T, T> &b) {
    if (cmp(a.first, b.first)) return true;
    if (cmp(b.first, a.first)) return false;
    return cmp(a.second, b.second);
  });
  int i = 0; for (int l = 0, r = 0, N = A.size(); l < N; l = r, i++) {
    A[i] = A[l]; for (r = l + 1; r < N && !cmp(A[i].second, A[r].first); r++)
      A[i].second = max(A[i].second, A[r].second, cmp);
  }
  A.erase(A.begin() + i, A.end()); return A;
}
