#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/shortestpaths/FloydWarshallAPSP.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 500, E = 2e4;
  vector<vector<long long>> matrix(V, vector<long long>(V, LLONG_MAX));
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    long long weight = rng() % int(1e9) + 1;
    matrix[v][w] = min(matrix[v][w], weight);
  }
  const auto start_time = chrono::system_clock::now();
  FloydWarshallAPSP<long long> apsp(matrix);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Floyd Warshall) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v = 0; v < V; v++) for (int w = 0; w < V; w++) checkSum = 31 * checkSum + apsp.dist[v][w];
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
