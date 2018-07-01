#ifndef ALGORITHMS_CONVEXHULLTRICK_H_
#define ALGORITHMS_CONVEXHULLTRICK_H_

#include <bits/stdc++.h>
using namespace std;

// convention is same as priority_queue in STL
template <typename T, typename Comparator = less<T>>
class ConvexHullTrick {
private:
    Comparator cmp;
    vector<T> M, B;
    int front, back;

public:
    ConvexHullTrick(int N) : M(N), B(N), front(0), back(0) {}

    void addLine(T m, T b) {
        while (back >= 2 && !cmp((B[back - 2] - B[back - 1]) * (m - M[back - 1]), (B[back - 1] - b) * (M[back - 1] - M[back - 2]))) back--;
        M[back] = m;
        B[back++] = b;
    }

    T getMax(T x) {
        while (front < back - 1 && !cmp(M[front + 1] * x + B[front + 1], M[front] * x + B[front])) front++;
        return M[front] * x + B[front];
    }
};

#endif /* ALGORITHMS_CONVEXHULLTRICK_H_ */
