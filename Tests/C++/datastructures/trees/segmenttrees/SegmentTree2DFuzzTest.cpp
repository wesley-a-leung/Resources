#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/segmenttrees/SegmentTreeBottomUp2D.h"
using namespace std;

struct C {
  using Data = long long;
  using Lazy = long long;
  static Data qdef() { return numeric_limits<long long>::min(); }
  static Data merge(const Data &l, const Data &r) {
    return max(l, r);
  }
  static Data applyLazy(const Data &l, const Lazy &r) {
    return l + r;
  }
};

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    int M = rng() % 21;
    vector<vector<long long>> A(N, vector<long long>(M));
    for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
    SegmentTreeBottomUp2D<C> ST(A);
    int Q = N == 0 || M == 0 ? 0 : 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int i = rng() % N, j = rng() % M;
        long long v = rng() % int(1e9) + 1;
        A[i][j] += v;
        ST.update(i, j, v);
      } else {
        int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
        if (u > d) swap(u, d);
        if (l > r) swap(l, r);
        long long mx = C::qdef();
        for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) mx = max(mx, A[j][k]);
        ans0.push_back(mx);
        ans1.push_back(ST.query(u, d, l, r));
      }
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (2D vector constructor) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    int M = rng() % 21;
    vector<vector<long long>> A(N, vector<long long>(M));
    for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
    SegmentTreeBottomUp2D<C> ST(N, M, 0);
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) ST.update(i, j, A[i][j]);
    int Q = N == 0 || M == 0 ? 0 : 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int i = rng() % N, j = rng() % M;
        long long v = rng() % int(1e9) + 1;
        A[i][j] += v;
        ST.update(i, j, v);
      } else {
        int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
        if (u > d) swap(u, d);
        if (l > r) swap(l, r);
        long long mx = C::qdef();
        for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) mx = max(mx, A[j][k]);
        ans0.push_back(mx);
        ans1.push_back(ST.query(u, d, l, r));
      }
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (vdef constructor) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
