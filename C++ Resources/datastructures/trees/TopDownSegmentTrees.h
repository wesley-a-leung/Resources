#pragma once
#include <bits/stdc++.h>
using namespace std;

template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree {
    using Data = int; const Data vdef = 0, qdef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data apply(const Data &x, const Data &v); // to be implemented
    Data T[MAXN * 4], A[MAXN]; int N;
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = A[cL]; return; }
        int m = cL + (cR - cL) / 2; build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    void update(int cur, int cL, int cR, int ind, const Data &val) {
        if (cL > ind || cR < ind) return;
        if (cL == cR) { T[cur] = apply(T[cur], val); return; }
        int m = cL + (cR - cL) / 2; update(cur * 2, cL, m, ind, val); update(cur * 2 + 1, m + 1, cR, ind, val);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2;
        return merge(query(cur * 2, cL, m, l, r), query(cur * 2 + 1, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(1, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(1, ONE_INDEXED, size - !ONE_INDEXED); }
    void update(int ind, const Data &val) { update(1, ONE_INDEXED, N - !ONE_INDEXED, ind, val); }
    Data query(int l, int r) { return query(1, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

template <const int MAXN, const bool ONE_INDEXED> struct LazySegmentTree {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data apply(const Data &x, const Lazy &v); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    Data T[MAXN * 4], A[MAXN]; Lazy L[MAXN * 4]; int N;
    void propagate(int cur, int cL, int cR) {
        if (T[cur].lazy != ldef) {
            int m = cL + (cR - cL) / 2;
            T[cur * 2] = apply(T[cur * 2], getSegmentVal(L[cur], m - cL + 1));
            L[cur * 2] = mergeLazy(L[cur * 2], L[cur]);
            T[cur * 2 + 1] = apply(T[cur * 2 + 1], getSegmentVal(L[cur], cR - m));
            L[cur * 2 + 1] = mergeLazy(L[cur * 2 + 1], L[cur]);
            L[cur].lazy = ldef;
        }
    }
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = A[cL]; L[cur] = ldef; return; }
        int m = cL + (cR - cL) / 2; build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    void update(int cur, int cL, int cR, int l, int r, const Lazy &val) {
        if (cL > r || cR < l) return;
        if (cL != cR) propagate(cur, cL, cR);
        if (cL >= l && cR <= r) {
            T[cur] = apply(T[cur], getSegmentVal(val, cR - cL + 1));
            L[cur] = mergeLazy(L[cur], val);
            return;
        }
        int m = cL + (cR - cL) / 2; update(cur * 2, cL, m, l, r, val); update(cur * 2 + 1, m + 1, cR, l, r, val);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (cL != cR) propagate(cur, cL, cR);
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2;
        return merge(query(cur * 2, cL, m, l, r), query(cur * 2 + 1, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(1, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(1, ONE_INDEXED, size - !ONE_INDEXED); }
    void update(int l, int r, const Lazy &val) { update(1, ONE_INDEXED, N - !ONE_INDEXED, l, r, val); }
    Data query(int l, int r) { return query(1, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

template <const bool ONE_INDEXED> struct PersistentSegmentTree {
    using Data = int; using Lazy = int; static const Data vdef = 0, qdef = 0; static const Lazy ldef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data apply(const Data &x, const Lazy &v); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    struct Node {
        Node *left = nullptr, *right = nullptr;
        Data val = PersistentSegmentTree::vdef;
        Lazy lazy = PersistentSegmentTree::ldef;
    };
    void propagate(Node *cur, int cL, int cR) {
        if (cur->lazy != ldef) {
            int m = cL + (cR - cL) / 2;
            if (cur->left == nullptr) cur->left = new Node();
            cur->left->val = apply(cur->left->val, getSegmentVal(cur->lazy, m - cL + 1));
            cur->left->lazy = mergeLazy(cur->left->lazy, cur->lazy);
            if (cur->right == nullptr) cur->right = new Node();
            cur->right->val = apply(cur->right->val, getSegmentVal(cur->lazy, cR - m));
            cur->right->lazy = mergeLazy(cur->right->lazy, cur->lazy);
            cur->lazy = ldef;
        }
    }
    int N; vector<Node*> roots = {new Node()};
    template <class It> Node *build(int cL, int cR, It st) {
        Node *ret = new Node();
        if (cL == cR) { ret->val = *(st + cL - ONE_INDEXED); return ret; }
        int m = cL + (cR - cL) / 2;
        ret->left = build(cL, m, st); ret->right = build(m + 1, cR, st);
        ret->val = merge(ret->left->val, ret->right->val);
        return ret;
    }
    Node *update(Node *cur, int cL, int cR, int l, int r, const Lazy &val) {
        if (cL > r || cR < l) return cur;
        Node *ret = new Node();
        if (cur) { ret->left = cur->left; ret->right = cur->right; ret->val = cur->val, ret->lazy = cur->lazy; }
        if (cL != cR) propagate(ret, cL, cR);
        if (cL >= l && cR <= r) {
            ret->val = apply(ret->val, getSegmentVal(val, cR - cL + 1));
            ret->lazy = mergeLazy(ret->lazy, val);
            return ret;
        }
        int m = cL + (cR - cL) / 2;
        ret->left = update(ret->left, cL, m, l, r, val);
        ret->right = update(ret->right, m + 1, cR, l, r, val);
        if (ret->left && ret->right) ret->val = merge(ret->left->val, ret->right->val);
        else if (ret->left) ret->val = merge(ret->left->val, Data(vdef));
        else if (ret->right) ret->val = merge(ret->right->val, Data(vdef));
        return ret;
    }
    Data query(Node *cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (!cur) return vdef;
        if (cL != cR) propagate(cur, cL, cR);
        if (cL >= l && cR <= r) return cur->val;
        int m = cL + (cR - cL) / 2;
        return merge(query(cur->left, cL, m, l, r), query(cur->right, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) { N = en - st; roots.push_back(build(ONE_INDEXED, N - !ONE_INDEXED, st)); }
    void init(int size) { N = size; }
    void update(int l, int r, int val) { roots.push_back(update(roots.back(), ONE_INDEXED, N - !ONE_INDEXED, l, r, val)); }
    Data query(int rootInd, int l, int r) { return query(roots[rootInd], ONE_INDEXED, N - !ONE_INDEXED, l, r); }
    void revert(int x) { roots.push_back(roots[x]); }
};
