#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 1001;
    vector<int> A(N);
    for (auto &&ai : A) ai = rng() % 2;
    FenwickTree1D<int> FT(A.begin(), A.end());
    BitFenwickTree BFT(N);
    for (int i = 0; i < N; i++) BFT.set(i, A[i]);
    BFT.build();
    int Q = N == 0 ? 0 : 1000 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int i = rng() % N;
        int v = rng() % 2;
        FT.update(i, v - A[i]);
        A[i] = v;
        BFT.update(i, v);
      } else {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        ans0.push_back(FT.query(l, r));
        ans1.push_back(BFT.query(l, r));
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
