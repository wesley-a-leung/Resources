#include <bits/stdc++.h>
#include "../../../../Content/C++/geometry/2d/FarthestPair.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 7;
    vector<pt> P(N);
    for (auto &&p : P) p = pt(int(rng() % int(10) - int(5)), int(rng() % int(10) - int(5)));
    FarthestPair fp(P);
    T mx = 0;
    for (int i = 0; i < N; i++) for (int j = 0; j < i; j++) mx = max(mx, distSq(P[i], P[j]));
    assert(eq(mx, fp.bestDistSq));
    checkSum = 31 * checkSum + fp.bestDistSq;
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
