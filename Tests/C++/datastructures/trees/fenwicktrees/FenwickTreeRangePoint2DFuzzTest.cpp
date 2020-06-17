#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 10 - rng() % 2;
    int M = 20 - rng() % 2;
    vector<vector<long long>> A(N, vector<long long>(M));
    for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
    FenwickTreeRangePoint<2, long long> FT(N, M);
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) FT.update(A[i][j], i, i, j, j);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
        if (u > d) swap(u, d);
        if (l > r) swap(l, r);
        long long v = rng() % int(1e9) + 1;
        for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) A[j][k] += v;
        FT.update(v, u, d, l, r);
      } else {
        int i = rng() % N, j = rng() % M;
        ans0.push_back(A[i][j]);
        ans1.push_back(FT.get(i, j));
      }
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum ^= a;
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
