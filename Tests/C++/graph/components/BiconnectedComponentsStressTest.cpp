#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/components/BiconnectedComponents.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  StaticGraph G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int c = rng() % 2;
    int v = rng() % (V / 2) + (V / 2) * c, w = rng() % (V / 2) + (V / 2) * c;
    G.addBiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  vector<pair<int, int>> blockCutForestEdges;
  BCC bcc(G, blockCutForestEdges);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) {
    checkSum = 31 * checkSum + v;
    for (int id : bcc.ids[v]) checkSum = 31 * checkSum + id;
  }
  sort(blockCutForestEdges.begin(), blockCutForestEdges.end());
  for (auto &&e : blockCutForestEdges) {
    checkSum = 31 * checkSum + e.first;
    checkSum = 31 * checkSum + e.second;
  }
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
