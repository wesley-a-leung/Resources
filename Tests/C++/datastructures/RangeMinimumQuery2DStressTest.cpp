#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/SparseTable2D.h"
#include "../../../Content/C++/datastructures/trees/segmenttrees/SegmentTreeBottomUp2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  int N = 500, M = 1000;
  vector<vector<int>> A(N, vector<int>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
  SparseTable2D<int, Min> ST(A);
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r);
    ans.push_back(ST.query(u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  int N = 500, M = 1000;
  vector<vector<int>> A(N, vector<int>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
  SparseTable2D<int, Min> ST(A);
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r);
    ans.push_back(ST.query(u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

struct C {
  using Data = int;
  using Lazy = int;
  static Data qdef() { return numeric_limits<int>::max(); }
  static Data merge(const Data &l, const Data &r) {
    return min(l, r);
  }
  static Data applyLazy(const Data &, const Lazy &r) {
    return r;
  }
};

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 500, M = 1000;
  vector<vector<int>> A(N, vector<int>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
  SegmentTreeBottomUp2D<C> ST(A);
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r);
    ans.push_back(ST.query(u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Segment Tree) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 500, M = 1000;
  vector<vector<int>> A(N, vector<int>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
  SegmentTreeBottomUp2D<C> ST(A);
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r);
    ans.push_back(ST.query(u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Segment Tree) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  cout << "Test Passed" << endl;
  return 0;
}
