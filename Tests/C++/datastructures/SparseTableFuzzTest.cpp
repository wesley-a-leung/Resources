#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/SparseTable.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(1e9) + 1;
    SparseTable<int, Min> ST(A.begin(), A.end());
    int Q = N == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      int mn = A[l];
      for (int j = l + 1; j <= r; j++) mn = min(mn, A[j]);
      ans0.push_back(mn);
      ans1.push_back(ST.query(l, r));
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
