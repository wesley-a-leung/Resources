#pragma once
#include <bits/stdc++.h>
using namespace std;

// Segment Tree supporting range updates with updates in the form of
// adding v, 2v, 3v, ... to the interval [l, r], and range sum queries
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)

using type = long long; using Data = pair<type, type>;
Data operator + (const Data &a, const Data &b) { return make_pair(a.first + b.first, a.second + b.second); }
Data operator * (const Data &a, const Data &b) { return make_pair(a.first * b.first, a.second * b.second); }

template <const int MAXN, const bool ONE_INDEXED> struct SegmentTreeLinear {
    Data T[MAXN * 4], L[MAXN * 4]; type A[MAXN]; int N; const Data ZERO = make_pair(0, 0);
    type sumTo(type n) { return n * (n + 1) / 2; }
    Data sumBet(type l, type r) { return make_pair(r - l + 1, sumTo(r) - sumTo(l - 1)); }
    void propagate(int cur, int cL, int cR) {
        int m = cL + (cR - cL) / 2;
        if (L[cur] != ZERO) {
            T[cur * 2] = T[cur * 2] + L[cur] * sumBet(cL, m); L[cur * 2] = L[cur * 2] + L[cur];
            T[cur * 2 + 1] = T[cur * 2 + 1] + L[cur] * sumBet(m + 1, cR); L[cur * 2 + 1] = L[cur * 2 + 1] + L[cur];
            L[cur] = ZERO;
        }
    }
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = make_pair(A[cL], 0); L[cur] = ZERO; return; }
        int m = cL + (cR - cL) / 2; build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR); T[cur] = T[cur * 2] + T[cur * 2 + 1];
    }
    void update(int cur, int cL, int cR, int l, int r, const Data &val) {
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) { T[cur] = T[cur] + val * sumBet(cL, cR); L[cur] = L[cur] + val; return; }
        int m = cL + (cR - cL) / 2; propagate(cur, cL, cR);
        update(cur * 2, cL, m, l, r, val); update(cur * 2 + 1, m + 1, cR, l, r, val); T[cur] = T[cur * 2] + T[cur * 2 + 1];
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return ZERO;
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2; propagate(cur, cL, cR); return query(cur * 2, cL, m, l, r) + query(cur * 2 + 1, m + 1, cR, l, r);
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 1; i <= N; i++) A[i] = *(st + i - 1);
        build(1, 1, N);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, 0); build(1, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int l, int r, type val) { update(1, ONE_INDEXED, N - !ONE_INDEXED, l, r, make_pair((1 - l) * val, val)); }
    type query(int l, int r) { Data q = query(1, ONE_INDEXED, N - !ONE_INDEXED, l, r); return q.first + q.second; }
};
