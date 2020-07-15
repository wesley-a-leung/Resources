#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/unionfind/UnionFind.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  UnionFind uf(N);
  int Q = 1e7;
  vector<int> ans;
  for (int i = 0; i < Q; i++) {
    if (rng() % 2 == 0) ans.push_back(uf.getSize(rng() % N));
    else {
      int v = rng() % N, w = rng() % N;
      uf.join(v, w);
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
