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
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5, MAXA = 100;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    vector<int> A(N, 0);
    for (int i = 0; i < N; i++) A[i] = rng() % MAXA;
    vector<int> B(N, 0);
    for (int i = 0; i < N; i++) B[i] = rng() % 2;
    WaveletMatrixAggregation<int, R> wm(A, B);
    int Q = N == 0 ? 0 : rng() % 101;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 3;
      if (t == 0) {
        int j = rng() % N;
        wm.update(j, B[j] ^= 1);
      } else if (t == 1) {
        int l = rng() % N, r = rng() % N, a = rng() % MAXA, b = rng() % MAXA;
        if (l > r) swap(l, r);
        if (a > b) swap(a, b);
        int sm = 0;
        for (int j = l; j <= r; j++) if (a <= A[j] && A[j] <= b) sm += B[j];
        ans0.push_back(sm);
        ans1.push_back(wm.query(l, r, a, b));
      } else {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        int k = rng() % ((wm.query(l, r, MAXA) + 1) * 2);
        vector<int> C;
        for (int j = l; j <= r; j++) if (B[j]) C.push_back(A[j]);
        sort(C.begin(), C.end());
        if (k == 0) ans0.push_back(*min_element(A.begin(), A.end()));
        else if (k - 1 < int(C.size())) ans0.push_back(C[k - 1]);
        else ans0.push_back(INT_MAX);
        pair<bool, int *> p = wm.bsearch(l, r, [&] (int agg) {
          return agg >= k;
        });
        ans1.push_back(p.first ? *p.second : INT_MAX);
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
