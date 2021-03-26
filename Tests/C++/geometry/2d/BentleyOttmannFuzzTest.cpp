#include <bits/stdc++.h>
#include "../../../../Content/C++/geometry/2d/BentleyOttmann.h"
#include "../../../../Content/C++/geometry/2d/Line.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    vector<pair<pt, pt>> segs;
    for (int i = 0; i < 10; i++) {
      T a, b, c;
      do {
        a = int(rng() % 21) - 10;
        b = int(rng() % 21) - 10;
        c = int(rng() % 401) - 201;
      } while (a == 0 && b == 0);
      Line l(a, b, c);
      while (rng() % 2) {
        pt p = l.proj(pt(int(rng() % 21) - 10, int(rng() % 21) - 10));
        pt q = l.proj(pt(int(rng() % 21) - 10, int(rng() % 21) - 10));
        segs.emplace_back(p, q);
      }
    }
    set<pair<int, int>> ans0, ans1;
    bentleyOttmann(segs, [&] (int i, int j) {
      if (i > j) swap(i, j);
      ans0.emplace(i, j);
    });
    for (int i = 0; i < int(segs.size()); i++) for (int j = i + 1; j < int(segs.size()); j++) {
      if (segSegIntersects(segs[i].first, segs[i].second, segs[j].first, segs[j].second)) ans1.emplace(i, j);
    }
    assert(ans0 == ans1);
    for (auto &&p : ans0) {
      checkSum = 31 * checkSum + p.first;
      checkSum = 31 * checkSum + p.second;
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
