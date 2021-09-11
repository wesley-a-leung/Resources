#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/binarysearchtrees/Splay.h"
using namespace std;

// Support online queries on connected components, after edges have been
//   added or removed, using a level structure (log V Euler Tour Trees)
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   cnt: the current number of connected components
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w, assuming
//     no edges currently exists
//   removeEdge(v, w): removes an edge between vertices v and w, assuming
//     an edge exists
//   connected(v, w): returns true if v and w are in the same
//     connected component, false otherwise
//   getSize(v): returns the size of the connected component containing
//     vertex v
// In practice, has a moderate constant, slower than DynamicConnectivityLCT and
//   DynamicConnectivityDivAndConq
// Time Complexity:
//   constructor: O(V)
//   addEdge, connected, getSize: O(log V) amortized
//   removeEdge: O((log V)^2) amortized
// Memory Complexity: O((V + E) log V)
// Tested:
//   https://www.acmicpc.net/problem/17465
struct DynamicConnectivityLevelStructure {
  struct Node {
    int v, sz; Node *l, *r, *p;
    Node(int v) : v(v), sz(1), l(nullptr), r(nullptr), p(nullptr) {}
    void update() {
      sz = 1;
      if (l) sz += l->sz;
      if (r) sz += r->sz;
    }
    void propagate() {}
  };
  struct Level : public Splay<Node> {
    vector<map<int, Node *>> T; vector<set<int>> G; vector<int> vis; int stamp;
    Level(int V) : T(V), G(V), vis(V, -1), stamp(0) {}
    void insert(Node *&root, int i, Node *n) {
      applyToRange(root, i, i - 1, [&] (Node *&x) { x = n; });
    }
    Node *disconnect(Node *&root, int l, int r) {
      Node *ret = nullptr;
      applyToRange(root, l, r, [&] (Node *&x) { ret = x; x = nullptr; });
      return ret;
    }
    void makeRoot(int v) {
      if (!T[v].empty()) {
        Node *l = T[v].begin()->second; int i = index(l, l);
        Node *r = disconnect(l, i, l->sz - 1); insert(l, 0, r);
        splay(T[v].begin()->second);
      }
    }
    int findRoot(int v) {
      if (T[v].empty()) return v;
      Node *x = T[v].begin()->second; splay(x);
      for (x->propagate(); x->l; (x = x->l)->propagate());
      splay(x); return x->v;
    }
    bool connected(int v, int w) { return findRoot(v) == findRoot(w); }
    int getSize(int v) {
      if (T[v].empty()) return 0;
      Node *x = T[v].begin()->second; splay(x); return x->sz / 2 + 1;
    }
    bool addEdge(int v, int w) {
      G[v].insert(w); G[w].insert(v); if (!connected(v, w)) {
        makeRoot(v); makeRoot(w);
        Node *l = T[v].empty() ? nullptr : T[v].begin()->second;
        insert(l, l ? l->sz : 0, T[v][w] = makeNode(v));
        if (!T[w].empty()) insert(l, l->sz, T[w].begin()->second);
        insert(l, l->sz, T[w][v] = makeNode(w)); return true;
      }
      return false;
    }
    bool removeEdge(int v, int w) {
      G[v].erase(w); G[w].erase(v); auto it1 = T[v].find(w);
      if (it1 != T[v].end()) {
        auto it2 = T[w].find(v); Node *a = it1->second, *b = it2->second;
        int i = index(a, a), j = index(b, b); if (i > j) swap(i, j);
        Node *c = disconnect(b, i, j); clear(disconnect(c, 0, 0));
        clear(disconnect(c, c->sz - 1, c->sz - 1));
        T[v].erase(it1); T[w].erase(it2); return true;
      }
      return false;
    }
    void dfs(Node *x, vector<int> &verts) {
      if (!x) return;
      if (vis[x->v] != stamp) { vis[x->v] = stamp; verts.push_back(x->v); }
      dfs(x->l, verts); dfs(x->r, verts);
    }
    vector<int> vertsInComp(int v) {
      if (T[v].empty()) return vector<int>{v};
      vector<int> ret; Node *x = T[v].begin()->second; splay(x); dfs(x, ret);
      stamp++; return ret;
    }
  };
  int V, cnt; deque<Level> lvls;
  DynamicConnectivityLevelStructure(int V) : V(V), cnt(V), lvls(1, Level(V)) {}
  void addEdge(int v, int w) { cnt -= lvls[0].addEdge(v, w); }
  void removeEdge(int v, int w) {
    int i = -1; for (int j = 0; j < int(lvls.size()); j++)
      if (lvls[j].removeEdge(v, w)) i = j;
    if (i == -1) return;
    pair<int, int> f = make_pair(-1, -1); cnt++;
    for (; f.first == -1 && i >= 0; i--) {
      if (i + 1 == int(lvls.size())) lvls.emplace_back(V);
      if (lvls[i].getSize(v) > lvls[i].getSize(w)) swap(v, w);
      int r = lvls[i].findRoot(w); for (int x : lvls[i].vertsInComp(v)) {
        for (auto &&y : lvls[i].T[x]) lvls[i + 1].addEdge(x, y.first);
        for (int y : lvls[i].G[x]) {
          if (f.first == -1 && lvls[i].findRoot(y) == r) f = make_pair(x, y);
          else { lvls[i + 1].G[x].insert(y); lvls[i + 1].G[y].insert(x); }
        }
      }
    }
    if (f.first != -1)
      for (cnt--, i++; i >= 0; i--) lvls[i].addEdge(f.first, f.second);
  }
  bool connected(int v, int w) { return lvls[0].connected(v, w); }
  int getSize(int v) { return lvls[0].getSize(v); }
};
