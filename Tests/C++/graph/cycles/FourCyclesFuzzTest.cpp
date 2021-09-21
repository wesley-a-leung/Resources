#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/cycles/FourCycles.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int V = rng() % 9, E = rng() % (V * (V - 1) / 2 + 1);
    vector<pair<int, int>> edges;
    set<pair<int, int>> S;
    vector<long long> X(V), Y(E);
    for (auto &&x : X) x = rng() % int(1e6) + 1;
    for (int i = 0; i < E; i++) {
      int a, b;
      do {
        a = rng() % V;
        b = rng() % V;
      } while (a == b || S.count(make_pair(a, b)) || S.count(make_pair(b, a)));
      S.emplace(a, b);
      edges.emplace_back(a, b);
      Y[i] = rng() % (int(1e6)) + 1;
    }
    long long cnt0 = 0;
    for (int i = 0; i < E; i++) {
      for (int j = 0; j < i; j++) {
        for (int k = 0; k < j; k++) {
          for (int m = 0; m < k; m++) {
            unordered_map<int, int> umap;
            umap[edges[i].first]++;
            umap[edges[i].second]++;
            umap[edges[j].first]++;
            umap[edges[j].second]++;
            umap[edges[k].first]++;
            umap[edges[k].second]++;
            umap[edges[m].first]++;
            umap[edges[m].second]++;
            int cntTwo = 0;
            for (auto &&p : umap) cntTwo += p.second == 2;
            cnt0 += cntTwo == 4;
          }
        }
      }
    }
    long long cnt1 = fourCycles(V, edges);
    assert(cnt0 == cnt1);
    checkSum = 31 * cnt0;
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
