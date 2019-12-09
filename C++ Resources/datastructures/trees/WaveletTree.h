#pragma once
#include <bits/stdc++.h>
using namespace std;

// Wavelet Tree supporting select and rank operations for a subarray
// Time Complexity:
//   build: O(N log N)
//   select, rank: O(log N)
// Memory Complexity: O(N log N)
template <const int MAXN, const int MAXLGN, class T, class Comparator = less<T>> struct WaveletTree {
    Comparator cmp; function<bool(int, int)> cmpInd; int N, P[MAXN], aux[MAXN], curNode, root[MAXN]; T A[MAXN];
    struct Node { int l, r, val; }; Node TR[MAXN * MAXLGN];
    int build(int tl, int tr) {
        int newNode = ++curNode;
        if (tl == tr) { TR[newNode].val = 0; return newNode; }
        int m = tl + (tr - tl) / 2;
        TR[newNode].l = build(tl, m); TR[newNode].r = build(m + 1, tr);
        TR[newNode].val = TR[TR[newNode].l].val + TR[TR[newNode].r].val; return newNode;
    }
    int update(int cur, int tl, int tr, int ind) {
        if (ind < tl || tr < ind) return cur;
        int newNode = ++curNode;
        if (tl == tr) { TR[newNode].val = 1; return newNode; }
        int m = tl + (tr - tl) / 2;
        TR[newNode].l = update(TR[cur].l, tl, m, ind); TR[newNode].r = update(TR[cur].r, m + 1, tr, ind);
        TR[newNode].val = TR[TR[newNode].l].val + TR[TR[newNode].r].val; return newNode;
    }
    int select(int curA, int curB, int tl, int tr, int k) {
        if (tl == tr) return tl;
        int m = tl + (tr - tl) / 2, x = TR[TR[curB].l].val - TR[TR[curA].l].val;
        if (k < x) return select(TR[curA].l, TR[curB].l, tl, m, k);
        else return select(TR[curA].r, TR[curB].r, m + 1, tr, k - x);
    }
    int rank(int cur, int tl, int tr, int l, int r) {
        if (r < tl || tr < l) return 0;
        if (l <= tl && tr <= r) return TR[cur].val;
        int m = tl + (tr - tl) / 2; return rank(TR[cur].l, tl, m, l, r) + rank(TR[cur].r, m + 1, tr, l, r);
    }
    WaveletTree() : cmpInd([&] (const int &i, const int &j) { return cmp(A[i], A[j]); }) {}
    template <class F> void build(int size, F buildAux) {
        N = size; iota(P, P + N, 0); stable_sort(P, P + N, cmpInd); buildAux();
        curNode = 0; root[0] = build(0, N - 1);
        for (int i = 0; i < N; i++) root[i + 1] = update(root[i], 0, N - 1, aux[i]);
    }
    void buildSelect(int size) { // aux[i] represents sorted index of the ith element
        build(size, [&] { for (int i = 0; i < N; i++) aux[P[i]] = i; });
    }
    void buildRank(int size) { // aux[i] represents the original index of the ith smallest element
        build(size, [&] { for (int i = 0; i < N; i++) aux[i] = P[i]; });
    }
    T select(int l, int r, int k) { // selects the kth smallest element in the range [l, r] (0-indexed)
        return A[P[select(root[l], root[r + 1], 0, N - 1, k)]];
    }
    T rank(int l, int r, T k) { // counts the number of elements less than k in the range [l, r] (0-indexed)
        A[N] = k;
        return rank(root[lower_bound(P, P + N, N, cmpInd) - P], 0, N - 1, l, r);
    }
};
