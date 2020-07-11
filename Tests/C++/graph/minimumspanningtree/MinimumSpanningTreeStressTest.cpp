#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/minimumspanningtree/BoruvkaMST.h"
#include "../../../../Content/C++/graph/minimumspanningtree/KruskalMST.h"
#include "../../../../Content/C++/graph/minimumspanningtree/PrimMST.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 1e4, E = 5e6;
  vector<KruskalMST<long long>::Edge> edges;
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  KruskalMST<long long> mst(V, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Kruskal) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int V = 1e6, E = 5e6;
  vector<KruskalMST<long long>::Edge> edges;
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  KruskalMST<long long> mst(V, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Kruskal) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int V = 1e4, E = 5e6;
  vector<BoruvkaMST<long long>::Edge> edges;
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  BoruvkaMST<long long> mst(V, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Boruvka) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  mt19937_64 rng(0);
  int V = 1e6, E = 5e6;
  vector<BoruvkaMST<long long>::Edge> edges;
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  BoruvkaMST<long long> mst(V, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Boruvka) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  mt19937_64 rng(0);
  int V = 1e4, E = 5e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    G.addBiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  PrimMST<long long> mst(move(G));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Prim) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  mt19937_64 rng(0);
  int V = 1e6, E = 5e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    G.addBiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  PrimMST<long long> mst(move(G));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Prim) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test7() {
  mt19937_64 rng(0);
  int V = 1e4, E = 5e6;
  StaticWeightedGraph<long long> G(V);
  G.reserveDiEdges(E * 2);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % (long long)(1e9) + 1;
    G.addBiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  ClassicalPrimMST<long long> mst(move(G));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 7 (Classical Prim) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (get<0>(e) > get<1>(e)) swap(get<0>(e), get<1>(e));
  sort(mst.mstEdges.begin(), mst.mstEdges.end());
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + get<0>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<1>(e)) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + get<2>(e)) % (long long)(1e9 + 7);
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
  cout << "Test Passed" << endl;
  return 0;
}
