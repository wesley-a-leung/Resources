#include <bits/stdc++.h>
#include "../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTreeAffine.h"
#include "../../../../Content/C++/datastructures/trees/segmenttrees/SegmentTreeAffine.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  FenwickTreeAffine<long long> FT(N);
  int Q = 2e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    if (t == 0) {
      long long m = rng() % int(1e2) + 1;
      long long b = rng() % int(1e2) + 1;
      FT.update(l, r, m, b);
    } else {
      ans.push_back(FT.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Fenwick Tree) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  int N = 2e6;
  SegmentTreeAffine<long long> ST(N);
  int Q = 2e6;
  vector<long long> ans;
  for (int i = 0; i < Q; i++) {
    int t = rng() % 2;
    int l = rng() % N, r = rng() % N;
    if (l > r) swap(l, r);
    if (t == 0) {
      long long m = rng() % int(1e2) + 1;
      long long b = rng() % int(1e2) + 1;
      ST.update(l, r, m, b);
    } else {
      ans.push_back(ST.query(l, r));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Segment Tree) Passed" << endl;
  cout << "  N: " << N << endl;
  cout << "  Q: " << Q << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (auto &&a : ans) checkSum = 31 * checkSum + a;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
