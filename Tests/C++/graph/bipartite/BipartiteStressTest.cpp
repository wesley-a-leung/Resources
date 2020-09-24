#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/bipartite/Bipartite.h"
#include "../../../../Content/C++/graph/bipartite/SemiDynamicBipartite.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  StaticGraph G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    G.addBiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  Bipartite bp(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Bipartite) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = bp.bipartite;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  SemiDynamicBipartite sdbp(V);
  const auto start_time = chrono::system_clock::now();
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    sdbp.addEdge(v, w);
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Semi Dynamic Bipartite) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = sdbp.bipartiteGraph;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  StaticGraph G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int v = rng() % (V / 2) * 2, w = rng() % (V / 2) * 2 + 1;
    G.addBiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  Bipartite bp(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Bipartite) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = bp.bipartite;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  SemiDynamicBipartite sdbp(V);
  const auto start_time = chrono::system_clock::now();
  for (int i = 0; i < E; i++) {
    int v = rng() % (V / 2) * 2, w = rng() % (V / 2) * 2 + 1;
    sdbp.addEdge(v, w);
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Semi Dynamic Bipartite) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = sdbp.bipartiteGraph;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  cout << "Test Passed" << endl;
  return 0;
}
