#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Line.h"
using namespace std;

// Helper struct for bentleyOttmann
struct Seg {
  static T X; mutable pt p, q; mutable int i; bool isQuery;
  pair<T, T> y() const {
    pt v = q - p; return make_pair(cross(v, p) + v.y * X, v.x);
  }
  Seg(pt p, pt q, int i) : p(p), q(q), i(i), isQuery(false) {
    assert(!eq(p.x, q.x)); if (p > q) swap(this->p, this->q);
  }
  Seg(T Y) : p(X, Y), q(p), i(-1), isQuery(true) {}
  bool operator < (const Seg &s) const {
    if (isQuery) {
      pair<T, T> p2 = s.y(); return lt(p.y * p2.second, p2.first);
    } else if (s.isQuery) {
      pair<T, T> p1 = y(); return lt(p1.first, s.p.y * p1.second);
    }
    pair<T, T> p1 = y(), p2 = s.y();
    return lt(p1.first * p2.second, p2.first * p1.second);
  }
};

T Seg::X = 0;

// Helper struct for bentleyOttmann
struct SegEvent {
  pt p; int type, i, j;
  SegEvent(pt p, int type, int i, int j) : p(p), type(type), i(i), j(j) {}
  bool operator > (const SegEvent &e) const {
    return eq(e.p.x, p.x) ? (eq(e.type, type) ? lt(e.p.y, p.y)
                                              : lt(e.type, type))
                          : lt(e.p.x, p.x);
  }
};

// Helper struct for bentleyOttmann
struct VerticalSeg {
  T lo, hi; int i; VerticalSeg(T lo, T hi, int i) : lo(lo), hi(hi), i(i) {}
  bool operator < (const VerticalSeg &vs) const { return lt(hi, vs.hi); }
};

// Runs a callback on all pairs of intersecting line segments (proper or not)
// Template Arguments:
//   F: the type of the function f
// Function Arguments:
//   segs: a vector of pairs of the endpoints of the line segments 
//   f(i, j): the function to run a callback on for each pair of intersecting
//     line segments, where i and j are the indices of the intersecting
//     line segments; each unordered pair (i, j) is passed at most once
// In practice, has a moderate constant
// Time Complexity: O((N + K) log N) for N line segments and K intersections
// Memory Complexity: O(N + K)
// Tested:
//   Fuzz Tested
//   https://open.kattis.com/problems/polygon
//   https://open.kattis.com/problems/scholarslawn
template <class F> void bentleyOttmann(const vector<pair<pt, pt>> &segs, F f) {
  std::priority_queue<SegEvent, vector<SegEvent>, greater<SegEvent>> events;
  int n = segs.size(); for (int i = 0; i < n; i++) {
    pt a, b; tie(a, b) = segs[i]; if (a > b) swap(a, b);
    int isEq = eq(a.x, b.x);
    events.emplace(a, isEq, i, i); events.emplace(b, 4 - isEq, i, i);
  }
  multiset<Seg> active; using iter = decltype(active)::iterator;
  vector<iter> ptr(n, active.end()); set<pair<int, int>> seen;
  auto checkSeen = [&] (int i, int j) {
    if (seen.count(make_pair(i, j)) || seen.count(make_pair(j, i)))
      return true;
    seen.emplace(i, j); return false;
  };
  auto checkInter = [&] (iter a, iter b) {
    vector<pt> p = segSegIntersection(a->p, a->q, b->p, b->q);
    if (int(p.size()) == 1 && cross(a->q - a->p, b->q - b->p) < 0) {
      if (!checkSeen(a->i, b->i)) f(a->i, b->i);
      events.emplace(p[0], 2, a->i, b->i);
    }
  };
  auto checkRange = [&] (int i, T lo, T hi) {
    auto it = active.lower_bound(Seg(lo));
    auto check = [&] {
      pair<T, T> y = it->y(); return !lt(hi * y.second, y.first);
    };
    for (; it != active.end() && check(); it++) {
      pt a, b; tie(a, b) = segs[i];
      if (segSegIntersects(a, b, it->p, it->q) && !checkSeen(i, it->i))
        f(i, it->i);
    }
  };
  multiset<VerticalSeg> vertical; while (!events.empty()) {
    SegEvent e = events.top(); events.pop(); Seg::X = e.p.x; if (e.type == 0) {
      checkRange(e.i, e.p.y, e.p.y);
      ptr[e.i] = active.emplace(segs[e.i].first, segs[e.i].second, e.i);
      if (ptr[e.i] != active.begin()) checkInter(prev(ptr[e.i]), ptr[e.i]);
      if (next(ptr[e.i]) != active.end()) checkInter(ptr[e.i], next(ptr[e.i]));
    } else if (e.type == 1) {
      T lo = segs[e.i].first.y, hi = segs[e.i].second.y;
      if (lo > hi) swap(lo, hi);
      checkRange(e.i, lo, hi);
      auto it = vertical.lower_bound(VerticalSeg(lo, lo, e.i));
      for (; it != vertical.end(); it++) if (!checkSeen(e.i, it->i))
        f(e.i, it->i);
      vertical.emplace(lo, hi, e.i);
    } else if (e.type == 2) {
      if (next(ptr[e.i]) != ptr[e.j]) continue;
      if (cross(ptr[e.i]->q - ptr[e.i]->p, ptr[e.j]->q - ptr[e.j]->p) < 0) {
        swap(ptr[e.i], ptr[e.j]); swap(ptr[e.i]->i, ptr[e.j]->i);
        swap(ptr[e.i]->p, ptr[e.j]->p); swap(ptr[e.i]->q, ptr[e.j]->q);
        if (ptr[e.j] != active.begin()) checkInter(prev(ptr[e.j]), ptr[e.j]);
        if (next(ptr[e.i]) != active.end())
          checkInter(ptr[e.i], next(ptr[e.i]));
      }
    } else if (e.type == 4) {
      if (ptr[e.i] != active.begin() && next(ptr[e.i]) != active.end())
        checkInter(prev(ptr[e.i]), next(ptr[e.i]));
      active.erase(ptr[e.i]); ptr[e.i] = active.end();
      checkRange(e.i, e.p.y, e.p.y);
    } else vertical.clear();
  }
}

// Transforms a set of line segments into another set of line segments
//   such that the union of the line segments remains the same, and any
//   intersection between any pair of segments is an endpoint of both segments
// Function Arguments:
//   segs: a vector of pairs of the endpoints of the line segments 
// Return Value: the resulting vector of pairs of the endpoints of the
//   line segments after the transformation
// In practice, has a moderate constant
// Time Complexity: O((N + K) log N) for N line segments and K intersections
// Memory Complexity: O(N + K)
// Tested:
//   https://open.kattis.com/problems/scholarslawn
vector<pair<pt, pt>> segArrangement(const vector<pair<pt, pt>> &segs) {
  int n = segs.size(); vector<vector<pt>> ptsOnSegs(n); set<pair<pt, pt>> ret;
  bentleyOttmann(segs, [&] (int i, int j) {
    for (auto &&p : segSegIntersection(segs[i].first, segs[i].second,
                                       segs[j].first, segs[j].second)) {
      ptsOnSegs[i].push_back(p);
      ptsOnSegs[j].push_back(p);
    }
  });
  for (int i = 0; i < n; i++) {
    ptsOnSegs[i].push_back(segs[i].first);
    ptsOnSegs[i].push_back(segs[i].second);
    sort(ptsOnSegs[i].begin(), ptsOnSegs[i].end());
    ptsOnSegs[i].erase(unique(ptsOnSegs[i].begin(), ptsOnSegs[i].end()),
                       ptsOnSegs[i].end());
    for (int j = 0; j < int(ptsOnSegs[i].size()) - 1; j++) {
      pt a = ptsOnSegs[i][j], b = ptsOnSegs[i][j + 1];
      if (!ret.count(make_pair(a, b)) && !ret.count(make_pair(b, a)))
        ret.emplace(a, b);
    }
  }
  return vector<pair<pt, pt>>(ret.begin(), ret.end());
}
