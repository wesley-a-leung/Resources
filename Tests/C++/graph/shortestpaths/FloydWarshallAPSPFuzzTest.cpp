#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/shortestpaths/FloydWarshallAPSP.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int V = rng() % 11, E = V == 0 ? 0 : rng() % (V * V);
    vector<vector<long long>> matrix(V, vector<long long>(V, LLONG_MAX));
    for (int i = 0; i < E; i++) {
      int v = rng() % V, w = rng() % V;
      long long weight = int(rng() % (int(2e9) + 1)) - int(1e9);
      matrix[v][w] = min(matrix[v][w], weight);
    }
    FloydWarshallAPSP<long long> apsp(matrix);
    for (int v = 0; v < V; v++) for (int w = 0; w < V; w++) {
      if (abs(apsp.dist[v][w]) != apsp.INF) {
        vector<tuple<int, int, long long>> path = apsp.getPath(v, w);
        long long sm = 0;
        for (auto &&e : path) {
          sm += get<2>(e);
          assert(get<2>(e) == matrix[get<0>(e)][get<1>(e)]);
        }
        assert(sm == apsp.dist[v][w]);
      }
      checkSum = 31 * checkSum + apsp.dist[v][w];
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
