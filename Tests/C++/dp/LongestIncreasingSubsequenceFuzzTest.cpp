#include <bits/stdc++.h>
#include "../../../Content/C++/dp/LongestIncreasingSubsequence.h"
#include "../../../Content/C++/dp/LongestIncreasingSubsequenceBIT.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 7;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(6) + 1;
    vector<int> maxInds;
    for (int mask = 1; mask < (1 << N); mask++) {
      vector<int> inds;
      for (int i = 0; i < N; i++) if ((mask >> i) & 1) inds.push_back(i);
      bool good = true;
      for (int i = 0; i < int(inds.size()) - 1 && good; i++) good &= A[inds[i]] < A[inds[i + 1]];
      if (good && make_pair(int(inds.size()), inds) > make_pair(int(maxInds.size()), maxInds)) maxInds = inds;
    }
    vector<int> inds = longestIncreasingSubsequence(A.begin(), A.end());
    assert(inds == maxInds);
    for (int i : inds) checkSum = 31 * checkSum + i;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (lower_bound) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 7;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(6) + 1;
    vector<int> minB;
    for (int mask = 1; mask < (1 << N); mask++) {
      vector<int> B;
      for (int i = 0; i < N; i++) if ((mask >> i) & 1) B.push_back(A[i]);
      bool good = true;
      for (int i = 0; i < int(B.size()) - 1 && good; i++) good &= B[i] < B[i + 1];
      if (good && make_pair(-int(B.size()), B) < make_pair(-int(minB.size()), minB)) minB = B;
    }
    vector<int> inds = longestIncreasingSubsequenceBIT(A.begin(), A.end()), B;
    for (int i : inds) B.push_back(A[i]);
    assert(B == minB);
    for (int i : inds) checkSum = 31 * checkSum + i;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (BIT) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
