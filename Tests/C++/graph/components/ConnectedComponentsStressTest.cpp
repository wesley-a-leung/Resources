#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/components/ConnectedComponents.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  CC cc(V);
  for (int i = 0; i < E; i++) {
    int c = rng() % 100;
    int v = rng() % (V / 100) + (V / 100) * c, w = rng() % (V / 100) + (V / 100) * c;
    cc.addEdge(v, w);
  }
  cc.assign();
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) checkSum = 31 * checkSum + cc.id[v];
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
