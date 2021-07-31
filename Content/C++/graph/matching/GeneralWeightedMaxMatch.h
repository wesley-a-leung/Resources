#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum weighted matching in a general undirected weighted
//   graph, such that each vertex is incident with at most one edge in the
//   matching, and the selected edges have the maximum sum of weights
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of each edge weight
// Constructor Arguments:
//   matrix: a V x V matrix containing the weight of the edge between two
//     vertices, 0 if an edge doesn't exist, must be symmetric and
//      non negative
//   INF: a value for infinity
// Fields:
//   V: the number of vertices in the graph
//   cardinality: the cardinality of the maximum matching
//   INF: a value for infinity
//   cost: the maximum cost of the weighted matching
//   mate: the other vertex in the matching, or -1 if unmatched
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V^3)
// Memory Complexity: O(V^2)
// Tested:
//   https://uoj.ac/problem/81
//   https://judge.yosupo.jp/problem/general_weighted_matching
template <class T> struct GeneralWeightedMaxMatch {
  struct Edge {
    int v, w; T weight; Edge() : v(0), w(0), weight(T()) {}
  };
  int V, VX, cardinality, curStamp; T INF, cost;
  vector<int> mate, match, slack, st, par, S, stamp; vector<T> lab;
  vector<vector<int>> flo, floFrom; vector<vector<Edge>> G; queue<int> q;
  T eDelta(const Edge &e) {
    return lab[e.v] + lab[e.w] - G[e.v][e.w].weight * T(2);
  }
  void updateSlack(int v, int x) {
    if (!slack[x] || eDelta(G[v][x]) < eDelta(G[slack[x]][x])) slack[x] = v;
  }
  void setSlack(int x) {
    slack[x] = 0; for (int v = 1; v <= V; v++)
      if (G[v][x].weight > T() && st[v] != x && S[st[v]] == 0)
        updateSlack(v, x);
  }
  void qPush(int x) {
    if (x <= V) q.push(x);
    else for (int t : flo[x]) qPush(t);
  }
  void setSt(int x, int b) {
    st[x] = b; if (x > V) for (int t : flo[x]) setSt(t, b);
  }
  int getPr(int b, int xr) {
    int pr = find(flo[b].begin(), flo[b].end(), xr) - flo[b].begin();
    if (pr % 2) {
      reverse(flo[b].begin() + 1, flo[b].end());
      return int(flo[b].size()) - pr;
    }
    return pr;
  }
  void setMatch(int v, int w) {
    Edge &e = G[v][w]; match[v] = e.w; if (v <= V) return;
    int xr = floFrom[v][e.v], pr = getPr(v, xr);
    for (int i = 0; i < pr; i++) setMatch(flo[v][i], flo[v][i ^ 1]);
    setMatch(xr, w); rotate(flo[v].begin(), flo[v].begin() + pr, flo[v].end());
  }
  void augment(int v, int w) {
    while (true) {
      int xnw = st[match[v]]; setMatch(v, w); if (!xnw) return;
      setMatch(xnw, v = st[par[w = xnw]]);
    }
  }
  int lca(int v, int w) {
    for (curStamp++; v || w; swap(v, w)) {
      if (!v) continue;
      if (stamp[v] == curStamp) return v;
      stamp[v] = curStamp; v = st[match[v]]; if (v) v = st[par[v]];
    }
    return 0;
  }
  void addBlossom(int v, int anc, int w) {
    int b = V + 1; while (b <= VX && st[b]) b++;
    if (b > VX) VX++;
    lab[b] = T(); S[b] = 0; match[b] = match[anc]; flo[b] = vector<int>{anc};
    auto blossom = [&] (int x) {
      for (int y; x != anc; x = st[par[y]]) {
        flo[b].push_back(x); flo[b].push_back(y = st[match[x]]); qPush(y);
      }
    };
    blossom(v); reverse(flo[b].begin() + 1, flo[b].end()); blossom(w);
    setSt(b, b);
    for (int x = 1; x <= VX; x++) G[b][x].weight = G[x][b].weight = T();
    for (int x = 1; x <= V; x++) floFrom[b][x] = 0;
    for (int xs : flo[b]) {
      for (int x = 1; x <= VX; x++)
        if (G[b][x].weight == T(0) || eDelta(G[xs][x]) < eDelta(G[b][x])) {
          G[b][x] = G[xs][x]; G[x][b] = G[x][xs];
        }
      for (int x = 1; x <= V; x++) if (floFrom[xs][x]) floFrom[b][x] = xs;
    }
    setSlack(b);
  }
  void expandBlossom(int b) {
    for (int t : flo[b]) setSt(t, t);
    int xr = floFrom[b][G[b][par[b]].v], pr = getPr(b, xr);
    for (int i = 0; i < pr; i += 2) {
      int xs = flo[b][i], xns = flo[b][i + 1]; par[xs] = G[xns][xs].v;
      S[xs] = 1; S[xns] = slack[xs] = slack[xns] = 0; qPush(xns);
    }
    S[xr] = 1; par[xr] = par[b];
    for (int i = pr + 1; i < int(flo[b].size()); i++) {
      int xs = flo[b][i]; S[xs] = -1; setSlack(xs);
    }
    st[b] = 0;
  }
  bool onFoundEdge(const Edge &e) {
    int v = st[e.v], w = st[e.w]; if (S[w] == -1) {
      par[w] = e.v; S[w] = 1; slack[w] = 0;
      int nv = st[match[w]]; S[nv] = slack[nv] = 0; qPush(nv);
    } else if (S[w] == 0) {
      int anc = lca(v, w);
      if (!anc) { augment(v, w); augment(w, v); return true; }
      addBlossom(v, anc, w);
    }
    return false;
  }
  bool matching() {
    q = queue<int>(); for (int x = 1; x <= VX; x++) {
      S[x] = -1; slack[x] = 0;
      if (st[x] == x && !match[x]) { par[x] = S[x] = 0; qPush(x); }
    }
    if (q.empty()) return false;
    while (true) {
      while (!q.empty()) {
        int v = q.front(); q.pop(); if (S[st[v]] == 1) continue;
        for (int w = 1; w <= V; w++)
          if (G[v][w].weight > T() && st[v] != st[w]) {
            if (eDelta(G[v][w]) == T()) {
              if (onFoundEdge(G[v][w])) return true;
            } else updateSlack(v, st[w]);
          }
      }
      T d = INF; for (int b = V + 1; b <= VX; b++)
        if (st[b] == b && S[b] == 1) d = min(d, lab[b] / T(2));
      for (int x = 1; x <= VX; x++) if (st[x] == x && slack[x]) {
        if (S[x] == -1) d = min(d, eDelta(G[slack[x]][x]));
        else if (S[x] == 0) d = min(d, eDelta(G[slack[x]][x]) / T(2));
      }
      for (int v = 1; v <= V; v++) {
        if (S[st[v]] == 0) {
          if (lab[v] <= d) return false;
          lab[v] -= d;
        } else if (S[st[v]] == 1) lab[v] += d;
      }
      for (int b = V + 1; b <= VX; b++) if (st[b] == b && S[b] != -1) {
        if (S[b] == 0) lab[b] += d * T(2);
        else lab[b] -= d * T(2);
      }
      q = queue<int>(); for (int x = 1; x <= VX; x++)
        if (st[x] == x && slack[x] && st[slack[x]] != x
            && eDelta(G[slack[x]][x]) == T() && onFoundEdge(G[slack[x]][x]))
          return true;
      for (int b = V + 1; b <= VX; b++)
        if (st[b] == b && S[b] == 1 && lab[b] == T()) expandBlossom(b);
    }
    return false;
  }
  GeneralWeightedMaxMatch(const vector<vector<T>> &matrix,
                          T INF = numeric_limits<T>::max())
      : V(matrix.size()), VX(V), cardinality(0), curStamp(0), INF(INF),
        cost(T()), mate(V, -1), match(V * 2 + 1, 0), slack(V * 2 + 1, 0),
        st(V * 2 + 1, 0), par(V * 2 + 1, 0), S(V * 2 + 1, 0),
        stamp(V * 2 + 1, 0), lab(V * 2 + 1, T()), flo(V * 2 + 1),
        floFrom(V * 2 + 1, vector<int>(V + 1, 0)),
        G(V * 2 + 1, vector<Edge>(V * 2 + 1)) {
    iota(st.begin(), st.begin() + V + 1, 0); T mx = T();
    for (int v = 1; v <= V; v++) for (int w = 1; w <= V; w++) {
      G[v][w].v = v; G[v][w].w = w; floFrom[v][w] = (v == w ?  v : 0);
      assert(G[v][w].weight >= T());
      mx = max(mx, G[v][w].weight = matrix[v - 1][w - 1]);
    }
    fill(lab.begin() + 1, lab.begin() + V + 1, mx);
    while (matching()) cardinality++;
    for (int v = 1; v <= V; v++) if (match[v] && match[v] < v) {
      mate[mate[v - 1] = G[v][match[v]].w - 1] = v - 1;
      cost += G[v][match[v]].weight;
    }
  }
};
