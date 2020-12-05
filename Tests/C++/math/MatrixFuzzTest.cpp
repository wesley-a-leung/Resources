#include <bits/stdc++.h>
#include "../../../Content/C++/math/Matrix.h"
using namespace std;

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e4, OPS = 10;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 11, M = rng() % 11;
    Matrix<long long> A = makeMatrix<long long>(N, M), B = A;
    for (int k = 0; k < OPS; k++) {
      Matrix<long long> C = makeMatrix<long long>(N, M);
      for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) C[i][j] = rng() % int(1e9);
      A = A + C;
      B = B - C;
      for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) checkSum = 31 * checkSum + A[i][j];
      for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) checkSum = 31 * checkSum + B[i][j];
    }
    assert((A + B) == makeMatrix<long long>(N, M));
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
