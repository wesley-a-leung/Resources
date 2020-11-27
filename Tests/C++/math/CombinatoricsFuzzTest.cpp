#include <bits/stdc++.h>
#include "../../../Content/C++/math/Combinatorics.h"
#include "../../../Content/C++/math/ModularArithmetic.h"
using namespace std;

const long long MOD1 = 1e9, MOD2 = 1e9 + 7;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 20;
  long long checkSum = 0;
  vector<long long> ans0, ans1, ans2;
  Combinatorics<long long> C(N);
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long n = rng() % (N + 1);
    ans0.push_back(factorial(n) % MOD1);
    ans1.push_back(factorialMod(n, MOD1));
    ans2.push_back(C.factorial(n) % MOD1);
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  assert(ans0 == ans2);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Factorial Mod) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 50;
  long long checkSum = 0;
  vector<long long> ans0, ans1;
  CombinatoricsModPrime<long long> C(N, MOD2);
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long n = rng() % (N + 1);
    ans0.push_back(factorialMod(n, MOD2));
    ans1.push_back(C.factorial(n));
    assert(mulMod(C.factorial(n), C.invFactorial(n), MOD2) == 1);
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Factorial Mod Prime) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 20;
  long long checkSum = 0;
  vector<long long> ans0, ans1, ans2, ans3;
  Combinatorics<long long> C(N);
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long n = rng() % (N + 1), k = rng() % (n + 1);
    ans0.push_back(factorial(n) / factorial(n - k) % MOD1);
    ans1.push_back(permute(n, k) % MOD1);
    ans2.push_back(permuteMod(n, k, MOD1));
    ans3.push_back(C.permute(n, k) % MOD1);
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  assert(ans0 == ans2);
  assert(ans0 == ans3);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Permute Mod) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 50;
  long long checkSum = 0;
  vector<long long> ans0, ans1, ans2;
  CombinatoricsModPrime<long long> C(N, MOD2);
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long n = rng() % (N + 1), k = rng() % (n + 1);
    ans0.push_back(divModPrime(factorialMod(n, MOD2), factorialMod(n - k, MOD2), MOD2));
    ans1.push_back(permuteMod(n, k, MOD2));
    ans2.push_back(C.permute(n, k));
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  assert(ans0 == ans2);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 4 (Permute Mod Prime) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test5() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 20;
  long long checkSum = 0;
  vector<long long> ans0, ans1, ans2;
  Combinatorics<long long> C(N);
  for (int ti = 0; ti < TESTCASES; ti++) {
    if (rng() % 2) {
      long long n = rng() % (N + 1), k = rng() % (n + 1);
      ans0.push_back(factorial(n) / factorial(n - k) / factorial(k));
      ans1.push_back(choose(n, k));
      ans2.push_back(C.choose(n, k));
    } else {
      long long n = rng() % (N / 2) + 1, k = rng() % (N / 2);
      ans0.push_back(factorial(n + k - 1) / factorial(n - 1) / factorial(k));
      ans1.push_back(multiChoose(n, k));
      ans2.push_back(C.multiChoose(n, k));
    }
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  assert(ans0 == ans2);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 5 (Combinations) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test6() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 50;
  long long checkSum = 0;
  vector<long long> ans0, ans1, ans2;
  CombinatoricsModPrime<long long> C(N, MOD2);
  for (int ti = 0; ti < TESTCASES; ti++) {
    if (rng() % 2) {
      long long n = rng() % (N + 1), k = rng() % (n + 1);
      ans0.push_back(divModPrime(divModPrime(factorialMod(n, MOD2), factorialMod(n - k, MOD2), MOD2), factorialMod(k, MOD2), MOD2));
      ans1.push_back(chooseModPrime(n, k, MOD2));
      ans2.push_back(C.choose(n, k));
    } else {
      long long n = rng() % (N / 2) + 1, k = rng() % (N / 2);
      ans0.push_back(divModPrime(divModPrime(factorialMod(n + k - 1, MOD2), factorialMod(n - 1, MOD2), MOD2), factorialMod(k, MOD2), MOD2));
      ans1.push_back(multiChooseModPrime(n, k, MOD2));
      ans2.push_back(C.multiChoose(n, k));
    }
    checkSum = 31 * checkSum + ans0.back();
  }
  assert(ans0 == ans1);
  assert(ans0 == ans2);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 6 (Combinations Mod Prime) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test7() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int N = 20;
  long long checkSum = 0;
  Combinatorics<long long> C(N);
  vector<vector<long long>> A = pascalsTriangle<long long>(N), B = pascalsTriangleMod<long long>(N, MOD1);
  for (int i = 0; i <= N; i++) {
    assert(A[i] == pascalsRow<long long>(i));
    for (int j = 0; j <= i; j++) {
      assert(A[i][j] % MOD1 == C.choose(i, j) % MOD1);
      assert(A[i][j] % MOD1 == B[i][j]);
      checkSum = checkSum * 31 + A[i][j];
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 7 (Pascal's Triangle Mod) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test8() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int N = 1000;
  long long checkSum = 0;
  CombinatoricsModPrime<long long> C(N, MOD2);
  vector<vector<long long>> A = pascalsTriangleMod<long long>(N, MOD2);
  for (int i = 0; i <= N; i++) {
    assert(A[i] == pascalsRowModPrime<long long>(i, MOD2));
    for (int j = 0; j <= i; j++) {
      assert(A[i][j] == C.choose(i, j));
      checkSum = checkSum * 31 + A[i][j];
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 8 (Pascal's Triangle Mod Prime) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test9() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 20;
  long long checkSum = 0;
  vector<long long> ans0, ans1;
  for (int ti = 0; ti < TESTCASES; ti++) {
    if (rng() % 2) {
      long long n = rng() % (N + 1);
      long long sm = 0;
      for (long long i = 0; i <= n; i++) sm += i;
      ans0.push_back(sm);
      ans1.push_back(sumTo(n));
    } else {
      long long a = rng() % (N + 1), b = rng() % (N + 1);
      if (a > b) swap(a, b);
      long long sm = 0;
      for (long long i = a; i <= b; i++) sm += i;
      ans0.push_back(sm);
      ans1.push_back(sumBetween(a, b));
    }
    checkSum = checkSum * 31 + ans0.back();
  }
  assert(ans0 == ans1);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 9 (Summation) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test10() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 20, MAXVAL = 1e6;
  long long checkSum = 0;
  vector<long long> ans0, ans1;
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long a1 = rng() % (MAXVAL * 2 + 1) - MAXVAL;
    long long d = rng() % (MAXVAL * 2 + 1) - MAXVAL;
    int n = rng() % N + 1;
    long long v = a1, sm = a1;
    for (int i = 2; i <= n; i++) {
      v += d;
      sm += v;
    }
    ans0.push_back(v);
    ans1.push_back(arithSeq(a1, d, n));
    checkSum = checkSum * 31 + ans0.back();
    ans0.push_back(sm);
    ans1.push_back(arithSeries(a1, d, n));
    checkSum = checkSum * 31 + ans0.back();
  }
  assert(ans0 == ans1);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 10 (Arithmetic Sequence) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test11() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e6;
  const int N = 10, MAXVAL = 10;
  long long checkSum = 0;
  vector<long long> ans0, ans1;
  for (int ti = 0; ti < TESTCASES; ti++) {
    long long a1 = rng() % (MAXVAL * 2 + 1) - MAXVAL;
    long long r = rng() % (MAXVAL * 2 + 1) - MAXVAL;
    int n = rng() % N + 1;
    long long v = a1, sm = a1;
    for (int i = 2; i <= n; i++) {
      v *= r;
      sm += v;
    }
    ans0.push_back(v);
    ans1.push_back(geoSeq(a1, r, n));
    checkSum = checkSum * 31 + ans0.back();
    ans0.push_back(sm);
    ans1.push_back(geoSeries(a1, r, n));
    checkSum = checkSum * 31 + ans0.back();
  }
  assert(ans0 == ans1);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 11 (Geometric Sequence) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  cout << "Test Passed" << endl;
  return 0;
}
