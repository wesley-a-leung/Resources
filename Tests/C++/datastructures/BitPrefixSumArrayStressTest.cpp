#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/BitPrefixSumArray.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  constexpr const int N = 1e7, U = 1e7, Q = 1e7;
  vector<int> A(N + 1);
  for (int i = 0; i < U; i++) {
    int j = rng() % N, v = rng() % 2;
    A[j + 1] = v;
  }
  partial_sum(A.begin(), A.end(), A.begin());
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(A[r + 1] - A[l]);
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Prefix Sum) Passed" << endl;
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
  BitPrefixSumArray bpsa(N);
  for (int i = 0; i < U; i++) {
    int j = rng() % N, v = rng() % 2;
    bpsa.set(j, v);
  }
  bpsa.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(bpsa.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Bit Prefix Sum) Passed" << endl;
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
  BitPrefixSumArray bpsa(N);
  for (int i = 0; i < U; i++) {
    int j = rng() % N, v = rng() % 2;
    bpsa.set(j, v);
  }
  bpsa.build();
  vector<int> ans;
  ans.reserve(Q);
  for (int i = 0; i < Q; i++) {
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    ans.push_back(bpsa.query(l, r));
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Bit Prefix Sum) Passed" << endl;
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
