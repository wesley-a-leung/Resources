#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/SparseTable2D.h"
#include "../../../Content/C++/datastructures/FischerHeunStructure2D.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  struct Min { int operator () (int a, int b) { return min(a, b); } };
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    int M = rng() % 21;
    vector<vector<int>> A(N, vector<int>(M));
    for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
    SparseTable2D<int, Min> ST(A);
    int Q = N == 0 || M == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r);
      int mn = A[u][l];
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) mn = min(mn, A[j][k]);
      ans0.push_back(mn);
      ans1.push_back(ST.query(u, d, l, r));
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Sparse Table) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11;
    int M = rng() % 21;
    vector<vector<int>> A(N, vector<int>(M));
    for (auto &&ai : A) for (auto &&aij : ai) aij = rng() % int(1e9) + 1;
    FischerHeunStructure2D<int, greater<int>> FHS(A);
    int Q = N == 0 || M == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1;
    for (int i = 0; i < Q; i++) {
      int u = rng() % N, d = rng() % N, l = rng() % M, r = rng() % M;
      if (u > d) swap(u, d);
      if (l > r) swap(l, r);
      int mn = A[u][l];
      for (int j = u; j <= d; j++) for (int k = l; k <= r; k++) mn = min(mn, A[j][k]);
      ans0.push_back(mn);
      ans1.push_back(FHS.query(u, d, l, r));
    }
    assert(ans0 == ans1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Fischer Heun Structure) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
