#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports range queries for the maximum (non empty) contiguous subarray sum, along with range updates

using Data = long long; const Data vdef = 0, ldef = LLONG_MAX;
struct Node {
    Data pre, suf, sum, maxSum, lazy; bool isNull;
    Node() : pre(0), suf(0), sum(0), maxSum(0), lazy(ldef), isNull(true) {}
    Node(Data val) : pre(val), suf(val), sum(val), maxSum(val), lazy(ldef), isNull(false) {}
    Node(Node l, Node r) {
        if (l.isNull) {
            pre = r.pre; suf = r.suf; sum = r.sum; maxSum = r.maxSum;
            lazy = ldef; isNull = r.isNull;
        } else if (r.isNull) {
            pre = l.pre; suf = l.suf; sum = l.sum; maxSum = l.maxSum;
            lazy = ldef; isNull = l.isNull;
        } else {
            pre = max(l.pre, l.sum + r.pre); suf = max(l.suf + r.sum, r.suf); sum = l.sum + r.sum;
            maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre);
            lazy = ldef; isNull = false;
        }
    }
};

template <const int MAXN> struct MaxContiguousSubarraySumSegmentTree {
    int N; Node T[MAXN * 4]; Data A[MAXN];
    void propagate(int cur, int cL, int cR) {
        if (T[cur].lazy != ldef) {
            int m = cL + (cR - cL) / 2;
            T[cur * 2].pre = T[cur * 2].suf = T[cur * 2].maxSum = max(T[cur].lazy, T[cur * 2].sum = T[cur].lazy * (m - cL + 1));
            T[cur * 2].lazy = T[cur].lazy;
            T[cur * 2 + 1].pre = T[cur * 2 + 1].suf = T[cur * 2 + 1].maxSum = max(T[cur].lazy, T[cur * 2 + 1].sum = T[cur].lazy * (cR - m));
            T[cur * 2 + 1].lazy = T[cur].lazy;
            T[cur].lazy = ldef;
        }
    }
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = Node(A[cL]); return; }
        int m = cL + (cR - cL) / 2;
        build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR);
        T[cur] = Node(T[cur * 2], T[cur * 2 + 1]);
    }
    void update(int cur, int cL, int cR, int l, int r, Data val) {
        if (cL > r || cR < l) return;
        if (cL != cR) propagate(cur, cL, cR);
        if (cL >= l && cR <= r) {
            T[cur].pre = T[cur].suf = T[cur].maxSum = max(val, T[cur].sum = val * (cR - cL + 1));
            T[cur].lazy = val; T[cur].isNull = false;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, l, r, val); update(cur * 2 + 1, m + 1, cR, l, r, val);
        T[cur] = Node(T[cur * 2], T[cur * 2 + 1]);
    }

    Node query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return Node();
        if (cL != cR) propagate(cur, cL, cR);
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2;
        return Node(query(cur * 2, cL, m, l, r), query(cur * 2 + 1, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 1; i <= N; i++) A[i] = *(st + i - 1);
        build(1, 1, N);
    }
    void init(int size) { N = size; fill(A + 1, A + N + 1, vdef); build(1, 1, N); }
    void update(int l, int r, Data val) { update(1, 1, N, l, r, val); }
    Node query(int l, int r) { return query(1, 1, N, l, r); }
};
