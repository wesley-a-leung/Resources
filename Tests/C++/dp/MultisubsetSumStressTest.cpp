#include <bits/stdc++.h>
#include "../../../Content/C++/dp/MultisubsetSum.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int N = 3e4, M = 3e4;
  vector<int> A(N);
  for (auto &&a : A) a = rng() % int(1e3) + 1;
  const auto start_time = chrono::system_clock::now();
  auto dp = multisubsetSum<bool>(A, M);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (bool) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&i : dp) checkSum = 31 * checkSum + i;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int N = 3e4, M = 3e4;
  vector<int> A(N);
  for (auto &&a : A) a = rng() % int(1e3) + 1;
  const auto start_time = chrono::system_clock::now();
  auto dp = multisubsetSum<char>(A, M);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (char) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&i : dp) checkSum = 31 * checkSum + i;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  mt19937_64 rng(0);
  int N = 3e4, M = 3e4;
  vector<int> A(N);
  for (auto &&a : A) a = rng() % int(1e3) + 1;
  const auto start_time = chrono::system_clock::now();
  auto dp = multisubsetSum<int>(A, M);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (int) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&i : dp) checkSum = 31 * checkSum + i;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  cout << "Test Passed" << endl;
  return 0;
}
