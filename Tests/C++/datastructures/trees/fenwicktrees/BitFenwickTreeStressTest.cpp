#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTree1D.h"
#include "../../../../../Content/C++/search/BinarySearch.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr const int N = 1e7, Q = 1e7;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % 2;
  FenwickTree1D<int> FT(A);
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      FT.update(i, v - A[i]);
      A[i] = v;
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Fenwick Tree) Passed" << endl;
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
  constexpr const int N = 1e7, Q = 1e7;
  BitFenwickTree BFT(N);
  for (int i = 0; i < N; i++) BFT.set(i, rng() % 2);
  BFT.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      BFT.update(i, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(BFT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Bit Fenwick Tree) Passed" << endl;
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
  constexpr const int N = 1e9, Q = 1e7;
  BitFenwickTree BFT(N);
  for (int i = 0; i < N; i++) BFT.set(i, rng() % 2);
  BFT.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      BFT.update(i, v);
    } else {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(BFT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Bit Fenwick Tree) Passed" << endl;
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
  constexpr const int N = 1e7, Q = 1e7;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % 2;
  FenwickTree1D<int> FT(A);
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 4;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      FT.update(i, v - A[i]);
      A[i] = v;
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(FT.query(l, r));
    } else if (t == 2) {
      int TOT = FT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(FT.bsearch(v, less<int>()));
    } else if (t == 3) {
      int TOT = FT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(FT.bsearch(v, less_equal<int>()));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Fenwick Tree, lower_bound, upper_bound) Passed" << endl;
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
  constexpr const int N = 1e7, Q = 1e7;
  BitFenwickTree BFT(N);
  for (int i = 0; i < N; i++) BFT.set(i, rng() % 2);
  BFT.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 4;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      BFT.update(i, v);
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(BFT.query(l, r));
    } else if (t == 2) {
      int TOT = BFT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(BFT.bsearch(v, less<int>()));
    } else if (t == 3) {
      int TOT = BFT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(BFT.bsearch(v, less_equal<int>()));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Bit Fenwick Tree, lower_bound, upper_bound) Passed" << endl;
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
  constexpr const int N = 1e7, Q = 1e7;
  BitFenwickTree BFT(N);
  for (int i = 0; i < N; i++) BFT.set(i, rng() % 2);
  BFT.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int t = rng() % 4;
    if (t == 0) {
      int i = rng() % N;
      int v = rng() % 2;
      BFT.update(i, v);
    } else if (t == 1) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      ans.push_back(BFT.query(l, r));
    } else if (t == 2) {
      int TOT = BFT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(bsearch<FIRST>(0, N, [&] (int k) {
        return BFT.query(k) >= v;
      }));
    } else if (t == 3) {
      int TOT = BFT.query(N - 1) * 2 + 1;
      int v = rng() % TOT;
      ans.push_back(bsearch<FIRST>(0, N, [&] (int k) {
        return BFT.query(k) > v;
      }));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Bit Fenwick Tree, bsearch with query) Passed" << endl;
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
  cout << "Test Passed" << endl;
  return 0;
}
