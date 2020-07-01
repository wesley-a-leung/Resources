#include <bits/stdc++.h>
#include "../../../Content/C++/string/SuffixArray.h"
using namespace std;

int lcp(const basic_string<int> &A, const basic_string<int> &B) {
  int i = 0;
  for (; i < min(int(A.size()), int(B.size())); i++) if (A[i] != B[i]) break;
  return i;
}

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    basic_string<int> A(N, 0);
    for (auto &&a : A) a = rng() % int(10) + 1e8;
    LongestCommonPrefix<int> LCP(A.begin(), A.end());
    vector<pair<basic_string<int>, int>> suffixes;
    for (int i = 0; i < N; i++) suffixes.emplace_back(A.substr(i), i);
    sort(suffixes.begin(), suffixes.end());
    vector<int> inv(N, -1);
    for (int i = 0; i < N; i++) {
      assert(suffixes[i].second == LCP.SA.ind[i]);
      inv[suffixes[i].second] = i;
      checkSum = (31 * checkSum + LCP.SA.ind[i]) % (long long)(1e9 + 7);
    }
    for (int i = 0; i < N; i++) {
      assert(inv[i] == LCP.SA.rnk[i]);
      checkSum = (31 * checkSum + LCP.SA.rnk[i]) % (long long)(1e9 + 7);
    }
    for (int i = 0; i < N - 1; i++) {
      assert(lcp(suffixes[i].first, suffixes[i + 1].first) == LCP.SA.LCP[i]);
      checkSum = (31 * checkSum + LCP.SA.LCP[i]) % (long long)(1e9 + 7);
    }
    if (N > 0) assert(LCP.SA.LCP[N - 1] == 0);
    for (int i = 0; i < N; i++) for (int j = i; j < N; j++) {
      assert(lcp(A.substr(i), A.substr(j)) == LCP.lcp(i, j));
      checkSum = (31 * checkSum + LCP.lcp(i, j)) % (long long)(1e9 + 7);
    }
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
