#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/minimumspanningtree/BoruvkaMST.h"
#include "../../../../Content/C++/graph/minimumspanningtree/KruskalMST.h"
#include "../../../../Content/C++/graph/minimumspanningtree/PrimMST.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int N = 1e6, M = 5e6;
  vector<KruskalMST<long long>::Edge> edges;
  for (int i = 0; i < M; i++) {
    int v = rng() % N, w = rng() % N;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  KruskalMST<long long> mst(N, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Kruskal) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (e.v > e.w) swap(e.v, e.w);
  sort(mst.mstEdges.begin(), mst.mstEdges.end(), [&] (const KruskalMST<long long>::Edge &a, KruskalMST<long long>::Edge &b) {
    return make_tuple(a.v, a.w, a.weight) < make_tuple(b.v, b.w, b.weight);
  });
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + e.v) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.w) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.weight) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int N = 1e6, M = 5e6;
  StaticWeightedGraph<long long> G(N);
  G.reserveDiEdges(M * 2);
  for (int i = 0; i < M; i++) {
    int v = rng() % N, w = rng() % N;
    long long weight = rng() % (long long)(1e9) + 1;
    G.addBiEdge(v, w, weight);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  PrimMST<long long> mst(move(G));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Prim) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (e.v > e.w) swap(e.v, e.w);
  sort(mst.mstEdges.begin(), mst.mstEdges.end(), [&] (const PrimMST<long long>::Edge &a, PrimMST<long long>::Edge &b) {
    return make_tuple(a.v, a.w, a.weight) < make_tuple(b.v, b.w, b.weight);
  });
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + e.v) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.w) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.weight) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int N = 1e6, M = 5e6;
  vector<BoruvkaMST<long long>::Edge> edges;
  for (int i = 0; i < M; i++) {
    int v = rng() % N, w = rng() % N;
    long long weight = rng() % (long long)(1e9) + 1;
    edges.emplace_back(v, w, weight);
  }
  const auto start_time = chrono::system_clock::now();
  BoruvkaMST<long long> mst(N, move(edges));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Boruvka) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = mst.mstWeight % (long long)(1e9 + 7);
  for (auto &&e : mst.mstEdges) if (e.v > e.w) swap(e.v, e.w);
  sort(mst.mstEdges.begin(), mst.mstEdges.end(), [&] (const BoruvkaMST<long long>::Edge &a, BoruvkaMST<long long>::Edge &b) {
    return make_tuple(a.v, a.w, a.weight) < make_tuple(b.v, b.w, b.weight);
  });
  for (auto &&e : mst.mstEdges) {
    checkSum = (31 * checkSum + e.v) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.w) % (long long)(1e9 + 7);
    checkSum = (31 * checkSum + e.weight) % (long long)(1e9 + 7);
  }
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  cout << "Test Passed" << endl;
  return 0;
}
