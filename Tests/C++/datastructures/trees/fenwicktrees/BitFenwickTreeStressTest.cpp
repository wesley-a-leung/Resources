#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr const int N = 1e7, U = 1e7, Q = 1e7;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % 2;
  FenwickTree1D<int> FT(A.begin(), A.end());
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
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr const int N = 1e7, U = 1e7, Q = 1e7;
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
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr const int N = 1e9, U = 1e7, Q = 1e7;
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
  cout << "  U: " << U << endl;
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
  cout << "Test Passed" << endl;
  return 0;
}
