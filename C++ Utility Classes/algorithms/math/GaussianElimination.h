/*
 * GaussianElimination.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_MATH_GAUSSIANELIMINATION_H_
#define ALGORITHMS_MATH_GAUSSIANELIMINATION_H_

#include <bits/stdc++.h>

using namespace std;

const double EPSILON = 1e-10;

// Gaussian elimination with partial pivoting
double *solve(int n, double **A, double *b) {
    for (int p = 0; p < n; p++) {
        // find pivot row and swap
        int m = p;
        for (int i = p + 1; i < n; i++) {
            if (abs(A[i][p]) > abs(A[m][p])) m = i;
        }
        swap(A[p], A[m]);
        swap(b[p], b[m]);
        // singular or nearly singular
        if (abs(A[p][p]) <= EPSILON) throw runtime_error("Matrix is singular or nearly singular");
        // pivot within A and b
        for (int i = p + 1; i < n; i++) {
            double alpha = A[i][p] / A[p][p];
            b[i] -= alpha * b[p];
            for (int j = p; j < n; j++) {
                A[i][j] -= alpha * A[p][j];
            }
        }
    }
    // back substitution
    double *x = new double[n];
    for (int i = 0; i < n; i++) {
        x[i] = 0.0;
    }
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
    return x;
}

#endif /* ALGORITHMS_MATH_GAUSSIANELIMINATION_H_ */
