#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Angle.h"
#include "Line.h"
using namespace std;

// Helper struct for polygonTriangulation
struct Chain {
  static T X; mutable Line l;
  Chain(T Y) : l(0, 1, Y) {}
  Chain(pt p, pt q) : l(p, q) {}
  bool operator < (const Chain &c) const {
    return lt((l.c + l.v.y * X) * c.l.v.x, (c.l.c + c.l.v.y * X) * l.v.x);
  }
};

// Helper struct for polygonTriangulation
struct ChainInfo {
  int chainId, last, link;
  ChainInfo(int chainId, int last)
      : chainId(chainId), last(last), link(last) {}
};

T Chain::X = 0;

// Triangulates a polygon such that all N - 2 triangles are disjoint, with the
//   union of the triangles being equal to the original polygon 
// Function Arguments:
//   P: the points of the simple polygon in ccw order, no two points can
//     be identical
// Return Value: a vector of arrays of 3 points, representing a triangle in
//   the triangulation, points given in ccw order
// In practice, has a moderate constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
vector<array<pt, 3>> polygonTriangulation(const vector<pt> &P) {
  vector<array<pt, 3>> ret;
  auto monotone = [&] (const vector<int> &ind) {
    auto add = [&] (int i, int j, int k) {
      ret.push_back(array<pt, 3>{P[ind[i]], P[ind[j]], P[ind[k]]});
    };
    auto pccw = [&] (int i, int j, int k) {
      return ccw(P[ind[i]], P[ind[j]], P[ind[k]]);
    };
    int st = min_element(ind.begin(), ind.end(), [&] (int i, int j) {
      return P[i].x < P[j].x;
    }) - ind.begin();
    auto prv = [&] (int i) { return i == 0 ? int(ind.size()) - 1 : i - 1; };
    auto nxt = [&] (int i) { return i + 1 == int(ind.size()) ? 0 : i + 1; };
    deque<int> lo{st}, hi{st}; for (int k = 1; k < int(ind.size()); k++) {
      int a = nxt(lo.back()), b = prv(hi.back());
      if (P[ind[a]].x < P[ind[b]].x) {
        while (int(lo.size()) >= 2
            && pccw(lo[lo.size() - 2], lo.back(), a) == 1) {
          add(lo[lo.size() - 2], lo.back(), a); lo.pop_back();
        }
        while (int(hi.size()) >= 2 && pccw(a, hi[1], hi[0]) == 1) {
          add(a, hi[1], hi[0]); hi.pop_front(); lo.front() = hi.front();
        }
        lo.push_back(a);
      } else {
        while (int(hi.size()) >= 2
            && pccw(b, hi.back(), hi[hi.size() - 2]) == 1) {
          add(b, hi.back(), hi[hi.size() - 2]); hi.pop_back();
        }
        while (int(lo.size()) >= 2 && pccw(lo[0], lo[1], b) == 1) {
          add(lo[0], lo[1], b); lo.pop_front(); hi.front() = lo.front();
        }
        hi.push_back(b);
      }
    }
  };
  int n = P.size(); vector<int> ord(n); iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&] (int a, int b) { return P[a].x < P[b].x; });
  auto isAdj = [&] (int i, int j) {
    int d = abs(i - j); return d == 1 || d == n - 1;
  };
  auto prv = [&] (int i) { return i == 0 ? n - 1 : i - 1; };
  auto nxt = [&] (int i) { return i + 1 == n ? 0 : i + 1; };
  vector<vector<pair<int, int>>> G(n); int curEdge = 0, curChain = 0;
  auto addBiEdge = [&] (int i, int j) {
    if (i == -1 || j == -1 || isAdj(i, j)) return;
    G[i].emplace_back(j, curEdge++); G[j].emplace_back(i, curEdge++);
  };
  for (int i = 0; i < n; i++) G[i].emplace_back(nxt(i), curEdge++);
  multimap<Chain, ChainInfo> lo, hi; for (int l = 0, r = 0; l < n; l = r) {
    for (r = l + 1; r < n && !lt(P[ord[l]].x, P[ord[r]].x); r++);
    sort(ord.begin() + l, ord.begin() + r, [&] (int a, int b) {
      return P[a].y < P[b].y;
    });
    Chain::X = P[ord[l]].x; for (int d = l, u = l; d < r; d = u) {
      for (u = d + 1; u < r && isAdj(ord[u - 1], ord[u]); u++);
      auto it1 = lo.upper_bound(Chain(P[ord[u - 1]].y));
      auto it2 = hi.lower_bound(Chain(P[ord[d]].y));
      if (it1 == lo.begin() || it2 == hi.end()
          || ((--it1)->second.chainId != it2->second.chainId
                && (nxt(it1->second.last) != ord[u - 1]
                      || prv(it2->second.last) != ord[d]))) {
        lo.emplace(Chain(P[ord[d]], P[nxt(ord[d])]),
                   ChainInfo(curChain, ord[d]));
        hi.emplace(Chain(P[ord[u - 1]], P[prv(ord[u - 1])]),
                   ChainInfo(curChain++, ord[u - 1]));
      } else if (it1->second.chainId != it2->second.chainId) {
        auto it3 = next(it2), it4 = prev(it1);
        addBiEdge(ord[u - 1], it1->second.link);
        addBiEdge(ord[d], it2->second.link);
        addBiEdge(ord[u - 1], it3->second.link);
        addBiEdge(ord[d], it4->second.link);
        lo.erase(it1); hi.erase(it2);
        it3->second.chainId = it4->second.chainId;
        it3->second.link = -1; it4->second.link = ord[u - 1];
      } else {
        bool conLo = nxt(it1->second.last) == ord[d];
        bool conHi = prv(it2->second.last) == ord[u - 1];
        if (conLo && conHi) {
          addBiEdge(ord[d], it1->second.link);
          addBiEdge(ord[d], it2->second.link);
          lo.erase(it1); hi.erase(it2);
        } else if (conLo) {
          addBiEdge(ord[d], it1->second.link);
          addBiEdge(ord[d], it2->second.link);
          it1->first.l = Line(P[ord[u - 1]], P[nxt(ord[u - 1])]);
          it1->second.last = it1->second.link = ord[u - 1];
          it2->second.link = -1;
        } else if (conHi) {
          addBiEdge(ord[d], it1->second.link);
          addBiEdge(ord[d], it2->second.link);
          it2->first.l = Line(P[ord[d]], P[prv(ord[d])]);
          it2->second.last = it2->second.link = ord[d];
          it1->second.link = -1;
        } else if (nxt(it1->second.last) == ord[u - 1]) {
          addBiEdge(ord[u - 1], it1->second.link);
          addBiEdge(ord[u - 1], it2->second.link);
          it1->first.l = Line(P[ord[d]], P[nxt(ord[d])]);
          it1->second.last = ord[d]; it1->second.link = ord[u - 1];
          it2->second.link = -1;
        } else if (prv(it2->second.last) == ord[d]) {
          addBiEdge(ord[d], it1->second.link);
          addBiEdge(ord[d], it2->second.link);
          it2->first.l = Line(P[ord[u - 1]], P[prv(ord[u - 1])]);
          it2->second.last = it2->second.link = ord[u - 1];
          it1->second.link = -1;
        } else {
          addBiEdge(ord[d], it1->second.link);
          addBiEdge(ord[d], it2->second.link);
          hi.emplace(Chain(P[ord[d]], P[prv(ord[d])]),
                     ChainInfo(it1->second.chainId, ord[d]));
          it1->second.link = it2->second.link = -1;
          it2->second.chainId = curChain;
          lo.emplace(Chain(P[ord[u - 1]], P[nxt(ord[u - 1])]),
                     ChainInfo(curChain++, ord[u - 1]));
        }
      }
    }
  }
  auto cmpAng = [&] (const pair<int, int> &a, const pair<int, int> &b) {
    return Angle(P[a.first]) < Angle(P[b.first]);
  };
  auto eqAng = [&] (const pair<int, int> &a, const pair<int, int> &b) {
    return Angle(P[a.first]) == Angle(P[b.first]);
  };
  for (int i = 0; i < n; i++) {
    Angle::setPivot(P[i]); sort(G[i].begin(), G[i].end(), cmpAng);
    G[i].erase(unique(G[i].begin(), G[i].end(), eqAng), G[i].end());
  }
  vector<bool> vis(curEdge, false);
  for (int i = 0; i < n; i++) for (auto &&e : G[i]) if (!vis[e.second]) {
    vector<int> ind; int cur = i, nxt = e.first, eid = e.second;
    while (!vis[eid]) {
      ind.push_back(cur); vis[eid] = true; Angle::setPivot(P[nxt]);
      auto it = lower_bound(G[nxt].begin(), G[nxt].end(),
                            make_pair(cur, -1), cmpAng);
      it = prev(it == G[nxt].begin() ? G[nxt].end() : it);
      cur = nxt; nxt = it->first; eid = it->second;
    }
    monotone(ind);
  }
  return ret;
}
