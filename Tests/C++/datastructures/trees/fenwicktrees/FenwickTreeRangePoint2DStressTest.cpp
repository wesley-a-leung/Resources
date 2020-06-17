#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 4000;
  int M = 6000;
  vector<vector<long long>> A(N, vector<long long>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
  FenwickTreeRangePoint<2, long long> FT(N, M);
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) FT.update(A[i][j], i, i, j, j);
  int Q = 1e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r);
      long long v = rng() % int(1e9) + 1;
      FT.update(v, u, d, l, r);
    } else {
      int i = rng() % N, j = rng() % M;
      ans.push_back(FT.get(i, j));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
