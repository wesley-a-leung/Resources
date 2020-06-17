#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRange1D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 5e6;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e4) + 1;
  FenwickTreeRange1D<long long> FT(N);
  for (int i = 0; i < N; i++) FT.update(i, i, A[i]);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      long long v = rng() % int(1e4) + 1;
      FT.update(l, r, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.rsq(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  N: " << N << endl;
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
