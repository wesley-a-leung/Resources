#include <bits/stdc++.h>
#include "../../../Content/C++/string/LongestCommonPrefix.h"
#include "../../../Content/C++/string/SuffixArray.h"
#include "../../../Content/C++/string/SAISSuffixArray.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  const auto start_time = chrono::system_clock::now();
  SuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Random, large alphabet, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % int(1e9) + 1;
  const auto start_time = chrono::system_clock::now();
  vector<int> tmp = A;
  sort(tmp.begin(), tmp.end());
  tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
  for (auto &&ai : A) ai = lower_bound(tmp.begin(), tmp.end(), ai) - tmp.begin();
  SAISSuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Random, large alphabet, SAISSuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % N + 1;
  const auto start_time = chrono::system_clock::now();
  SuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Random, medium alphabet, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % N + 1;
  const auto start_time = chrono::system_clock::now();
  SAISSuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Random, medium alphabet, SAISSuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % 26 + 1;
  const auto start_time = chrono::system_clock::now();
  SuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Random, small alphabet, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N);
  for (auto &&ai : A) ai = rng() % 26 + 1;
  const auto start_time = chrono::system_clock::now();
  SAISSuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Random, small alphabet, SAISSuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test7() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> B(500);
  for (auto &&bi : B) bi = rng() % N + 1;
  vector<int> A(N);
  for (int i = 0; i < N; i++) A[i] = B[i % 500];
  const auto start_time = chrono::system_clock::now();
  SuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 7 (Period 500, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test8() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> B(500);
  for (auto &&bi : B) bi = rng() % N + 1;
  vector<int> A(N);
  for (int i = 0; i < N; i++) A[i] = B[i % 500];
  const auto start_time = chrono::system_clock::now();
  SAISSuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 8 (Period 500, SAISSuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test9() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % N + 1);
  const auto start_time = chrono::system_clock::now();
  SuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 9 (Period 1, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test10() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % N + 1);
  const auto start_time = chrono::system_clock::now();
  SAISSuffixArray<int> SA(A);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 10 (Period 1, SAISSuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : SA.ind) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.rnk) checkSum = 31 * checkSum + a;
  for (auto &&a : SA.LCP) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test11() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % N + 1);
  const auto start_time = chrono::system_clock::now();
  LongestCommonPrefix<SuffixArray<int>> LCP(A);
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) ans.push_back(LCP.lcp(rng() % N, rng() % N));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 11 (lcp queries, SuffixArray) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test12() {
  mt19937_64 rng(0);
  int N = 2e6;
  vector<int> A(N, rng() % N + 1);
  const auto start_time = chrono::system_clock::now();
  LongestCommonPrefix<SAISSuffixArray<int>> LCP(A);
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) ans.push_back(LCP.lcp(rng() % N, rng() % N));
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 12 (lcp queries, SAISSuffixArray) Passed" << endl;
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
  test9();
  test10();
  test11();
  test12();
  cout << "Test Passed" << endl;
  return 0;
}
