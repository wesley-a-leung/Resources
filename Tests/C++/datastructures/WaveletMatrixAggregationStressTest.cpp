#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/WaveletMatrixAggregation.h"
#include "../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

struct R {
  using Data = int;
  using Lazy = int;
  static Data qdef() { return 0; }
  static Data merge(const Data &l, const Data &r) { return l + r; }
  BitFenwickTree FT;
  R(const vector<Data> &A) : FT(A.size()) {
    for (int i = 0; i < int(A.size()); i++) FT.set(i, A[i]);
    FT.build();
  }
  void update(int i, const Lazy &val) { FT.update(i, val); }
  Data query(int l, int r) { return FT.query(l, r); }
};

void test1() {
  mt19937_64 rng(0);
  constexpr const int N = 1e6, Q = 1e6, MAXA = 1e9;
  long long checkSum = 0;
  vector<int> A(N, 0);
  for (int i = 0; i < N; i++) A[i] = rng() % MAXA;
  vector<int> B(N, 0);
  for (int i = 0; i < N; i++) B[i] = rng() % 2;
  const auto start_time = chrono::system_clock::now();
  WaveletMatrixAggregation<int, R> wm(A, B);
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 3;
    if (t == 0) {
      int j = rng() % N;
      wm.update(j, B[j] ^= 1);
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N, a = rng() % MAXA, b = rng() % MAXA;
      if (l > r) swap(l, r);
      if (a > b) swap(a, b);
      ans.push_back(wm.query(l, r, a, b));
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      int k = rng() % ((wm.query(l, r, MAXA) + 1) * 2);
      pair<bool, int *> p = wm.bsearch(l, r, [&] (int agg) {
        return agg >= k;
      });
      ans.push_back(p.first ? *p.second : INT_MAX);
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  for (auto &&a : ans) checkSum = 31 * checkSum + a; 
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
