#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
#include "../../../../Content/C++/graph/search/TransitiveClosure.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  constexpr int V = 5e4, E = 1e5;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int c = rng() % 100;
    int v = rng() % (V / 100) + (V / 100) * c, w = rng() % (V / 100) + (V / 100) * c;
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = (31 * checkSum + tc.reachable(v, w)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
