#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/PrefixSumArray2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 10 - rng() % 2;
    int M = 20 - rng() % 2;
    vector<vector<long long>> A(N, vector<long long>(M)), B(N, vector<long long>(M));
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) B[i][j] = A[i][j] = rng() % int(1e6) + 1;
    adjacent_difference_2d(B, N, M, B);
    int U = 100 - rng() % 5;
    for (int i = 0; i < U; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      long long v = rng() % int(1e6) + 1;
      add(B, N, M, u, d, l, r, v);
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) A[j][k] += v;
    }
    partial_sum_2d(B, N, M, B);
    partial_sum_2d(B, N, M, B);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      ans0.push_back(rsq(B, u, d, l, r));
      long long sm = 0;
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) sm += A[j][k];
      ans1.push_back(sm);
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum ^= a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (std::vector) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 10 - rng() % 2;
    int M = 20 - rng() % 2;
    array<array<long long, 20>, 10> A, B;
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) B[i][j] = A[i][j] = rng() % int(1e6) + 1;
    adjacent_difference_2d(B, N, M, B);
    int U = 100 - rng() % 5;
    for (int i = 0; i < U; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      long long v = rng() % int(1e6) + 1;
      add(B, N, M, u, d, l, r, v);
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) A[j][k] += v;
    }
    partial_sum_2d(B, N, M, B);
    partial_sum_2d(B, N, M, B);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      ans0.push_back(rsq(B, u, d, l, r));
      long long sm = 0;
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) sm += A[j][k];
      ans1.push_back(sm);
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum ^= a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (std::array) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 10 - rng() % 2;
    int M = 20 - rng() % 2;
    long long A[10][20], B[10][20];
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) B[i][j] = A[i][j] = rng() % int(1e6) + 1;
    adjacent_difference_2d(B, N, M, B);
    int U = 100 - rng() % 5;
    for (int i = 0; i < U; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      long long v = rng() % int(1e6) + 1;
      add(B, N, M, u, d, l, r, v);
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) A[j][k] += v;
    }
    partial_sum_2d(B, N, M, B);
    partial_sum_2d(B, N, M, B);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r); 
      ans0.push_back(rsq(B, u, d, l, r));
      long long sm = 0;
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) sm += A[j][k];
      ans1.push_back(sm);
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum ^= a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (C style array) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  return 0;
}
