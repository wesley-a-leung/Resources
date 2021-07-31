#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T> using Matrix = vector<vector<T>>;

// Returns the size of the first dimension of a matrix
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the matrix
// Return Value: the size of the first dimension of the matrix A
// In practice, has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> int getN(const Matrix<T> &A) { return A.size(); }

// Returns the size of the second dimension of a matrix
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the matrix
// Return Value: the size of the second dimension of the matrix A
// In practice, has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T>
int getM(const Matrix<T> &A) { return A.empty() ? 0 : A[0].size(); }

// Creates a matrix of size N x M filled with the default value of T
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   N: the size of the first dimension
//   M: the size of the second dimension
// Return Value: a matrix of size N x M filled with the default value of T
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T> Matrix<T> makeMatrix(int N, int M) {
  return vector<vector<T>>(N, vector<T>(M, T()));
}

// Creates an identity matrix of size N x N
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   N: the size of both dimensions
// Return Value: an identity matrix of size N x N
// In practice, has a very small constant
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
// Tested:
//   https://www.spoj.com/problems/MPOW/
template <class T> Matrix<T> identityMatrix(int N) {
  Matrix<T> A = makeMatrix<T>(N, N);
  for (int i = 0; i < N; i++) A[i][i] = T(1);
  return A;
}

// Returns the transpose of a matrix
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the matrix
// Return Value: the transpose of the matrix A
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T> Matrix<T> transpose(const Matrix<T> &A) {
  Matrix<T> C = makeMatrix<T>(getM(A), getN(A));
  for (int i = 0; i < getN(C); i++) for (int j = 0; j < getM(C); j++)
    C[i][j] = A[j][i];
  return C;
}

// Adds B to the matrix A
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: a reference to the first matrix
//   B: a constant reference to the matrix to add
// Return Value: a reference to the matrix A after B is added to it
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T> Matrix<T> &operator += (Matrix<T> &A, const Matrix<T> &B) {
  assert(getN(A) == getN(B) && getM(A) == getM(B));
  for (int i = 0; i < getN(A); i++) for (int j = 0; j < getM(A); j++)
    A[i][j] += B[i][j];
  return A;
}

// Adds the matrices A and B
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the first matrix
//   B: a constant reference to the matrix to add
// Return Value: the matrix A plus B
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T>
Matrix<T> operator + (Matrix<T> A, const Matrix<T> &B) { return A += B; }

// Subtracts B from the matrix A
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: a reference to the first matrix
//   B: a constant reference to the matrix to subtract
// Return Value: a reference to the matrix A after B is subtracted from it
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T> Matrix<T> &operator -= (Matrix<T> &A, const Matrix<T> &B) {
  assert(getN(A) == getN(B) && getM(A) == getM(B));
  for (int i = 0; i < getN(A); i++) for (int j = 0; j < getM(A); j++)
    A[i][j] -= B[i][j];
  return A;
}

// Subtracts the matrices A and B
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the first matrix
//   B: a constant reference to the matrix to add
// Return Value: the matrix A minus B
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz Tested
template <class T>
Matrix<T> operator - (Matrix<T> A, const Matrix<T> &B) { return A -= B; }

// Multiplies the matrices A and B, getM(A) must equal getN(B)
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: a constant reference to the first matrix
//   B: the matrix to multiply
// Return Value: the matrix A times B
// In practice, has a very small constant
// Time Complexity: O(N(A) M(B) M(A))
// Memory Complexity: O((N(A) + N(B)) M(B))
// Tested:
//   https://www.spoj.com/problems/MPOW/
template <class T> Matrix<T> operator * (const Matrix<T> &A, Matrix<T> B) {
  assert(getM(A) == getN(B));
  B = transpose(B); Matrix<T> C = makeMatrix<T>(getN(A), getN(B));
  for (int i = 0; i < getN(C); i++) for (int j = 0; j < getM(C); j++) {
    T tmp = T(); for (int k = 0; k < getM(A); k++) tmp += A[i][k] * B[j][k];
    C[i][j] = tmp;
  }
  return C;
}

// Multiplies the matrix B into the matrix A, getM(A) must equal getN(B)
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: a reference to the first matrix
//   B: a constant reference to the matrix to multiply
// Return Value: a reference to the matrix A after it is multipled by B
// In practice, has a very small constant
// Time Complexity: O(N(A) M(B) M(A))
// Memory Complexity: O((N(A) + N(B)) M(B))
// Tested:
//   https://www.spoj.com/problems/MPOW/
template <class T>
Matrix<T> &operator *= (Matrix<T> &A, const Matrix<T> &B) { return A = A * B; }

// Raises the square matrix A to the power of pow, getN(A) must equal getM(A)
// Template Arguments:
//   T: the type of each element in the matrix
//   U: the type of pow
// Function Arguments:
//   A: the base square matrix
// Return Value: the matrix A raised to the power pow
// In practice, has a very small constant
// Time Complexity: O(N^3 log pow)
// Memory Complexity: O(N^2)
// Tested:
//   https://www.spoj.com/problems/MPOW/
template <class T, class U> Matrix<T> powMat(Matrix<T> A, U pow) {
  assert(getN(A) == getM(A));
  Matrix<T> x = identityMatrix<T>(getN(A)); while (true) {
    if (pow % 2 == 1) x *= A;
    if ((pow /= 2) == 0) break;
    A *= A;
  }
  return x;
}

// Computes the determinant of a square matrix A, getN(A) must equal getM(A)
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the matrix
// Return Value: the determinant of the matrix A
// In practice, has a very small constant
// Time Complexity: O(N^3)
// Memory Complexity: O(N^2)
// Tested:
//   https://judge.yosupo.jp/problem/matrix_det
//   https://dmoj.ca/problem/det
template <class T> T det(Matrix<T> A) {
  auto abs = [&] (T a) { return a >= 0 ? a : -a; };
  int N = getN(A); assert(N == getM(A)); T ret = T(1);
  for (int i = 0; i < N; i++) {
    int mx = i; for (int j = i + 1; j < N; j++)
      if (abs(A[j][i]) > abs(A[mx][i])) mx = j;
    if (i != mx) { ret = -ret; A[i].swap(A[mx]); }
    T inv = T(1) / A[i][i]; for (int j = i + 1; j < N; j++) {
      T alpha = A[j][i] * inv;
      for (int k = i + 1; k < N; k++) A[j][k] -= alpha * A[i][k];
    }
  }
  for (int i = 0; i < N; i++) ret *= A[i][i];
  return ret;
}

// Computes the inverse of a square matrix A, getN(A) must equal getM(A)
// Template Arguments:
//   T: the type of each element in the matrix
// Function Arguments:
//   A: the matrix
//   EPS: a value for epsilon
// Return Value: the inverse of the matrix A,
//   or a 0 x 0 matrix if it does not exist
// In practice, has a very small constant
// Time Complexity: O(N^3)
// Memory Complexity: O(N^2)
// Tested:
//   https://www.spoj.com/problems/MIFF/
//   https://judge.yosupo.jp/problem/inverse_matrix
template <class T> Matrix<T> invMat(Matrix<T> A, T EPS = T(1e-9)) {
  auto abs = [&] (T a) { return a >= 0 ? a : -a; };
  int N = getN(A); assert(N == getM(A)); Matrix<T> I = identityMatrix<T>(N);
  for (int i = 0; i < N; i++) {
    int mx = i; for (int j = i; j < N; j++)
      if (A[j][i] != 0) { mx = j; break; }
    if (abs(A[mx][i]) <= EPS) return makeMatrix<T>(0, 0);
    if (i != mx) { A[i].swap(A[mx]); I[i].swap(I[mx]); }
    T inv = T(1) / A[i][i]; for (int k = i; k < N; k++) A[i][k] *= inv;
    for (int k = 0; k < N; k++) I[i][k] *= inv;
    for (int j = 0; j < N; j++) if (j != i) {
      T alpha = A[j][i]; if (abs(alpha) <= EPS) continue;
      for (int k = i; k < N; k++) A[j][k] -= alpha * A[i][k];
      for (int k = 0; k < N; k++) I[j][k] -= alpha * I[i][k];
    }
  }
  return I;
}
