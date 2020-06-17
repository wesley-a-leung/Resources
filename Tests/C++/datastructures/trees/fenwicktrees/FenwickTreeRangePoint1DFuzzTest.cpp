#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint.h"
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint1D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 100 - rng() % 5;
    vector<long long> A(N);
    for (auto &&ai : A) ai = rng() % int(1e9) + 1;
    FenwickTreeRangePoint1D<long long> FT1(N);
    FenwickTreeRangePoint1D<long long> FT2(A.begin(), A.end());
    FenwickTreeRangePoint<1, long long> FT3(N);
    for (int i = 0; i < N; i++) {
      FT1.update(i, i, A[i]);
      FT3.update(A[i], i, i);
    }
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1, ans2, ans3;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        long long v = rng() % int(1e9) + 1;
        for (int j = l; j <= r; j++) A[j] += v;
        FT1.update(l, r, v);
        FT2.update(l, r, v);
        FT3.update(v, l, r);
      } else {
        int j = rng() % N;
        ans0.push_back(A[j]);
        ans1.push_back(FT1.get(j));
        ans2.push_back(FT2.get(j));
        ans3.push_back(FT3.get(j));
      }
    }
    vector<long long> A1 = FT1.values(), A2 = FT2.values();
    assert(ans0 == ans1);
    assert(ans0 == ans2);
    assert(ans0 == ans3);
    assert(A == A1);
    assert(A == A2);
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
