#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/PrefixSumArray2D.h"
using namespace std;

long long A[4000][6000];
array<array<long long, 6000>, 4000> B;
vector<vector<long long>> C(4000, vector<long long>(6000));

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr int N = 4000;
  constexpr int M = 6000;
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) A[i][j] = rng() % int(1e5) + 1;
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
  constexpr int N = 500;
  constexpr int M = 1000;
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) A[i][j] = rng() % int(1e5) + 1;
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
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) B[i][j] = rng() % int(1e5) + 1;
  adjacent_difference_2d(B, N, M, B);
  int U = 1;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(B, N, M, u, d, l, r, v);
  }
  partial_sum_2d(B, N, M, B);
  partial_sum_2d(B, N, M, B);
  int Q = 1;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(B, u, d, l, r));
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
  constexpr int N = 500;
  constexpr int M = 1000;
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) B[i][j] = rng() % int(1e5) + 1;
  adjacent_difference_2d(B, N, M, B);
  int U = 5e6;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(B, N, M, u, d, l, r, v);
  }
  partial_sum_2d(B, N, M, B);
  partial_sum_2d(B, N, M, B);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(B, u, d, l, r));
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
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) C[i][j] = rng() % int(1e5) + 1;
  adjacent_difference_2d(C, N, M, C);
  int U = 1;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(C, N, M, u, d, l, r, v);
  }
  partial_sum_2d(C, N, M, C);
  partial_sum_2d(C, N, M, C);
  int Q = 1;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(C, u, d, l, r));
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
  constexpr int N = 500;
  constexpr int M = 1000;
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) C[i][j] = rng() % int(1e5) + 1;
  adjacent_difference_2d(C, N, M, C);
  int U = 5e6;
  for (int i = 0; i < U; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    long long v = rng() % int(1e5) + 1;
    add(C, N, M, u, d, l, r, v);
  }
  partial_sum_2d(C, N, M, C);
  partial_sum_2d(C, N, M, C);
  int Q = 5e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
    if (u > d) swap(u, d);
    if (l > r) swap(l, r); 
    ans.push_back(rsq(C, u, d, l, r));
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
  cout << "Test Passed" << endl;
  return 0;
}
