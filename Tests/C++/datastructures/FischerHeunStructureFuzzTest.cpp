#include <bits/stdc++.h>
#include "../../../Content/C++/datastructures/FischerHeunStructure.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(100) + 1;
    FischerHeunStructure<int, greater<int>> ST(A);
    int Q = N == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1, ansA0, ansA1;
    for (int i = 0; i < Q; i++) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      int mnInd = l;
      for (int j = l + 1; j <= r; j++) if (A[mnInd] > A[j]) mnInd = j;
      ans0.push_back(mnInd);
      ansA0.push_back(A[ans0.back()]);
      ans1.push_back(ST.queryInd(l, r));
      ansA1.push_back(A[ans1.back()]);
    }
    assert(ans0 == ans1);
    assert(ansA0 == ansA1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 101;
    vector<int> A(N);
    for (auto &&a : A) a = rng() % int(100) + 1;
    FischerHeunStructure<int, greater_equal<int>> ST(A);
    int Q = N == 0 ? 0 : 100 - rng() % 5;
    vector<int> ans0, ans1, ansA0, ansA1;
    for (int i = 0; i < Q; i++) {
      int l = rng() % N, r = rng() % N;
      if (l > r) swap(l, r);
      int mnInd = l;
      for (int j = l + 1; j <= r; j++) if (A[mnInd] >= A[j]) mnInd = j;
      ans0.push_back(mnInd);
      ansA0.push_back(A[ans0.back()]);
      ans1.push_back(ST.queryInd(l, r));
      ansA1.push_back(A[ans1.back()]);
    }
    assert(ans0 == ans1);
    assert(ansA0 == ansA1);
    for (auto &&a : ans0) checkSum = 31 * checkSum + a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
