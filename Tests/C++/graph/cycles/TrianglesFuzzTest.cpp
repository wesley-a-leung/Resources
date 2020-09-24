#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/components/Triangles.h"
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
    long long cnt0 = 0, vertSum0 = 0, edgeSum0 = 0, cnt1 = 0, vertSum1 = 0, edgeSum1 = 0;
    for (int i = 0; i < E; i++) {
      for (int j = 0; j < i; j++) {
        for (int k = 0; k < j; k++) {
          unordered_set<int> uset;
          uset.insert(edges[i].first);
          uset.insert(edges[i].second);
          uset.insert(edges[j].first);
          uset.insert(edges[j].second);
          uset.insert(edges[k].first);
          uset.insert(edges[k].second);
          if (int(uset.size()) == 3) {
            cnt0++;
            long long prod = 1;
            for (int a : uset) prod *= X[a];
            vertSum0 += prod;
            edgeSum0 += Y[i] * Y[j] * Y[k];
          }
        }
      }
    }
    triangles(V, edges, [&] (int a, int b, int c, int i, int j, int k) {
      cnt1++;
      vertSum1 += X[a] * X[b] * X[c];
      edgeSum1 += Y[i] * Y[j] * Y[k];
    });
    assert(cnt0 == cnt1);
    assert(vertSum0 == vertSum1);
    assert(edgeSum0 == edgeSum1);
    checkSum = 31 * checkSum + vertSum0;
    checkSum = 31 * checkSum + edgeSum0;
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
