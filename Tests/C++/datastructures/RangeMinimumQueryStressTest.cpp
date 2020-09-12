#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/DisjointSparseTable.h"
#include "../../../Content/C++/datastructures/FischerHeunStructure.h"
#include "../../../Content/C++/datastructures/SparseTable.h"
#include "../../../Content/C++/datastructures/trees/segmenttrees/SegmentTreeBottomUp.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  SparseTable<int, Min> ST(A.begin(), A.end());
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
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
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  SparseTable<int, Min> ST(A.begin(), A.end());
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  DisjointSparseTable<int, Min> ST(A.begin(), A.end());
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Disjoint Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  DisjointSparseTable<int, Min> ST(A.begin(), A.end());
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Disjoint Sparse Table) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FischerHeunStructure<int, greater<int>> ST(A.begin(), A.end());
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Fischer Heun) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FischerHeunStructure<int, greater<int>> ST(A.begin(), A.end());
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Fischer Heun) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test7() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  struct Combine {
    using Data = int;
    using Lazy = int;
    static Data qdef() { return numeric_limits<int>::max(); }
    static Data merge(const Data &l, const Data &r) { return min(l, r); }
  };
  SegmentTreeBottomUp<Combine> ST(A.begin(), A.end());
  int Q = 1;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 7 (Segment Tree) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test8() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  struct Combine {
    using Data = int;
    using Lazy = int;
    static Data qdef() { return numeric_limits<int>::max(); }
    static Data merge(const Data &l, const Data &r) { return min(l, r); }
  };
  SegmentTreeBottomUp<Combine> ST(A.begin(), A.end());
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(ST.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 8 (Segment Tree) Passed" << endl;
  cout << "  N: " << N << endl;
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
  test5();
  test6();
  test7();
  test8();
  cout << "Test Passed" << endl;
  return 0;
}
