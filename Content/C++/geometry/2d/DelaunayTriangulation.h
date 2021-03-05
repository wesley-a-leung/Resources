#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Computes the Delaunay Triangulation of a set of distinct points
// If all points are collinear points, there is triangulation
// If there are 4 or more points on the same circle, the triangulation is
//   ambiguous, otherwise it is unique
// Each circumcircle does not completely contain any of the input points
// Constructor Arguments:
//   P: the distinct points
// Fields:
//   tri: a vector of arrays of 3 points, representing a triangle in
//     the triangulation, points given in ccw order
// In practice, has a moderate constant
// Time Complexity:
//   construction: O(N log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://dmoj.ca/problem/cco08p6
struct DelaunayTriangulation {
  static constexpr const pt inf = pt(numeric_limits<T>::max(),
                                     numeric_limits<T>::max());
  struct Quad; using Q = Quad *;
  struct Quad {
    Q o, rot; pt p; bool mark;
    Quad(ref p) : o(nullptr), rot(nullptr), p(p), mark(false) {}
    Q r() { return rot->rot; }
    pt f() { return r()->p; }
    Q prv() { return rot->o->rot; }
    Q nxt() { return r()->prv(); }
  };
  deque<Quad> buf; vector<array<pt, 3>> tri;
  Q makeQuad(ref p) { buf.emplace_back(p); return &buf.back(); }
  bool circ(ref p, ref a, ref b, ref c) {
    T p2 = norm(p), A = norm(a) - p2, B = norm(b) - p2, C = norm(c) - p2;
    return lt(0, area2(p, a, b) * C + area2(p, b, c) * A + area2(p, c, a) * B);
  }
  Q makeEdge(ref a, ref b) {
    Q q[] = {makeQuad(a), makeQuad(inf), makeQuad(b), makeQuad(inf)};
    for (int i = 0; i < 4; i++) {
      q[i]->o = q[-i & 3]; q[i]->rot = q[(i + 1) & 3];
    }
    return q[0];
  }
  void splice(Q a, Q b) { swap(a->o->rot->o, b->o->rot->o); swap(a->o, b->o); }
  Q connect(Q a, Q b) {
    Q q = makeEdge(a->f(), b->p); splice(q, a->nxt()); splice(q->r(), b);
    return q;
  }
  bool valid(Q a, Q base) { return ccw(a->f(), base->f(), base->p) > 0; }
  pair<Q, Q> rec(const vector<pt> &P, int lo, int hi) {
    int k = hi - lo + 1; if (k <= 3) {
      Q a = makeEdge(P[lo], P[lo + 1]), b = makeEdge(P[lo + 1], P[hi]);
      if (k == 2) return make_pair(a, a->r());
      splice(a->r(), b); int side = sgn(ccw(P[lo], P[lo + 1], P[hi]));
      Q c = side ? connect(b, a) : nullptr;
      return make_pair(side < 0 ? c->r() : a, side < 0 ? c : b->r());
    }
    Q a, b, ra, rb; int mid = lo + (hi - lo) / 2;
    tie(ra, a) = rec(P, lo, mid); tie(b, rb) = rec(P, mid + 1, hi);
    while ((ccw(b->p, a->f(), a->p) < 0 && (a = a->nxt()))
        || (ccw(a->p, b->f(), b->p) > 0 && (b = b->r()->o)));
    Q base = connect(b->r(), a); if (a->p == ra->p) ra = base->r();
    if (b->p == rb->p) rb = base;
    while (true) {
      Q l = base->r()->o; if (valid(l, base))
        while (circ(l->o->f(), base->f(), base->p, l->f())) {
          Q t = l->o; splice(l, l->prv()); splice(l->r(), l->r()->prv());
          l = t;
        }
      Q r = base->prv(); if (valid(r, base))
        while (circ(r->prv()->f(), base->f(), base->p, r->f())) {
          Q t = r->prv(); splice(r, r->prv()); splice(r->r(), r->r()->prv());
          r = t;
        }
      if (!valid(l, base) && !valid(r, base)) break;
      if (!valid(l, base) || (valid(r, base)
          && circ(r->f(), r->p, l->f(), l->p))) base = connect(r, base->r());
      else base = connect(base->r(), l->r());
    }
    return make_pair(ra, rb);
  }
  DelaunayTriangulation(vector<pt> P) {
    sort(P.begin(), P.end(), pt_lt());
    assert(unique(P.begin(), P.end(), pt_eq()) == P.end());
    if (int(P.size()) < 2) return;
    Q e = rec(P, 0, int(P.size()) - 1).first; vector<Q> q{e}; int qi = 0;
    while (ccw(e->o->f(), e->f(), e->p) < 0) e = e->o;
    auto add = [&] {
      Q c = e; do {
        c->mark = true; P.push_back(c->p); q.push_back(c->r()); c = c->nxt();
      } while (c != e);
    };
    add(); P.clear();
    while (qi < int(q.size())) if (!(e = q[qi++])->mark) add();
    assert(P.size() % 3 == 0); tri.reserve(P.size() / 3);
    for (int i = 0; i < int(P.size()); i += 3)
      tri.emplace_back(array<pt, 3>{P[i], P[i + 1], P[i + 2]});
  }
};
