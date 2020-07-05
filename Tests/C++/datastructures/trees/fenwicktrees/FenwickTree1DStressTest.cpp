#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTree.h"
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTree1D.h"
#include "../../../../../Content/C++/search/BinarySearch.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTree1D<long long> FT(A.begin(), A.end());
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      long long v = rng() % int(1e9) + 1;
      FT.update(i, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (1D Iterator Constructor) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTree1D<long long> FT(N);
  for (int i = 0; i < N; i++) FT.update(i, A[i]);
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      long long v = rng() % int(1e9) + 1;
      FT.update(i, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (1D default value constructor) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTree<1, long long> FT(N);
  for (int i = 0; i < N; i++) FT.update(i, A[i]);
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      long long v = rng() % int(1e9) + 1;
      FT.update(i, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (ND) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTree1D<long long> FT(A.begin(), A.end());
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 4;
    if (t == 0) {
      int i = rng() % N;
      long long v = rng() % int(1e9) + 1;
      FT.update(i, v);
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    } else if (t == 2) {
      long long TOT = FT.query(N - 1) * 2 + 1;
      long long v = rng() % TOT;
      ans.push_back(FT.bsearch(v, less<long long>()));
    } else {
      long long TOT = FT.query(N - 1) * 2 + 1;
      long long v = rng() % TOT;
      ans.push_back(FT.bsearch(v, less_equal<long long>()));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (lower_bound, upper_bound) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTree1D<long long> FT(A.begin(), A.end());
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 4;
    if (t == 0) {
      int i = rng() % N;
      long long v = rng() % int(1e9) + 1;
      FT.update(i, v);
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    } else if (t == 2) {
      long long TOT = FT.query(N - 1) * 2 + 1;
      long long v = rng() % TOT;
      ans.push_back(bsearch<FIRST>(0, N, [&] (int k) {
          return FT.query(k) > v;
      }));
    } else {
      long long TOT = FT.query(N - 1) * 2 + 1;
      long long v = rng() % TOT;
      ans.push_back(bsearch<FIRST>(0, N, [&] (int k) {
          return FT.query(k) >= v;
      }));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (bsearch with query) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  cout << "Test Passed" << endl;
  return 0;
}
