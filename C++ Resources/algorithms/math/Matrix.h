#pragma once
#include <bits/stdc++.h>
using namespace std;

// Matrix data structure
template <class T> struct Matrix : vector<vector<T>> {
    int N, M; Matrix(int N, int M) : vector<vector<T>>(N, vector<T>(M, 0)), N(N), M(M) {}
};

// Returns the identiy matrix of dimension N
// Time Complexity: O(N^2)
template <class T> Matrix<T> identity(int N) {
    Matrix<T> A(N, N);
    for (int i = 0; i < N; i++) A[i][i] = 1;
    return A;
}

template <class T> Matrix<T> transpose(const Matrix<T> &A) {
    Matrix<T> C(A.M, A.N);
    for (int i = 0; i < C.N; i++) for (int j = 0; j < C.M; j++) C[i][j] = A[j][i];
    return C;
}

// Returns A + B
// Time Complexity: O(N^2)
template <class T> Matrix<T> add(const Matrix<T> &A, const Matrix<T> &B) {
    assert(A.N == B.N && A.M == B.M); Matrix<T> C(A.N, A.M);
    for (int i = 0; i < C.N; i++) for (int j = 0; j < C.M; j++) C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Returns A - B
// Time Complexity: O(N^2)
template <class T> Matrix<T> sub(const Matrix<T> &A, const Matrix<T> &B) {
    assert(A.N == B.N && A.M == B.M); Matrix<T> C(A.N, A.M);
    for (int i = 0; i < C.N; i++) for (int j = 0; j < C.M; j++) C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Returns A * B
// Time Complexity: O(N^3)
template <class T> Matrix<T> mul(const Matrix<T> &A, const Matrix<T> &B) {
    assert(A.M == B.N); Matrix<T> C(A.N, B.M);
    for (int i = 0; i < A.N; i++) for (int j = 0; j < B.M; j++) {
        T temp = 0;
        for (int k = 0; k < A.M; k++) temp += A[i][k] * B[k][j];
        C[i][j] = temp;
    }
    return C;
}

// Returns A * B using the transpose trick and the sqrt caching trick to optimize cache hits
// Time Complexity: O(N^3)
template <class T> Matrix<T> mulOpt(const Matrix<T> &A, const Matrix<T> &B, int BSZ) {
    assert(A.M == B.N); Matrix<T> BP = transpose(B), C(A.N, B.M);
    for (int i = 0; i < A.N; i += BSZ) for (int j = 0; j < BP.N; j += BSZ) for (int k = 0; k < A.M; k += BSZ) {
        for (int ii = i, eni = min(i + BSZ, A.N); ii < eni; ii++) for (int jj = j, enj = min(j + BSZ, BP.N); jj < enj; jj++) {
            T temp = 0;
            for (int kk = k, enk = min(k + BSZ, A.M); kk < enk; kk++) temp += A[ii][kk] * BP[jj][kk];
            C[ii][jj] += temp;
        }
    }
    return C;
}

// Returns A ^ pow
// Time Complexity: O(N^3 log pow)
template <class T, class U> Matrix<T> pow(Matrix<T> A, U pow) {
    assert(A.N == A.M); Matrix<T> x = identity<T>(A.N);
    for (; pow > 0; pow >>= 1, A = mul(A, A)) if (pow & 1) x = mul(x, A);
    return x;
}

// Determinant
// Time Complexity: O(N^3)
template <class T> T det(Matrix<T> A) {
    assert(A.N > 0 && A.N == A.M); T ret = 1;
    for (int i = 0; i < A.N; i++) {
        int mx = i;
        for (int j = i + 1; j < A.N; j++) if (A[j][i] > A[mx][i]) mx = j;
        if (i != mx) { ret = -ret; A[i].swap(A[mx]); }
        for (int j = i + 1; j < A.N; j++) {
            T alpha = A[j][i] / A[i][i];
            for (int k = i + 1; k < A.N; k++) A[j][k] -= alpha * A[i][k];
        }
    }
    for (int i = 0; i < A.N; i++) ret *= A[i][i];
    return ret;
}
