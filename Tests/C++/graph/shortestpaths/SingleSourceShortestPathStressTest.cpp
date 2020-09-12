#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
#include "../../../../Content/C++/graph/shortestpaths/BellmanFordSSSP.h"
#include "../../../../Content/C++/graph/shortestpaths/ClassicalDijkstraSSSP.h"
#include "../../../../Content/C++/graph/shortestpaths/DijkstraSSSP.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 1e4, E = 4e4;
  vector<tuple<int, int, long long>> edges;
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  BellmanFordSSSP<long long> sssp(V, edges, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Bellman Ford) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int V = 1e4, E = 4e4;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    G.addDiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  ClassicalDijkstraSSSP<long long> sssp(G, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Classical Dijkstra) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int V = 1e4, E = 4e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    G.addDiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  ClassicalDijkstraSSSP<long long> sssp(G, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Classical Dijkstra) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  mt19937_64 rng(0);
  int V = 1e4, E = 4e4;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    G.addDiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  DijkstraSSSP<long long> sssp(G, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Dijkstra with PQ) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  mt19937_64 rng(0);
  int V = 1e4, E = 4e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    G.addDiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  DijkstraSSSP<long long> sssp(G, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Dijkstra with PQ) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  mt19937_64 rng(0);
  int V = 1e6, E = 4e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    G.addDiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  DijkstraSSSP<long long> sssp(G, 0);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Dijkstra with PQ) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + sssp.dist[v];
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  cout << "Test Passed" << endl;
  return 0;
}
