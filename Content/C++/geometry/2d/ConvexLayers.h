#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Helper struct to maintain upper hull
struct DecrementalUpperHull {
  struct Link; using ptr = Link *;
  struct Link {
    int id; pt p; ptr prv, nxt;
    Link(int id, pt p) : id(id), p(p), prv(nullptr), nxt(nullptr) {}
  };
  struct Node {
    ptr chain, back, tangent;
    Node() : chain(nullptr), back(nullptr), tangent(nullptr) {}
  };
  int N; vector<Node> TR; vector<Link> links;
  template <class F, class G>
  pair<ptr, ptr> findBridge(ptr l, ptr r, F f, G g) {
    while (f(l) || f(r)) {
      if (!f(r) || (f(l) && g(pt(0, 0), f(l)->p - l->p, f(r)->p - r->p))) {
        if (g(l->p, f(l)->p, r->p)) l = f(l);
        else break;
      } else {
        if (!g(l->p, r->p, f(r)->p)) r = f(r);
        else break;
      }
    }
    return make_pair(l, r);
  }
  void fixChain(int x, ptr l, ptr r, bool rev) {
    if (rev) {
      tie(r, l) = findBridge(r, l, [&] (ptr q) { return q->prv; }, 
                             [&] (pt a, pt b, pt c) {
                               return ccw(a, b, c) >= 0;
                             });
    } else {
      tie(l, r) = findBridge(l, r, [&] (ptr q) { return q->nxt; },
                             [&] (pt a, pt b, pt c) {
                               return ccw(a, b, c) <= 0;
                             });
    }
    TR[x].tangent = l;
    TR[x].chain = TR[x * 2].chain; TR[x].back = TR[x * 2 + 1].back;
    TR[x * 2].chain = l->nxt; TR[x * 2 + 1].back = r->prv;
    if (l->nxt) l->nxt->prv = nullptr;
    else TR[x * 2].chain = nullptr;
    if (r->prv) r->prv->nxt = nullptr;
    else TR[x * 2 + 1].chain = nullptr;
    l->nxt = r; r->prv = l;
  }
  void build(int x, int tl, int tr) {
    if (tl == tr) { TR[x].chain = TR[x].back = &links[tl]; return; }
    int m = tl + (tr - tl) / 2;
    build(x * 2, tl, m); build(x * 2 + 1, m + 1, tr);
    fixChain(x, TR[x * 2].chain, TR[x * 2 + 1].chain, false);
  }
  void rob(int x, int y) {
    TR[x].chain = TR[y].chain; TR[x].back = TR[y].back;
    TR[y].chain = TR[y].back = nullptr;
  }
  void rem(int x, int tl, int tr, int i) {
    if (i < tl || tr < i) return;
    int m = tl + (tr - tl) / 2, y = x * 2 + int(i > m); if (!TR[x].tangent) {
      TR[y].chain = TR[x].chain; TR[y].back = TR[x].back;
      if (i <= m) rem(x * 2, tl, m, i);
      else rem(x * 2 + 1, m + 1, tr, i);
      rob(x, y); return;
    }
    ptr l = TR[x].tangent, r = l->nxt; l->nxt = TR[x * 2].chain;
    if (TR[x * 2].chain) TR[x * 2].chain->prv = l;
    else TR[x * 2].back = l;
    TR[x * 2].chain = TR[x].chain; r->prv = TR[x * 2 + 1].back;
    if (TR[x * 2 + 1].back) TR[x * 2 + 1].back->nxt = r;
    else TR[x * 2 + 1].chain = r;
    TR[x * 2 + 1].back = TR[x].back;
    if (TR[y].chain == TR[y].back && TR[y].chain->id == i) {
      TR[y].chain = TR[y].back = nullptr;
      rob(x, y ^ 1); TR[x].tangent = nullptr; return;
    }
    if (i <= m) {
      if (l->id == i) l = l->nxt;
      rem(x * 2, tl, m, i); if (!l) l = TR[x * 2].back;
    } else {
      if (r->id == i) r = r->prv;
      rem(x * 2 + 1, m + 1, tr, i); if (!r) r = TR[x * 2 + 1].chain;
    }
    fixChain(x, l, r, i <= m);
  }
  void rem(int i) {
    if (TR[1].chain == TR[1].back) {
      TR[1].chain = TR[1].back = nullptr; return;
    }
    rem(1, 0, N - 1, i);
  }
  DecrementalUpperHull(const vector<pt> &P)
      : N(P.size()), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1)) {
    links.reserve(N); for (int i = 0; i < N; i++) links.emplace_back(i, P[i]);
    build(1, 0, N - 1);
  }
  vector<int> getHull() {
    vector<int> ret;
    for (ptr x = TR[1].chain; x; x = x->nxt) ret.push_back(x->id);
    return ret;
  }
};

// Computes the convex layer each point is on
// Function Arguments:
//   P: a vector of points
// Return Value: a vector of integers representing the convex layer each
//   point is on, with 0 being the outermost layer, and increasing when
//   moving inwards
// In practice, has a small constant
// Time Complexity: O(N (log N)^2)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/convex_layers
vector<int> convexLayers(const vector<pt> &P) {
  if (P.empty()) return vector<int>();
  int N = P.size(); vector<int> ind(N); iota(ind.begin(), ind.end(), 0);
  sort(ind.begin(), ind.end(), [&] (int i, int j) { return P[i] < P[j]; });
  vector<pt> tmp(N); for (int i = 0; i < N; i++) tmp[i] = P[ind[i]];
  DecrementalUpperHull up(tmp);
  for (int i = 0; i < N; i++) tmp[i] = P[ind[N - i - 1]] * T(-1);
  DecrementalUpperHull down(move(tmp)); vector<int> ret(N, -1);
  for (int layer = 0, done = 0; done < N; layer++) {
    vector<int> hull; for (int i : up.getHull()) hull.push_back(i);
    for (int i : down.getHull()) hull.push_back(N - i - 1);
    for (int i : hull) if (ret[ind[i]] == -1) {
      ret[ind[i]] = layer; done++; up.rem(i); down.rem(N - i - 1);
    }
  }
  return ret;
}
