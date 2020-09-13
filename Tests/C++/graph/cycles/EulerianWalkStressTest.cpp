#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/cycles/EulerianWalk.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 2e5, E = 1e6;
  vector<pair<int, int>> edges;
  edges.reserve(E);
  edges.emplace_back(rng() % V, rng() % V);
  for (int i = 1; i < E - 1; i++) edges.emplace_back(edges.back().second, rng() % V);
  edges.emplace_back(edges.back().second, edges[0].first);
  const auto start_time = chrono::system_clock::now();
  vector<int> walk = eulerianWalk(V, edges, true, true);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v : walk) checkSum = 31 * checkSum + v;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
