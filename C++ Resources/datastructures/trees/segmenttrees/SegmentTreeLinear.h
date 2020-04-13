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
    Data T[MAXN * 2], L[MAXN * 2]; type A[MAXN]; int N; const Data ZERO = make_pair(0, 0);
    type sumTo(type n) { return n * (n + 1) / 2; }
    Data sumBet(type l, type r) { return make_pair(r - l + 1, sumTo(r) - sumTo(l - 1)); }
    void propagate(int cur, int cL, int cR) {
        int m = cL + (cR - cL) / 2, rc = cur + (m - cL + 1) * 2;
        if (L[cur] != ZERO) {
            T[cur + 1] = T[cur + 1] + L[cur] * sumBet(cL, m); L[cur + 1] = L[cur + 1] + L[cur];
            T[rc] = T[rc] + L[cur] * sumBet(m + 1, cR); L[rc] = L[rc] + L[cur];
            L[cur] = ZERO;
        }
    }
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = make_pair(A[cL], 0); L[cur] = ZERO; return; }
        int m = cL + (cR - cL) / 2, rc = cur + (m - cL + 1) * 2; build(cur + 1, cL, m); build(rc, m + 1, cR); T[cur] = T[cur + 1] + T[rc];
    }
    void update(int cur, int cL, int cR, int l, int r, const Data &val) {
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) { T[cur] = T[cur] + val * sumBet(cL, cR); L[cur] = L[cur] + val; return; }
        int m = cL + (cR - cL) / 2, rc = cur + (m - cL + 1) * 2; propagate(cur, cL, cR);
        update(cur + 1, cL, m, l, r, val); update(rc, m + 1, cR, l, r, val); T[cur] = T[cur + 1] + T[rc];
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return ZERO;
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2, rc = cur + (m - cL + 1) * 2; propagate(cur, cL, cR);
        return query(cur + 1, cL, m, l, r) + query(rc, m + 1, cR, l, r);
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(0, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, 0); build(0, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int l, int r, type val) { update(0, ONE_INDEXED, N - !ONE_INDEXED, l, r, make_pair((1 - l) * val, val)); }
    type query(int l, int r) { Data q = query(0, ONE_INDEXED, N - !ONE_INDEXED, l, r); return q.first + q.second; }
};
