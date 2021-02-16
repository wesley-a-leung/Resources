#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/shortestpaths/ShortestHamiltonianPath.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int V = rng() % 8, E = V == 0 ? 0 : rng() % (V * V);
    vector<vector<long long>> matrix(V, vector<long long>(V, LLONG_MAX));
    for (int i = 0; i < E; i++) {
      int v = rng() % V, w = rng() % V;
      long long weight = int(rng() % (int(2e9) + 1)) - int(1e9);
      matrix[v][w] = min(matrix[v][w], weight);
    }
    ShortestHamiltonianPath<long long> shp(matrix);
    vector<int> P(V);
    iota(P.begin(), P.end(), 0);
    long long mn = LLONG_MAX;
    do {
      long long dist = 0;
      bool valid = true;
      for (int i = 0; valid && i < V - 1; i++) {
        if (matrix[P[i]][P[i + 1]] == LLONG_MAX) valid = false;
        dist += matrix[P[i]][P[i + 1]];
      }
      if (valid) mn = min(mn, dist);
    } while (next_permutation(P.begin(), P.end()));
    assert(mn == shp.shortestPathDist);
    if (shp.shortestPathDist != LLONG_MAX) {
      long long checkDist = 0;
      for (int i = 0; i < V - 1; i++) checkDist += matrix[shp.ord[i]][shp.ord[i + 1]];
      assert(shp.shortestPathDist == checkDist);
    }
    checkSum = 31 * checkSum + shp.shortestPathDist;
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
