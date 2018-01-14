#ifndef ALGORITHMS_MATH_MATRIX_H_
#define ALGORITHMS_MATH_MATRIX_H_

#include <bits/stdc++.h>
using namespace std;

class Matrix {
    // return n-by-n identity matrix I
    double **identity(int n) {
        double **a = new double*[n];
        for (int i = 0; i < n; i++)
            a[i] = new double[n];
        for (int i = 0; i < n; i++)
            a[i][i] = 1;
        return a;
    }

    // return x^T y, both 1-by-n matrix
    double dot(double *x, double *y, int n) {
        double sum = 0.0;
        for (int i = 0; i < n; i++)
            sum += x[i] * y[i];
        return sum;
    }

    // return B = A^T, both m-by-n matrix
    double *transpose(double **a, int m, int n) {;
        double **b = new double*[n];
        for (int i = 0; i < m; i++)
            b[i] = new double[m];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                b[j][i] = a[i][j];
        return b;
    }

    // return c = a + b, all m-by-n matrices
    double **add(double **a, double **b, int m, int n) {
        double **c = new double*[m];
        for (int i = 0; i < n; i++)
            c[i] = new double[n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                c[i][j] = a[i][j] + b[i][j];
        return c;
    }

    // return c = a - b, all m-by-n matrices
    double **subtract(double **a, double **b, int m, int n) {
        double **c = new double*[m];
        for (int i = 0; i < n; i++)
            c[i] = new double[n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                c[i][j] = a[i][j] - b[i][j];
        return c;
    }

    // return c = a * b, a is a m-by-n matrix, b is a n-by-p matrix, c is a m-by-p matrix
    double **multiply(double **a, double **b, int m, int n, int p) {
        double **c = new double*[m];
        for (int i = 0; i < p; i++)
            c[i] = new double[p];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < p; j++)
                for (int k = 0; k < n; k++)
                    c[i][j] += a[i][k] * b[k][j];
        return c;
    }

    // matrix-vector multiplication (y = A * x), a is a m-by-n matrix, x is a 1-by-n matrix, y is a 1-by-m matrix
    double *multiply(double **a, double *x, int m, int n) {
        double *y = new double[m];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                y[i] += a[i][j] * x[j];
        return y;
    }

    // vector-matrix multiplication (y = x^T A), x is a 1-by-m matrix, a is a m-by-n matrix, y1 is a 1-by-n matrix;
    double *multiply(double *x, double **a, int m, int n) {
        double *y = new double[n];
        for (int j = 0; j < n; j++)
            for (int i = 0; i < m; i++)
                y[j] += a[i][j] * x[i];
        return y;
    }

    // matrix power (ans = a^x), a and ans are n-by-n matrices, x is a long long
    double **pow(double **a, long long x, int n) {
        double **ans = new double*[n];
        for (int i = 0; i < n; i++)
            ans[i] = new double[n];
        bool flag = false;
        while (x > 0) {
            if (x % 2 == 1) {
                if (!flag) {
                    ans = a;
                    flag = true;
                } else {
                    ans = multiply(ans, a, n, n, n);
                }
            }
            a = multiply(a, a, n, n, n);
            x /= 2;
        }
        return ans;
    }
};

#endif /* ALGORITHMS_MATH_MATRIX_H_ */
