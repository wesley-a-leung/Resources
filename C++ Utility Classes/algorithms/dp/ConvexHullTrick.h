#ifndef ALGORITHMS_CONVEXHULLTRICK_H_
#define ALGORITHMS_CONVEXHULLTRICK_H_

#include <bits/stdc++.h>
using namespace std;

class ConvexHullTrick {
private:
    vector<long long> M, B;
    int front, back;

public:
    ConvexHullTrick(int N) : M(N), B(N), front(0), back(0) {}

    void addLine(long long m, long long b) {
        while (back >= 2 && (B[back - 2] - B[back - 1]) * (m - M[back - 1]) >= (B[back - 1] - b) * (M[back - 1] - M[back - 2])) back--;
        M[back] = m;
        B[back++] = b;
    }

    long long getMax(long long x) {
        while (front < back - 1 && M[front + 1] * x + B[front + 1] >= M[front] * x + B[front]) front++;
        return M[front] * x + B[front];
    }
};

#endif /* ALGORITHMS_CONVEXHULLTRICK_H_ */
