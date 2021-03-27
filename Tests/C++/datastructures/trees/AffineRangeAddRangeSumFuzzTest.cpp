#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeAffine.h"
#include "../../../../Content/C++/datastructures/trees/segmenttrees/SegmentTreeAffine.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    FenwickTreeAffine<long long> FT(N);
    SegmentTreeAffine<long long> ST(N);
    vector<long long> A(N, 0);
    int Q = N == 0 ? 0 : 100 - rng() % 5;
    vector<long long> ans0, ans1, ans2;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      if (t == 0) {
        long long m = rng() % int(1e2) + 1;
        long long b = rng() % int(1e2) + 1;
        for (int j = l; j <= r; j++) A[j] += (j - l + 1) * m + b;
        FT.update(l, r, m, b);
        ST.update(l, r, m, b);
      } else {
        long long sm = 0;
        for (int j = l; j <= r; j++) sm += A[j];
        ans0.push_back(sm);
        ans1.push_back(FT.query(l, r));
        ans2.push_back(ST.query(l, r));
      }
    }
    assert(ans0 == ans1);
    assert(ans0 == ans2);
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
