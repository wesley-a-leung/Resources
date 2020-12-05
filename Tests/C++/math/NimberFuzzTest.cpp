#include <bits/stdc++.h>
#include "../../../Content/C++/math/Nimber.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e3;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    uint64_t a = 0;
    while (a == 0) a = rng();
    uint64_t inv = mulInvNim(a);
    assert(mulNim(a, inv) == 1);
    checkSum = 31 * checkSum + inv;
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
