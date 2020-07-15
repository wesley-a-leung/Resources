#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
#include "../../../../Content/C++/graph/search/TransitiveClosure.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e4;
  vector<bitset<V>> matrix(V);
  for (int i = 0; i < E; i++) {
    int v, w;
    if (i < 1500) {
      v = rng() % V;
      w = rng() % V;
    } else {
      int c = rng() % 1000;
      v = rng() % (V / 1000) + (V / 1000) * c;
      w = rng() % (V / 1000) + (V / 1000) * c;
    }
    matrix[v][w] = 1;
  }
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureFloydWarshall<V> tc(matrix);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Floyd Warshall Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e4;
  vector<bitset<V>> matrix(V);
  for (int i = 0; i < E; i++) {
    int v = rng() % V;
    int w = rng() % V;
    if (v > w) swap(v, w);
    matrix[v][w] = 1;
  }
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureFloydWarshall<V> tc(matrix);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Floyd Warshall Lines) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e5;
  vector<bitset<V>> matrix(V);
  for (int i = 0; i < E; i++) {
    int v, w;
    if (i < 1500) {
      v = rng() % V;
      w = rng() % V;
    } else {
      int c = rng() % 1000;
      v = rng() % (V / 1000) + (V / 1000) * c;
      w = rng() % (V / 1000) + (V / 1000) * c;
    }
    matrix[v][w] = 1;
  }
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureFloydWarshall<V> tc(matrix);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Floyd Warshall Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e5;
  vector<bitset<V>> matrix(V);
  for (int i = 0; i < E; i++) {
    int v = rng() % V;
    int w = rng() % V;
    if (v > w) swap(v, w);
    matrix[v][w] = 1;
  }
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureFloydWarshall<V> tc(matrix);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Floyd Warshall Lines) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e4;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v, w;
    if (i < 1500) {
      v = rng() % V;
      w = rng() % V;
    } else {
      int c = rng() % 1000;
      v = rng() % (V / 1000) + (V / 1000) * c;
      w = rng() % (V / 1000) + (V / 1000) * c;
    }
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (SCC Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e4;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V;
    int w = rng() % V;
    if (v > w) swap(v, w);
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (SCC Lines) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test7() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e5;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v, w;
    if (i < 1500) {
      v = rng() % V;
      w = rng() % V;
    } else {
      int c = rng() % 1000;
      v = rng() % (V / 1000) + (V / 1000) * c;
      w = rng() % (V / 1000) + (V / 1000) * c;
    }
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 7 (SCC Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test8() {
  mt19937_64 rng(0);
  constexpr int V = 5e3, E = 2e5;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V;
    int w = rng() % V;
    if (v > w) swap(v, w);
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 8 (SCC Lines) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test9() {
  mt19937_64 rng(0);
  constexpr int V = 5e4, E = 2e5;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v, w;
    if (i < 1500) {
      v = rng() % V;
      w = rng() % V;
    } else {
      int c = rng() % 1000;
      v = rng() % (V / 1000) + (V / 1000) * c;
      w = rng() % (V / 1000) + (V / 1000) * c;
    }
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 9 (SCC Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test10() {
  mt19937_64 rng(0);
  constexpr int V = 5e4, E = 2e5;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V;
    int w = rng() % V;
    if (v > w) swap(v, w);
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  TransitiveClosureSCC<V> tc(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 10 (SCC Lines) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  int Q = 1e7;
  for (int i = 0; i < Q; i++) {
    int v = rng() % V, w = rng() % V;
    checkSum = 31 * checkSum + tc.reachable(v, w);
  }
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  cout << "Test Passed" << endl;
  return 0;
}
