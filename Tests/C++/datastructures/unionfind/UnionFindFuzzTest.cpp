#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/unionfind/UnionFind.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    UnionFind uf(N);
    vector<vector<int>> sets(N);
    vector<int> par(N);
    for (int i = 0; i < N; i++) sets[par[i] = i].push_back(i);
    int Q = N == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      if (rng() % 2 == 0) {
        int v = rng() % N;
        ans0.push_back(int(sets[par[v]].size()));
        ans1.push_back(uf.getSize(v));
      } else {
        int v = rng() % N, w = rng() % N;
        if (sets[par[v]].size() < sets[par[w]].size()) swap(v, w);
        if (par[w] != par[v]) for (int x : sets[par[w]]) sets[par[x] = par[v]].push_back(x);
        uf.join(v, w);
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
