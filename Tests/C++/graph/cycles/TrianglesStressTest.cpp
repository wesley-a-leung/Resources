#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/cycles/Triangles.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 2e5, E = 1e6;
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
  const auto start_time = chrono::system_clock::now();
  long long vertSum = 0, edgeSum = 0, cnt = 0;
  triangles(V, edges, [&] (int a, int b, int c, int i, int j, int k) {
    cnt++;
    vertSum += X[a] * X[b] * X[c];
    edgeSum += Y[i] * Y[j] * Y[k];
  });
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Random) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = cnt;
  checkSum = 31 * checkSum + vertSum;
  checkSum = 31 * checkSum + edgeSum;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int V = 2e5, E = 1e6;
  vector<pair<int, int>> edges;
  set<pair<int, int>> S;
  vector<long long> X(V), Y(E);
  for (auto &&x : X) x = rng() % int(1e6) + 1;
  int sqrtE = sqrt(E);
  for (int i = 0; i < E; i++) {
    int a, b;
    do {
      if (i < sqrtE * (sqrtE - 1) / 2) {
        a = rng() % sqrtE;
        b = rng() % sqrtE;
      } else {
        a = rng() % V;
        b = rng() % V;
      }
    } while (a == b || S.count(make_pair(a, b)) || S.count(make_pair(b, a)));
    S.emplace(a, b);
    edges.emplace_back(a, b);
    Y[i] = rng() % (int(1e6)) + 1;
  }
  const auto start_time = chrono::system_clock::now();
  long long vertSum = 0, edgeSum = 0, cnt = 0;
  triangles(V, edges, [&] (int a, int b, int c, int i, int j, int k) {
    cnt++;
    vertSum += X[a] * X[b] * X[c];
    edgeSum += Y[i] * Y[j] * Y[k];
  });
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Small Dense Cluster) Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = cnt;
  checkSum = 31 * checkSum + vertSum;
  checkSum = 31 * checkSum + edgeSum;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
