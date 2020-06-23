#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRange1D.h"
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
    FenwickTreeRange1D<long long> FT(N);
    for (int i = 0; i < N; i++) FT.update(i, i, A[i]);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        long long v = rng() % int(1e9) + 1;
        for (int j = l; j <= r; j++) A[j] += v;
        FT.update(l, r, v);
      } else {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        long long sm = 0;
        for (int j = l; j <= r; j++) sm += A[j];
        ans0.push_back(sm);
        ans1.push_back(FT.query(l, r));
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
