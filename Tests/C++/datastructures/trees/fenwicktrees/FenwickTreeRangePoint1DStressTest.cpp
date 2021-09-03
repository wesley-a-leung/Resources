#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint.h"
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeRangePoint1D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTreeRangePoint1D<long long> FT(A);
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      long long v = rng() % int(1e9) + 1;
      FT.update(l, r, v);
    } else {
      int j = rng() % N;
      ans.push_back(FT.get(j));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (1D vector Constructor) Passed" << endl;
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
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTreeRangePoint1D<long long> FT(N);
  for (int i = 0; i < N; i++) FT.update(i, i, A[i]);
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      long long v = rng() % int(1e9) + 1;
      FT.update(l, r, v);
    } else {
      int j = rng() % N;
      ans.push_back(FT.get(j));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (1D default value constructor) Passed" << endl;
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
  int N = 1e7;
  vector<long long> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  FenwickTreeRangePoint<1, long long> FT(N);
  for (int i = 0; i < N; i++) FT.update(A[i], i, i);
  int Q = 1e7;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    if (t == 0) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      long long v = rng() % int(1e9) + 1;
      FT.update(v, l, r);
    } else {
      int j = rng() % N;
      ans.push_back(FT.get(j));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (ND) Passed" << endl;
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
  cout << "Test Passed" << endl;
  return 0;
}
