#pragma once
#include <bits/stdc++.h>
using namespace std;

// FenwickTree supporting range updates with updates in the form of
// adding v, 2v, 3v, ... to the interval [l, r], and range queries
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
template <class T> struct FenwickTreeQuadratic {
    int size; vector<T> con, lin, quad;
    T rsq(vector<T> &BIT, int i) { T ret = 0; for (; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    void update(vector<T> &BIT, int i, T v) { for (; i <= size; i += i & -i) BIT[i] += v; }
    FenwickTreeQuadratic(int size) : size(size), con(size + 1), lin(size + 1), quad(size + 1) {}
    T rsq(int ind) { return (rsq(quad, ind) * (T) ind * (T) ind + rsq(lin, ind) * (T) ind + rsq(con, ind)) / (T) 2; }
    T rsq(int a, int b) { return rsq(b) - rsq(a - 1); }
    void update(int a, int b, T value) {
        int s = a - 1, len = b - a + 1;
        update(quad, a, value); update(quad, b + 1, -value);
        update(lin, a, value * ((T) 1 - (T) 2 * (T) s)); update(lin, b + 1, -value * ((T) 1 - (T) 2 * (T) s));
        update(con, a, value * ((T) s * (T) s - (T) s)); update(con, b + 1, -value * (((T) s * (T) s - (T) s)));
        update(con, b + 1, value * ((T) len * (T) (len + 1)));
    }
};
