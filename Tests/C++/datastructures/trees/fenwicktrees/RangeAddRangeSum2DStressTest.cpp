#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/RangeAddRangeSum2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2000;
  int M = 3000;
  RangeAddRangeSum2D<long long> ST(N, M);
  int Q = 1e5;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r);
    if (t == 0) {
      long long v = rng() % int(1e9) + 1;
      ST.update(u, d, l, r, v);
    } else {
      ans.push_back(ST.query(u, d, l, r));
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
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
