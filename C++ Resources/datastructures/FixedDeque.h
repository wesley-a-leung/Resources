#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fixed size deque using a circular buffer
// Time Complexity:
//   constructor: O(N)
//   clear, push_back, emplace_back, push_front, emplace_front, pop_back, pop_front: O(1)
//   back, front, size, empty, at, accessor: O(1)
// Memory Complexity: O(N)
template <class T, const int MAXN> struct FixedDeque {
    T A[MAXN]; int N, st = 0, en = 0;
    FixedDeque(int N = 0) : N(N) {}
    FixedDeque(int N, T val): N(N) { fill(A, A + N, val); }
    void clear() { N = st = en = 0; }
    void push_back(const T &val) {
        if (en == MAXN) en = 0;
        A[en++] = val; N++;
    }
    template <class ...Args> void emplace_back(Args &&...args) { push_back(T(forward<Args>(args)...)); }
    void push_front(const T &val) {
        if (st == 0) st = MAXN;
        A[--st] = val; N++;
    }
    template <class ...Args> void emplace_front(Args &&...args) { push_front(T(forward<Args>(args)...)); }
    void pop_back() {
        if (en == 0) en = MAXN;
        en--; N--;
    }
    void pop_front() {
        st++; N--;
        if (st == MAXN) st = 0;
    }
    const T &back() const { return A[en == 0 ? MAXN - 1 : en - 1]; }
    const T &front() const { return A[st]; }
    int size() const { return N; }
    bool empty() const { return N == 0; }
    T &operator [](int i) { return A[st + i < MAXN ? st + i : st + i - MAXN]; }
    const T &at(const int i) { return A[st + i < MAXN ? st + i : st + i - MAXN]; }
    const T &operator [](const int i) const { return at(i); }
};
