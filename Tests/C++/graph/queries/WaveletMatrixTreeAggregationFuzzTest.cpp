#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/queries/WaveletMatrixTreeAggregation.h"
#include "../../../../Content/C++/graph/queries/WaveletMatrixHeavyLightDecomposition.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
#include "../../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

struct R1 {
  using Data = int;
  using Lazy = int;
  static Data qdef() { return 0; }
  static Data merge(const Data &l, const Data &r) { return l + r; }
  static Data invData(const Data &v) { return -v; }
  BitFenwickTree FT;
  R1(const vector<Data> &A) : FT(A.size()) {
    for (int i = 0; i < int(A.size()); i++) FT.set(i, A[i]);
    FT.build();
  }
  void update(int i, const Lazy &val) { FT.update(i, val); }
  Data query(int r) { return FT.query(r); }
};

struct R2 {
  using Data = int;
  using Lazy = int;
  static Data qdef() { return 0; }
  static Data merge(const Data &l, const Data &r) { return l + r; }
  BitFenwickTree FT;
  R2(const vector<Data> &A) : FT(A.size()) {
    for (int i = 0; i < int(A.size()); i++) FT.set(i, A[i]);
    FT.build();
  }
  void update(int i, const Lazy &val) { FT.update(i, val); }
  Data query(int l, int r) { return FT.query(l, r); }
};

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4, MAXA = 100;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int V = rng() % 101;
    vector<int> A(V, 0);
    for (int v = 1; v < V; v++) A[v] = rng() % MAXA;
    vector<int> X(V, 0);
    for (int v = 1; v < V; v++) X[v] = rng() % 2;
    StaticGraph G(V);
    for (int v = 1; v < V; v++) G.addBiEdge(rng() % v, v);
    G.build();
    WaveletMatrixTreeAggregation<int, R1, true> wm(G, A, X);
    WaveletMatrixHLD<int, R2, true> hld(G, A, X);
    int Q = V <= 1 ? 0 : rng() % 101;
    vector<int> par(V), dep(V);
    function<void(int, int, int)> dfs = [&] (int v, int prev, int d) {
      par[v] = prev;
      dep[v] = d;
      for (int w : G[v]) if (w != prev) dfs(w, v, d + 1);
    };
    if (V > 0) dfs(0, -1, 0);
    auto getPath = [&] (int v, int w) {
      vector<int> ret, ret2;
      while (v != w) {
        if (dep[v] > dep[w]) {
          ret.push_back(v);
          v = par[v];
        } else {
          ret2.push_back(w);
          w = par[w];
        }
      }
      reverse(ret2.begin(), ret2.end());
      ret.insert(ret.end(), ret2.begin(), ret2.end());
      return ret;
    };
    vector<int> ans0, ans1, ans2;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 3;
      if (t == 0) {
        int v;
        do {
          v = rng() % V;
        } while (v == 0);
        wm.update(v, X[v] ^= 1);
        hld.update(v, X[v]);
      } else if (t == 1) {
        int v = rng() % V, w;
        do {
          w = rng() % V;
        } while (v == w);
        int a = rng() % MAXA, b = rng() % MAXA;
        if (a > b) swap(a, b);
        int sm = 0;
        for (int x : getPath(v, w)) if (a <= A[x] && A[x] <= b) sm += X[x];
        ans0.push_back(sm);
        ans1.push_back(wm.query(v, w, a, b));
        ans2.push_back(hld.query(v, w, a, b));
      } else {
        int v = rng() % V, w;
        do {
          w = rng() % V;
        } while (v == w);
        int k = rng() % ((wm.query(v, v, MAXA) + 1) * 2);
        vector<int> C;
        for (int x : getPath(v, w)) if (X[x]) C.push_back(A[x]);
        sort(C.begin(), C.end());
        if (k == 0) ans0.push_back(*min_element(A.begin(), A.end()));
        else if (k - 1 < int(C.size())) ans0.push_back(C[k - 1]);
        else ans0.push_back(INT_MAX);
        pair<bool, int *> p = wm.bsearch(v, w, [&] (int agg) {
          return agg >= k;
        });
        ans1.push_back(p.first ? *p.second : INT_MAX);
        p = hld.bsearch(v, w, [&] (int agg) {
          return agg >= k;
        });
        ans2.push_back(p.first ? *p.second : INT_MAX);
      }
    }
    assert(ans0 == ans1);
    assert(ans0 == ans2);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Values on Edges) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4, MAXA = 100;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int V = rng() % 101;
    vector<int> A(V, 0);
    for (int v = 0; v < V; v++) A[v] = rng() % MAXA;
    vector<int> X(V, 0);
    for (int v = 0; v < V; v++) X[v] = rng() % 2;
    StaticGraph G(V);
    for (int v = 1; v < V; v++) G.addBiEdge(rng() % v, v);
    G.build();
    WaveletMatrixTreeAggregation<int, R1, false> wm(G, A, X);
    WaveletMatrixHLD<int, R2, false> hld(G, A, X);
    int Q = V == 0 ? 0 : rng() % 101;
    vector<int> par(V), dep(V);
    function<void(int, int, int)> dfs = [&] (int v, int prev, int d) {
      par[v] = prev;
      dep[v] = d;
      for (int w : G[v]) if (w != prev) dfs(w, v, d + 1);
    };
    if (V > 0) dfs(0, -1, 0);
    auto getPath = [&] (int v, int w) {
      vector<int> ret, ret2;
      while (v != w) {
        if (dep[v] > dep[w]) {
          ret.push_back(v);
          v = par[v];
        } else {
          ret2.push_back(w);
          w = par[w];
        }
      }
      ret.push_back(v);
      reverse(ret2.begin(), ret2.end());
      ret.insert(ret.end(), ret2.begin(), ret2.end());
      return ret;
    };
    vector<int> ans0, ans1, ans2;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 3;
      if (t == 0) {
        int v = rng() % V;
        wm.update(v, X[v] ^= 1);
        hld.update(v, X[v]);
      } else if (t == 1) {
        int v = rng() % V, w = rng() % V;
        int a = rng() % MAXA, b = rng() % MAXA;
        if (a > b) swap(a, b);
        int sm = 0;
        for (int x : getPath(v, w)) if (a <= A[x] && A[x] <= b) sm += X[x];
        ans0.push_back(sm);
        ans1.push_back(wm.query(v, w, a, b));
        ans2.push_back(hld.query(v, w, a, b));
      } else {
        int v = rng() % V, w = rng() % V;
        int k = rng() % ((wm.query(v, v, MAXA) + 1) * 2);
        vector<int> C;
        for (int x : getPath(v, w)) if (X[x]) C.push_back(A[x]);
        sort(C.begin(), C.end());
        if (k == 0) ans0.push_back(*min_element(A.begin(), A.end()));
        else if (k - 1 < int(C.size())) ans0.push_back(C[k - 1]);
        else ans0.push_back(INT_MAX);
        pair<bool, int *> p = wm.bsearch(v, w, [&] (int agg) {
          return agg >= k;
        });
        ans1.push_back(p.first ? *p.second : INT_MAX);
        p = hld.bsearch(v, w, [&] (int agg) {
          return agg >= k;
        });
        ans2.push_back(p.first ? *p.second : INT_MAX);
      }
    }
    assert(ans0 == ans1);
    assert(ans0 == ans2);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Values on Vertices) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
