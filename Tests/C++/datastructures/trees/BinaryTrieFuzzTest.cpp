#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/trees/BinaryTrie.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 100 + 1;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % (1 << 30);
    BinaryTrie<int> trie(1 << 30);
    for (auto &&a : A) trie.insert(a);
    int Q = 100 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      int v = rng() % (1 << 30);
      if (t == 0) {
        int mn = INT_MAX;
        for (auto &&a : A) mn = min(mn, v ^ a);
        ans0.push_back(mn);
        ans1.push_back(trie.minXor(v));
      } else {
        int mx = INT_MIN;
        for (auto &&a : A) mx = max(mx, v ^ a);
        ans0.push_back(mx);
        ans1.push_back(trie.maxXor(v));
      }
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
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
