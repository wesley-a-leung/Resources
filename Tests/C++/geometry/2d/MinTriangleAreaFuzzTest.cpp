#include <bits/stdc++.h>
#include "../../../../Content/C++/geometry/2d/MinTriangleArea.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 2e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    vector<pt> P(N);
    for (auto &&p : P) p = pt(rng() % int(10), rng() % int(10));
    T minArea2 = numeric_limits<T>::max();
    for (int i = 0; i < N; i++) for (int j = 0; j < i; j++) for (int k = 0; k < j; k++)
      minArea2 = min(minArea2, abs(area2(P[i], P[j], P[k])));
    MinTriangleArea mta(P);
    assert(minArea2 == mta.minArea2);
    if (minArea2 != numeric_limits<T>::max()) assert(area2(mta.PA, mta.PB, mta.PC) == minArea2);
    checkSum = 31 * checkSum + minArea2;
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
