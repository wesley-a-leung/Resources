#include <bits/stdc++.h>
#include "../../../Content/C++/string/SuffixArray.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  SuffixArray<int> SA(A.begin(), A.end());
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Random) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.rnk) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.LCP) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> B(500);
  for (auto &&bi : B) bi = rng() % int(1e9) + 1;
  vector<int> A(N);
  for (int i = 0; i < N; i++) A[i] = B[i % 500];
  SuffixArray<int> SA(A.begin(), A.end());
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Period 500) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.rnk) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.LCP) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % int(1e9) + 1);
  SuffixArray<int> SA(A.begin(), A.end());
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Period 1) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.rnk) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  for (auto &&a : SA.LCP) checkSum = (31 * checkSum + a) % (long long)(1e9 + 7);
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % int(1e9) + 1);
  LongestCommonPrefix<int> LCP(A.begin(), A.end());
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) ans.push_back(LCP.lcp(rng() % N, rng() % N));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (lcp queries) Passed" << endl;
  cout << "  N: " << N << endl;
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
  cout << "Test Passed" << endl;
  return 0;
}
