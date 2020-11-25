#include <bits/stdc++.h>
#include "../../../Content/C++/math/GCD.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int m = rng() % 100 + 1, a = rng() % m, c = rng() % m;
    pair<int, int> x = solveCongruence(a, c, m);
    for (int i = 0; i < 1000; i++) assert((a * i % m == c) == (i % x.second == x.first));
    checkSum = 31 * checkSum + x.first;
    checkSum = 31 * checkSum + x.second;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
