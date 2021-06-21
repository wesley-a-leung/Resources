#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Computes the Delaunay Triangulation of a set of distinct points
// If all points are collinear, there is no triangulation
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
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/cco08p6
struct DelaunayTriangulation {
  static constexpr const pt inf = pt(numeric_limits<T>::max(),
                                     numeric_limits<T>::max());
  struct Quad {
    static vector<Quad> V;
    int rot, o; pt p; Quad(int rot) : rot(rot), o(-1), p(inf) {}
    int &r() { return V[rot].rot; }
    pt &f() { return V[r()].p; }
    int prv() { return V[V[rot].o].rot; }
    int nxt() { return V[r()].prv(); }
  };
  int head; vector<Quad> &V = Quad::V; vector<array<pt, 3>> tri;
  int makeQuad(int rot) { V.emplace_back(rot); return int(V.size()) - 1; }
  bool circ(pt p, pt a, pt b, pt c) {
    T p2 = norm(p), A = norm(a) - p2, B = norm(b) - p2, C = norm(c) - p2;
    return lt(0, area2(p, a, b) * C + area2(p, b, c) * A + area2(p, c, a) * B);
  }
  int makeEdge(pt a, pt b) {
    int r = ~head ? head : makeQuad(makeQuad(makeQuad(makeQuad(-1))));
    head = V[r].o; V[V[r].r()].r() = r; for (int i = 0; i < 4; i++) {
      r = V[r].rot; V[r].o = i & 1 ? r : V[r].r();
    }
    V[r].p = a; V[r].f() = b; return r;
  }
  void splice(int a, int b) {
    swap(V[V[V[a].o].rot].o, V[V[V[b].o].rot].o); swap(V[a].o, V[b].o);
  }
  int connect(int a, int b) {
    int q = makeEdge(V[a].f(), V[b].p);
    splice(q, V[a].nxt()); splice(V[q].r(), b); return q;
  }
  bool valid(int a, int base) {
    return ccw(V[a].f(), V[base].f(), V[base].p) > 0;
  }
  pair<int, int> rec(const vector<pt> &P, int lo, int hi) {
    int k = hi - lo + 1; if (k <= 3) {
      int a = makeEdge(P[lo], P[lo + 1]), b = makeEdge(P[lo + 1], P[hi]);
      if (k == 2) return make_pair(a, V[a].r());
      splice(V[a].r(), b); int side = sgn(ccw(P[lo], P[lo + 1], P[hi]));
      int c = side ? connect(b, a) : -1;
      return make_pair(side < 0 ? V[c].r() : a, side < 0 ? c : V[b].r());
    }
    int a, b, ra, rb, mid = lo + (hi - lo) / 2;
    tie(ra, a) = rec(P, lo, mid); tie(b, rb) = rec(P, mid + 1, hi);
    while ((ccw(V[b].p, V[a].f(), V[a].p) < 0 && (a = V[a].nxt()))
        || (ccw(V[a].p, V[b].f(), V[b].p) > 0 && (b = V[V[b].r()].o)));
    int base = connect(V[b].r(), a); if (V[a].p == V[ra].p) ra = V[base].r();
    if (V[b].p == V[rb].p) rb = base;
    while (true) {
      int l = V[V[base].r()].o; if (valid(l, base)) 
        while (circ(V[V[l].o].f(), V[base].f(), V[base].p, V[l].f())) {
          int t = V[l].o; splice(l, V[l].prv());
          splice(V[l].r(), V[V[l].r()].prv()); V[l].o = head; head = l; l = t;
        }
      int r = V[base].prv(); if (valid(r, base))
        while (circ(V[V[r].prv()].f(), V[base].f(), V[base].p, V[r].f())) {
          int t = V[r].prv(); splice(r, V[r].prv());
          splice(V[r].r(), V[V[r].r()].prv()); V[r].o = head; head = r; r = t;
        }
      if (!valid(l, base) && !valid(r, base)) break;
      if (!valid(l, base) || (valid(r, base)
          && circ(V[r].f(), V[r].p, V[l].f(), V[l].p)))
        base = connect(r, V[base].r());
      else base = connect(V[base].r(), V[l].r());
    }
    return make_pair(ra, rb);
  }
  DelaunayTriangulation(vector<pt> P) : head(-1) {
    sort(P.begin(), P.end()); assert(unique(P.begin(), P.end()) == P.end());
    V.reserve(P.size() * 16); if (int(P.size()) < 2) return;
    int e = rec(P, 0, int(P.size()) - 1).first, qi = 0, ind = 0;
    vector<bool> vis(V.size(), false); vector<int> q{e}; q.reserve(V.size());
    while (ccw(V[V[e].o].f(), V[e].f(), V[e].p) < 0) e = V[e].o;
    auto add = [&] {
      int c = e; do {
        if (ind % 3 == 0) tri.emplace_back();
        tri.back()[ind++ % 3] = V[c].p;
        vis[c] = true; q.push_back(V[c].r()); c = V[c].nxt();
      } while (c != e);
    };
    add(); tri.clear(); ind = 0; tri.reserve(V.size() / 3 + 1);
    while (qi < int(q.size())) if (!vis[e = q[qi++]]) add();
    assert(ind % 3 == 0); V = vector<Quad>();
  }
};

vector<DelaunayTriangulation::Quad> DelaunayTriangulation::Quad::V
    = vector<DelaunayTriangulation::Quad>();
