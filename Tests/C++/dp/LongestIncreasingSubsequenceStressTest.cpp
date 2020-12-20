#include <bits/stdc++.h>
#include "../../../Content/C++/dp/LongestIncreasingSubsequence.h"
#include "../../../Content/C++/dp/LongestIncreasingSubsequenceBIT.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int N = 1e7;
  vector<int> A(N);
  for (auto &&a : A) a = rng() % int(6) + 1;
  const auto start_time = chrono::system_clock::now();
  vector<int> inds = longestIncreasingSubsequence(A.begin(), A.end());
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (lower_bound) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int i : inds) checkSum = 31 * checkSum + i;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int N = 1e7;
  vector<int> A(N);
  for (auto &&a : A) a = rng() % int(6) + 1;
  const auto start_time = chrono::system_clock::now();
  vector<int> inds = longestIncreasingSubsequenceBIT(A.begin(), A.end());
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (BIT) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int i : inds) checkSum = 31 * checkSum + i;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
