#include <bits/stdc++.h>
#include "../../../Content/C++/string/LongestCommonPrefix.h"
#include "../../../Content/C++/string/SuffixArray.h"
#include "../../../Content/C++/string/SAISSuffixArray.h"
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
    for (auto &&a : A) a = rng() % int(10) + 1e5;
    LongestCommonPrefix<SuffixArray<int>> LCP1(A.begin(), A.end());
    LongestCommonPrefix<SAISSuffixArray<int>> LCP2(A.begin(), A.end());
    vector<pair<basic_string<int>, int>> suffixes;
    for (int i = 0; i < N; i++) suffixes.emplace_back(A.substr(i), i);
    sort(suffixes.begin(), suffixes.end());
    vector<int> inv(N, -1);
    for (int i = 0; i < N; i++) {
      assert(suffixes[i].second == LCP1.SA.ind[i]);
      assert(suffixes[i].second == LCP2.SA.ind[i]);
      inv[suffixes[i].second] = i;
      checkSum = 31 * checkSum + LCP1.SA.ind[i];
    }
    for (int i = 0; i < N; i++) {
      assert(inv[i] == LCP1.SA.rnk[i]);
      assert(inv[i] == LCP2.SA.rnk[i]);
      checkSum = 31 * checkSum + LCP1.SA.rnk[i];
    }
    for (int i = 0; i < N - 1; i++) {
      assert(lcp(suffixes[i].first, suffixes[i + 1].first) == LCP1.SA.LCP[i]);
      assert(lcp(suffixes[i].first, suffixes[i + 1].first) == LCP2.SA.LCP[i]);
      checkSum = 31 * checkSum + LCP1.SA.LCP[i];
    }
    if (N > 0) {
      assert(LCP1.SA.LCP[N - 1] == 0);
      assert(LCP2.SA.LCP[N - 1] == 0);
    }
    for (int i = 0; i < N; i++) for (int j = i; j < N; j++) {
      assert(lcp(A.substr(i), A.substr(j)) == LCP1.lcp(i, j));
      assert(lcp(A.substr(i), A.substr(j)) == LCP2.lcp(i, j));
      checkSum = 31 * checkSum + LCP1.lcp(i, j);
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
