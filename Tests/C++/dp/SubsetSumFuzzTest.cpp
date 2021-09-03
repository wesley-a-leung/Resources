#include <bits/stdc++.h>
#include "../../../Content/C++/dp/SubsetSum.h"
using namespace std;

const int M = 100;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(10) + 1;
    bitset<M + 1> possible = subsetSum<M>(A);
    int tot = accumulate(A.begin(), A.end(), 0);
    vector<int> cnt(tot + 1, 0), dp = subsetSumCount<int>(A, tot);
    for (int mask = 0; mask < (1 << N); mask++) {
      int sm = 0;
      for (int i = 0; i < N; i++) if ((mask >> i) & 1) sm += A[i];
      cnt[sm]++;
    }
    assert(cnt == dp);
    for (int i = 0; i < int(cnt.size()); i++) assert(possible[i] == bool(cnt[i]));
    for (int i : dp) checkSum = 31 * checkSum + i;
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
