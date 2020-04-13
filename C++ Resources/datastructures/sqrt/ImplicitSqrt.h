#pragma once
#include <bits/stdc++.h>
using namespace std;

// ImplicitSqrt for range updates and queries
// Time Complexity:
//   constructor: O(N)
//   build, rebuild, values: O(N)
//   insert: O(sqrt N + M) amortized for M insertions
//   erase, update, query: O(sqrt N)
//   reverse: O(sqrt N) amortized
//   size: O(1)
// Memory Complexity: O(N)

using Data = int; using Lazy = int;
const int SCALE_FACTOR = 4; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Block : public vector<Data> {
    int p; Data d; Lazy l; bool rev;
    void propagate() {
        if (rev) {
            int i = 0, j = int(size()) - 1; rev = false;
            for (; i < j; i++, j--) { iter_swap(begin() + i, begin() + j); revData(operator[](i)); revData(operator[](j)); }
            if (i == j) revData(operator[](i));
        }
        if (l != ldef) {
            for (int i = 0, s = int(size()); i < s; i++) operator[](i) = applyLazy(operator[](i), l);
            l = ldef;
        }
    }
    void pushup() {
        d = qdef;
        for (int i = 0, s = int(size()); i < s; i++) d = merge(d, operator[](i));
    }
    void reverse() { revData(d); rev ^= true; }
    void swap(Block &b) { std::swap(p, b.p); std::swap(d, b.d); std::swap(l, b.l); std::swap(rev, b.rev); vector<Data>::swap(b); }
    template <class It> Block(int p, It st, It en) : vector<Data>(st, en), p(p), l(ldef), rev(false) { pushup(); }
};
template <class It> void build(It st, It en, vector<Block> &a) {
    int sqrtn = sqrt(en - st) * SCALE_FACTOR;
    for (It i = st; i < en; i += sqrtn) a.emplace_back(a.empty() ? 0 : a.back().p + int(a.back().size()), i, min(i + sqrtn, en));
}
void build(int n, vector<Block> &a) { vector<Data> d(n, vdef); build(d.begin(), d.end(), a); }
struct ImplicitSqrt {
    int N; vector<Block> A;
    ImplicitSqrt(int n = 0) : N(n) { build(N, A); }
    template <class It> ImplicitSqrt(It st, It en) : N(en - st) { build(st, en, A); }
    void rebuild() {
        vector<Data> B; B.reserve(N); for (auto &&a : A) { a.propagate(); for (auto &&x : a) B.push_back(x); }
        A.clear(); build(B.begin(), B.end(), A);
    }
    template <class It> void insert(int k, It st, It en) { // inserts the range [st, en) before index k
        if (st == en) return;
        int lo = 0, hi = int(A.size()) - 1, mid, sqrtn = sqrt(N += en - st) * SCALE_FACTOR;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1 || (k -= A[hi].p) == int(A[hi].size())) build(st, en, A);
        else {
            if (0 < k) {
                A[hi].propagate(); A.emplace(A.begin() + hi + 1, A[hi].p + k, A[hi].begin() + k, A[hi].end());
                A[hi].resize(k); A[hi++].pushup();
            }
            vector<Block> B; build(st, en, B); A.insert(A.begin() + hi, B.begin(), B.end());
            if (hi < int(A.size())) A[hi].p = hi == 0 ? 0 : A[hi - 1].p + int(A[hi - 1].size());
            for (int i = hi + 1, s = int(A.size()); i < s; i++) A[i].p = A[i - 1].p + int(A[i - 1].size());
        }
        if (int(A.size()) > 2 * sqrtn) rebuild();
    }
    void erase(int k, int len) { // erases the range [k, k + len)
        if (len == 0) return;
        int lo = 0, hi = int(A.size()) - 1, mid; N -= len;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int l1 = hi, l2 = k - A[hi].p; lo = 0; hi = int(A.size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k + len - 1 < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int r1 = hi, r2 = k + len - 1 - A[hi].p;
        if (l1 == r1) {
            A[l1].propagate(); A[l1].erase(A[l1].begin() + l2, A[l1].begin() + r2 + 1);
            if (A[l1].empty()) {
                A.erase(A.begin() + l1);
                if (l1 < int(A.size())) A[l1].p = l1 == 0 ? 0 : A[l1 - 1].p + int(A[l1 - 1].size());
            } else A[l1].pushup();
            for (int i = l1 + 1, s = int(A.size()); i < s; i++) A[i].p = A[i - 1].p + int(A[i - 1].size());
        } else {
            A[r1].propagate(); A[r1].erase(A[r1].begin(), A[r1].begin() + r2 + 1);
            if (A[r1].empty()) A.erase(A.begin() + r1);
            else A[r1].pushup();
            A[l1].propagate(); A[l1].erase(A[l1].begin() + l2, A[l1].end());
            if (A[l1].empty()) { A.erase(A.begin() + l1--); r1--; }
            else A[l1].pushup();
            if (l1 + 1 < r1) A.erase(A.begin() + l1 + 1, A.begin() + r1);
            if (l1 < 0) l1 = 0;
            if (l1 < int(A.size())) A[l1].p = l1 == 0 ? 0 : A[l1 - 1].p + int(A[l1 - 1].size());
            for (int i = l1 + 1, s = int(A.size()); i < s; i++) A[i].p = A[i - 1].p + int(A[i - 1].size());
        }
    }
    void update(int k, int len, const Lazy &v) { // updates the range [k, k + len)
        if (len == 0) return;
        int lo = 0, hi = int(A.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int l1 = hi, l2 = k - A[hi].p; lo = 0; hi = int(A.size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k + len - 1 < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int r1 = hi, r2 = k + len - 1 - A[hi].p;
        if (l1 == r1) {
            A[l1].propagate();
            for (int i = l2; i <= r2; i++) A[l1][i] = applyLazy(A[l1][i], v);
            A[l1].pushup();
        } else {
            A[l1].propagate();
            for (int i = l2, en = int(A[l1].size()); i < en; i++) A[l1][i] = applyLazy(A[l1][i], v);
            A[l1].pushup();
            for (int i = l1 + 1; i < r1; i++) { A[i].d = applyLazy(A[i].d, getSegmentVal(v, int(A[i].size()))); A[i].l = mergeLazy(A[i].l, v); }
            A[r1].propagate();
            for (int i = 0; i <= r2; i++) A[r1][i] = applyLazy(A[r1][i], v);
            A[r1].pushup();
        }
    }
    void reverse(int k, int len) { // reverses the range [k, k + len)
        if (len == 0) return;
        int lo = 0, hi = int(A.size()) - 1, mid, sqrtn = sqrt(N) * SCALE_FACTOR;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int l1 = hi, l2 = k - A[hi].p; lo = 0; hi = int(A.size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k + len - 1 < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int r1 = hi, r2 = k + len - 1 - A[hi].p;
        if (l1 == r1) {
            A[l1].propagate(); int i = l2, j = r2;
            for (; i < j; i++, j--) { swap(A[l1][i], A[l1][j]); revData(A[l1][i]); revData(A[l1][j]); }
            if (i == j) revData(A[l1][i]);
            A[l1].pushup();
        } else {
            int i = l1 + 1, j = r1 - 1;
            for (; i < j; i++, j--) { A[i].swap(A[j]); A[i].reverse(); A[j].reverse(); }
            if (i == j) A[i].reverse();
            if (r2 < int(A[r1].size()) - 1) {
                A[r1].propagate(); A.emplace(A.begin() + r1 + 1, A[r1].p + r2 + 1, A[r1].begin() + r2 + 1, A[r1].end()); 
                A[r1].resize(r2 + 1); A[r1].pushup();
            }
            A[r1].reverse();
            if (0 < l2) {
                A[l1].propagate(); A.emplace(A.begin() + l1 + 1, A[l1].p + l2, A[l1].begin() + l2, A[l1].end()); 
                A[l1].resize(l2); A[l1++].pushup(); r1++;
            }
            A[l1].reverse(); A[l1].swap(A[r1]);
            if (l1 < int(A.size())) A[l1].p = l1 == 0 ? 0 : A[l1 - 1].p + int(A[l1 - 1].size());
            for (i = l1 + 1; i <= r1; i++) A[i].p = A[i - 1].p + int(A[i - 1].size());
            if (int(A.size()) > 2 * sqrtn) rebuild();
        }
    }
    Data query(int k, int len) { // queries the range [k, k + len)
        if (len == 0) return qdef;
        int lo = 0, hi = int(A.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int l1 = hi, l2 = k - A[hi].p;
        lo = 0; hi = int(A.size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k + len - 1 < A[mid].p) hi = mid - 1;
            else lo = mid + 1;
        }
        int r1 = hi, r2 = k + len - 1 - A[hi].p;
        if (l1 == r1) {
            Data ret = qdef; A[l1].propagate();
            for (int i = l2; i <= r2; i++) ret = merge(ret, A[l1][i]);
            return ret;
        } else {
            Data ret = qdef; A[l1].propagate();
            for (int i = l2, en = int(A[l1].size()); i < en; i++) ret = merge(ret, A[l1][i]);
            for (int i = l1 + 1; i < r1; i++) ret = merge(ret, A[i].d);
            A[r1].propagate();
            for (int i = 0; i <= r2; i++) ret = merge(ret, A[r1][i]);
            return ret;
        }
    }
    vector<Data> values() {
        vector<Data> ret;
        for (auto &&a : A) { a.propagate(); for (auto &&x : a) ret.push_back(x); }
        return ret;
    }
    int size() { return N; }
};
