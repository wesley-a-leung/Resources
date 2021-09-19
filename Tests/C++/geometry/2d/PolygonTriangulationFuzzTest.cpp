#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/unionfind/UnionFind.h"
#include "../../../../Content/C++/geometry/2d/Point.h"
#include "../../../../Content/C++/geometry/2d/Line.h"
#include "../../../../Content/C++/geometry/2d/Polygon.h"
#include "../../../../Content/C++/geometry/2d/PolygonTriangulation.h"
#include "../../../../Content/C++/graph/representations/AdjacencyListGraph.h"
using namespace std;

vector<pt> generatePolygon(int N, mt19937_64 &rng, bool grid) {
  vector<pt> P(N);
  assert(N >= 3);
  set<pt> seen;
  if (grid) {
    uniform_int_distribution<int> dis(0, 10);
    while (true) {
      for (auto &&p : P) {
        do {
          p.x = dis(rng);
          p.y = dis(rng);
        } while (seen.count(p));
        seen.insert(p);
      }
      sort(P.begin(), P.end());
      T sm = 0;
      for (int i = 0; i < N - 1; i++) sm += dist(P[i], P[i + 1]);
      if (!eq(sm, dist(P[0], P[N - 1]))) break;
    }
  } else {
    uniform_real_distribution<T> dis(0, 10);
    while (true) {
      for (auto &&p : P) {
        do {
          p.x = dis(rng);
          p.y = dis(rng);
        } while (seen.count(p));
        seen.insert(p);
      }
      sort(P.begin(), P.end());
      T sm = 0;
      for (int i = 0; i < N - 1; i++) sm += dist(P[i], P[i + 1]);
      if (!eq(sm, dist(P[0], P[N - 1]))) break;
    }
  }
  vector<pair<int, int>> edges;
  for (int i = 0; i < N; i++) for (int j = 0; j < i; j++) edges.emplace_back(i, j);
  sort(edges.begin(), edges.end(), [&] (const pair<int, int> &a, const pair<int, int> &b) {
    return dist(P[a.first], P[a.second]) < dist(P[b.first], P[b.second]);
  });
  UnionFind uf(N);
  AdjacencyListGraph G(N);
  for (auto &&e : edges) if (uf.join(e.first, e.second)) G.addBiEdge(e.first, e.second);
  for (int v = 0; v < N; v++) shuffle(G[v].begin(), G[v].end(), rng);
  vector<pt> ret;
  function<void(int, int)> dfs = [&] (int v, int prev) {
    ret.push_back(P[v]);
    for (int w : G[v]) if (w != prev) dfs(w, v);
  };
  dfs(0, -1);
  bool done = false;
  while (!done) {
    done = true;
    for (int i = 0; i < N; i++) for (int j = i + 2; j < N; j++) if (mod(j + 1, N) != i) {
      if (segSegIntersects(ret[i], ret[mod(i + 1, N)], ret[j], ret[mod(j + 1, N)])) {
        reverse(ret.begin() + i + 1, ret.begin() + j + 1);
        done = false;
      }
    }
  }
  return ret;
}

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  long long checkSum = 0;
  uniform_int_distribution<int> dis(3, 30);
  for (int ti = 0; ti < TESTCASES; ti++) {
    vector<pt> poly = generatePolygon(dis(rng), rng, false);
    if (isCcwPolygon(poly) == -1) reverse(poly.begin(), poly.end());
    T a2 = getArea2(poly), sm = 0;
    auto tris = polygonTriangulation(poly);
    assert(int(tris.size()) == int(poly.size()) - 2);
    for (auto &&t : tris) sm += area2(t[0], t[1], t[2]);
    assert(eq(a2, sm));
    checkSum += a2;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Random) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  long long checkSum = 0;
  uniform_int_distribution<int> dis(3, 30);
  for (int ti = 0; ti < TESTCASES; ti++) {
    vector<pt> poly = generatePolygon(dis(rng), rng, true);
    if (isCcwPolygon(poly) == -1) reverse(poly.begin(), poly.end());
    T a2 = getArea2(poly), sm = 0;
    auto tris = polygonTriangulation(poly);
    assert(int(tris.size()) == int(poly.size()) - 2);
    for (auto &&t : tris) sm += area2(t[0], t[1], t[2]);
    assert(eq(a2, sm));
    checkSum += a2;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Grid) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  long long checkSum = 0;
  vector<pt> poly = {pt(0, 0), pt(1, 0), pt(2, 0), pt(2, -1), pt(2, -2), pt(3, -2), pt(4, -2), pt(4, 0), pt(5, 0), pt(6, 0), pt(6, 1), pt(5, 1), pt(4, 1), pt(4, 3), pt(3, 3), pt(2, 3), pt(2, 2), pt(2, 1), pt(1, 1), pt(0, 1)};
  assert(int(poly.size()) == 20);
  T a2 = getArea2(poly), sm = 0;
  auto tris = polygonTriangulation(poly);
  assert(int(tris.size()) == int(poly.size()) - 2);
  for (auto &&t : tris) sm += area2(t[0], t[1], t[2]);
  assert(eq(a2, sm));
  checkSum += a2;
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Special) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  long long checkSum = 0;
  vector<pt> poly = {pt(6, 5), pt(0, 5), pt(0, 4), pt(1, 4), pt(1, 3), pt(0, 3), pt(0, 2), pt(1, 2), pt(1, 1), pt(0, 1), pt(0, 0), pt(6, 0), pt(6, 1), pt(5, 1), pt(5, 2), pt(6, 2), pt(6, 3), pt(5, 3), pt(5, 4), pt(6, 4)};
  assert(int(poly.size()) == 20);
  T a2 = getArea2(poly), sm = 0;
  auto tris = polygonTriangulation(poly);
  assert(int(tris.size()) == int(poly.size()) - 2);
  for (auto &&t : tris) sm += area2(t[0], t[1], t[2]);
  assert(eq(a2, sm));
  checkSum += a2;
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Special) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  cout << "Test Passed" << endl;
  return 0;
}
