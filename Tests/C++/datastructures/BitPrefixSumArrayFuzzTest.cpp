#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/BitPrefixSumArray.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 1001;
    vector<int> A(N + 1, 0);
    BitPrefixSumArray bpsa(N);
    int U = N == 0 ? 0 : 1000 - rng() % 5;
    for (int i = 0; i < U; i++) {
      int j = rng() % N, v = rng() % 2;
      A[j + 1] = v;
      bpsa.set(j, v);
    }
    for (int i = 0; i < N; i++) assert(A[i + 1] == bpsa.get(i));
    partial_sum(A.begin(), A.end(), A.begin());
    bpsa.build();
    int Q = N == 0 ? 0 : 1000 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans0.push_back(A[r + 1] - A[l]);
      ans1.push_back(bpsa.query(l, r));
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
