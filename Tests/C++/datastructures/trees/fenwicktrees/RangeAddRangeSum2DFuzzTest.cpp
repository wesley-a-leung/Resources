#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/RangeAddRangeSum2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 21;
    int M = rng() % 21;
    vector<vector<long long>> A(N, vector<long long>(M, 0));
    RangeAddRangeSum2D<long long> ST(N, M);
    int Q = N == 0 || M == 0 ? 0 : 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r);
      if (t == 0) {
        long long v = rng() % int(1e9) + 1;
        for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) A[j][k] += v;
        ST.update(u, d, l, r, v);
      } else {
        long long sm = 0;
        for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) sm += A[j][k];
        ans0.push_back(sm);
        ans1.push_back(ST.query(u, d, l, r));
      }
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
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
