#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/queries/WaveletMatrixTreeAggregation.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
#include "../../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

struct R {
  using Data = int;
  using Lazy = int;
  static Data qdef() { return 0; }
  static Data merge(const Data &l, const Data &r) { return l + r; }
  static Data invData(const Data &v) { return -v; }
  BitFenwickTree FT;
  R(const vector<Data> &A) : FT(A.size()) {
    for (int i = 0; i < int(A.size()); i++) FT.set(i, A[i]);
    FT.build();
  }
  void update(int i, const Lazy &val) { FT.update(i, val); }
  Data query(int r) { return FT.query(r); }
};

void test1() {
  mt19937_64 rng(0);
  constexpr const int V = 2e5, Q = 2e5, MAXA = 1e9;
  vector<int> A(V, 0);
  for (int v = 0; v < V; v++) A[v] = rng() % MAXA;
  vector<int> X(V, 0);
  for (int v = 0; v < V; v++) X[v] = rng() % 2;
  StaticGraph G(V);
  for (int v = 1; v < V; v++) G.addBiEdge(rng() % v, v);
  G.build();
  const auto start_time = chrono::system_clock::now();
  WaveletMatrixTreeAggregation<int, R, false> wm(G, A, X);
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 3;
    if (t == 0) {
      int v = rng() % V;
      wm.update(v, X[v] ^= 1);
    } else if (t == 1) {
      int v = rng() % V, w = rng() % V;
      int a = rng() % MAXA, b = rng() % MAXA;
      if (a > b) swap(a, b);
      ans.push_back(wm.query(v, w, a, b));
    } else {
      int v = rng() % V, w = rng() % V;
      int k = rng() % ((wm.query(v, v, MAXA) + 1) * 2);
      vector<int> C;
      sort(C.begin(), C.end());
      pair<bool, int *> p = wm.bsearch(v, w, [&] (int agg) {
        return agg >= k;
      });
      ans.push_back(p.first ? *p.second : INT_MAX);
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  V: " << V << endl;
  cout << "  Q: " << Q << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
