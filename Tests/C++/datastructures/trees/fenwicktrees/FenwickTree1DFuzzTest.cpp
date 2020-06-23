#include <bits/stdc++.h>
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTree.h"
#include "../../../../../Content/C++/datastructures/trees/fenwicktrees/FenwickTree1D.h"
#include "../../../../../Content/C++/search/BinarySearch.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = 100 - rng() % 5;
    vector<long long> A(N);
    for (auto &&ai : A) ai = rng() % int(1e9) + 1;
    FenwickTree1D<long long> FT1(N);
    FenwickTree1D<long long> FT2(A.begin(), A.end());
    FenwickTree<1, long long> FT3(N);
    for (int i = 0; i < N; i++) {
      FT1.update(i, A[i]);
      FT3.update(i, A[i]);
    }
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1, ans2, ans3;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 2;
      if (t == 0) {
        int i = rng() % N;
        long long v = rng() % int(1e9) + 1;
        A[i] += v;
        FT1.update(i, v);
        FT2.update(i, v);
        FT3.update(i, v);
      } else {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        long long sm = 0;
        for (int j = l; j <= r; j++) sm += A[j];
        ans0.push_back(sm);
        ans1.push_back(FT1.query(l, r));
        ans2.push_back(FT2.query(l, r));
        ans3.push_back(FT3.query(l, r));
      }
    }
    vector<long long> A1 = FT1.values(), A2 = FT2.values();
    assert(ans0 == ans1);
    assert(ans0 == ans2);
    assert(ans0 == ans3);
    assert(A == A1);
    assert(A == A2);
    for (auto &&a : ans0) checkSum ^= a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (point update, range query) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int maxVal = pow(10, rng() % 10);
    int N = 100 - rng() % 5;
    vector<long long> A(N);
    for (auto &&ai : A) ai = rng() % maxVal + 1;
    FenwickTree1D<long long> FT1(N);
    FenwickTree1D<long long> FT2(A.begin(), A.end());
    for (int i = 0; i < N; i++) FT1.update(i, A[i]);
    int Q = 100 - rng() % 5;
    vector<long long> ans0, ans1, ans2, ans3;
    for (int i = 0; i < Q; i++) {
      int t = rng() % 4;
      if (t == 0) {
        int i = rng() % N;
        long long v = rng() % maxVal + 1;
        A[i] += v;
        FT1.update(i, v);
        FT2.update(i, v);
      } else if (t == 1) {
        int l = rng() % N, r = rng() % N;
        if (l > r) swap(l, r);
        long long sm = 0;
        for (int j = l; j <= r; j++) sm += A[j];
        ans0.push_back(sm);
        ans1.push_back(FT1.query(l, r));
        ans2.push_back(FT2.query(l, r));
        ans3.push_back(ans2.back());
      } else if (t == 2) {
        long long TOT = FT1.query(N - 1) * 2 + 1;
        long long v = rng() % TOT;
        int j = 0;
        long long sm = 0;
        while (j < N && sm + A[j] < v) sm += A[j++];
        ans0.push_back(j);
        ans1.push_back(FT1.lower_bound(v));
        ans2.push_back(FT2.lower_bound(v));
        ans3.push_back(bsearch<FIRST>(0, N, [&] (int k) {
          return FT2.query(k) >= v;
        }));
      } else {
        long long TOT = FT1.query(N - 1) * 2 + 1;
        long long v = rng() % TOT;
        int j = 0;
        long long sm = 0;
        while (j < N && sm + A[j] <= v) sm += A[j++];
        ans0.push_back(j);
        ans1.push_back(FT1.upper_bound(v));
        ans2.push_back(FT2.upper_bound(v));
        ans3.push_back(bsearch<FIRST>(0, N, [&] (int k) {
          return FT2.query(k) > v;
        }));
      }
    }
    vector<long long> A1 = FT1.values(), A2 = FT2.values();
    assert(ans0 == ans1);
    assert(ans0 == ans2);
    assert(ans0 == ans3);
    assert(A == A1);
    assert(A == A2);
    for (auto &&a : ans0) checkSum ^= a;
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (point update, range query, binary search) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
