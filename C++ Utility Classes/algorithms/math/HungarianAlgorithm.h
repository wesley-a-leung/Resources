#ifndef ALGORITHMS_MATH_MATCHING_HUNGARIANALGORITHM_H_
#define ALGORITHMS_MATH_MATCHING_HUNGARIANALGORITHM_H_

#include <bits/stdc++.h>
using namespace std;

void reduceMatrix(int **aux, int n) {
    for (int i = 0; i < n; i++) {
        int min = 1 << 30;
        for (int j = 0; j < n; j++)
        min = min(min, aux[i][j]);
        for (int j = 0; j < n; j++)
        aux[i][j] -= min;
    }
    for (int j = 0; j < n; j++) {
        int min = 1 << 30;
        for (int i = 0; i < n; i++)
        min = min(min, aux[i][j]);
        for (int i = 0; i < n; i++)
        aux[i][j] -= min;
    }
}

/**
 * The Hungarian algorithm is a combinatorial optimization algorithm that solves the assignment problem in polynomial time.
 *
 * Time complexity: O(N^3)
 *
 * @param matrix a square matrix where the rows represent workers and the columns represents jobs
 * @param n the number of workers and jobs
 *
 * @author Jeffrey Xiao
 */
int solve(int **matrix, int n) {
    int **aux = new int*[n];
    for (int i = 0; i < n; i++) {
        aux[i] = new int*[n];
        for (int j = 0; j < n; j++) {
            aux[i][j] = matrix[i][j];
        }
    }
    bool markedRow[n], markedCol[n], assignedRow[n], assignedCol[n], assigned[n][n];
    while (true) {
        reduceMatrix(aux, n);
        for (int i = 0; i < n; i++) {
            markedRow[i] = false;
            markedCol[i] = false;
            assignedRow[i] = false;
            assignedCol[i] = false;
            for (int j = 0; j < n; j++) {
                assigned[i][j] = false;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (aux[i][j] == 0 && !assignedRow[i] && !assignedCol[j]) {
                    assignedRow[i] = true;
                    assignedCol[j] = true;
                    assigned[i][j] = true;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            if (!assignedRow[i]) {
                markedRow[i] = true;
                for (int j = 0; j < n; j++) {
                    if (aux[i][j] == 0) {
                        markedCol[j] = true;
                        for (int k = 0; k < n; k++)
                        if (assigned[k][j])
                        markedRow[k] = true;
                    }
                }
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (markedCol[i])
            cnt++;
            if (!markedRow[i])
            cnt++;
        }
        if (cnt == n)
        break;
        int min = 1 << 30;
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        if (!markedCol[j] && markedRow[i])
        min = min(aux[i][j], min);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (markedCol[j])
                aux[i][j] += min;
                if (markedRow[i])
                aux[i][j] -= min;
            }
        }
    }
    int res = 0;
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
    if (assigned[i][j])
    res += matrix[i][j];
    return res;
}

#endif /* ALGORITHMS_MATH_MATCHING_HUNGARIANALGORITHM_H_ */
