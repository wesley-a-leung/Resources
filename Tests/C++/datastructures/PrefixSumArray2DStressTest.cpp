#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/PrefixSumArray2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  long long A[N][M];
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 1;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 1;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (C style array) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  long long A[N][M];
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 5e6;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (C style array) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  array<array<long long, M>, N> A;
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 1;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 1;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (std::array) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  array<array<long long, M>, N> A;
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 5e6;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (std::array) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  vector<vector<long long>> A(N, vector<long long>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 1;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 1;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (std::vector) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  vector<vector<long long>> A(N, vector<long long>(M));
  for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e5) + 1;
  adjacent_difference_2d(A, N, M, A);
  int U = 5e6;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(A, N, M, u, d, l, r, v);
  }
  partial_sum_2d(A, N, M, A);
  partial_sum_2d(A, N, M, A);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(A, u, d, l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (std::vector) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  M: " << M << endl;
  cout << "  U: " << U << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum ^= a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  return 0;
}
