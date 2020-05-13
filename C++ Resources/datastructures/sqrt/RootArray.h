#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes the array recursively into N ^ (1 / R) containers of size N ^ ((R - 1) / R) multiplied by a scale factor
// insert, erase, below, floor, ceiling, above, and find require the data to be sorted by the comparator
// insert inserts before the first index i where cmp(at(i), val) is false
// erase erases the first index i where cmp(at(i), val) and cmp(val, at(i)) are false, if it exists
// insert_at inserts before index k
// below returns the last element x where cmp(x, val) is true
// floor returns the last element x where cmp(val, x) is false
// ceiling returns the first element x where cmp(x, val) is false
// above returns the first element x where cmp(val, x) is true
// find returns the first element x where cmp(val, x) and cmp(x, val) are false, if it exists
// All other operations work regardles of whether it is sorted
// Default comparator is a simple pointer comparator
// In practice, has a small constant, and is faster than balanced binary search trees when R = 3, and SCALE = 6, even for N >= 1e7
// Time Complexity:
//   constructor: O(N)
//   insert, insert_at, erase, erase_at, push_front, pop_front, at: O(R * (I ^ (1 / R))) where I is the total number of insertions
//   below, floor, ceiling, above, find: O(R * (I ^ (1 / R))) where I is the total number of insertions
//   front, back, empty, size, pop_back: O(1)
//   push_back: O(1) amortized
//   values, clear: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4
//   https://dmoj.ca/problem/cco10p3
//   https://dmoj.ca/problem/ccc05s5
template <const int R, class T> struct RootArray {
    static_assert(R > 0, "R must be positive"); int N; vector<RootArray<R - 1, T>> A; double SCALE;
    int getRootN() {
        if (N == 0) return 0;
        int lg = __lg(N); lg -= lg / R; return SCALE * (1 << lg);
    }
    RootArray(double SCALE = 6) : N(0), SCALE(SCALE) { assert(SCALE > 0); }
    template <class It> RootArray(const It st, const It en, double SCALE = 6) : N(en - st), SCALE(SCALE) {
        assert(N >= 0); assert(SCALE > 0);
        if (N == 0) return;
        int rootN = getRootN(); A.reserve((N - 1) / rootN + 1);
        for (It i = st; i < en; i += rootN) A.emplace_back(i, min(i + rootN, en), SCALE);
    }
    void split(int i) {
        int rootN = getRootN();
        if (int(A[i].size()) > 2 * rootN) {
            vector<T> tmp; tmp.reserve(int(A[i].size()) - 2 * rootN);
            while (int(A[i].size()) > rootN) { tmp.push_back(A[i].back()); A[i].pop_back(); }
            A.emplace(A.begin() + i + 1, tmp.rbegin(), tmp.rend(), SCALE);
        }
    }
    template <class Comp> void insert(const T &val, Comp cmp) {
        if (N++ == 0) { A.emplace_back(SCALE); A.back().insert(val, cmp); return; }
        int i = 0;
        while (i < int(A.size()) && cmp(A[i].back(), val)) i++;
        if (i >= int(A.size())) i = int(A.size()) - 1;
        A[i].insert(val, cmp); split(i);
    }
    void insert_at(int k, const T &val) {
        assert(0 <= k && k <= N);
        if (k == N) { push_back(val); return; }
        N++; int i = 0;
        while (int(A[i].size()) <= k) k -= int(A[i++].size());
        A[i].insert_at(k, val); split(i);
    }
    template <class Comp> bool erase(const T &val, Comp cmp) {
        int i = 0;
        while (i < int(A.size()) && cmp(A[i].back(), val)) i++;
        if (i >= int(A.size()) || !A[i].erase(val, cmp)) return false;
        if (A[i].empty()) A.erase(A.begin() + i);
        N--; return true;
    }
    void erase_at(int k) {
        assert(0 <= k && k < N); int i = 0;
        while (int(A[i].size()) <= k) k -= int(A[i++].size());
        N--; A[i].erase_at(k);
        if (A[i].empty()) A.erase(A.begin() + i);
    }
    int size() const { return N; }
    bool empty() const { return N == 0; }
    const T &front() const { assert(N > 0); return A.front().front(); }
    T &front() { assert(N > 0); return A.front().front(); }
    const T &back() const { assert(N > 0); return A.back().back(); }
    T &back() { assert(N > 0); return A.back().back(); }
    void push_front(const T &val) {
        if (N++ == 0) { A.emplace_back(SCALE); A.back().push_back(val); return; }
        A.front().push_front(val); split(0);
    }
    void push_back(const T &val) {
        if (N++ == 0) { A.emplace_back(SCALE); A.back().push_back(val); return; }
        A.back().push_back(val); split(int(A.size()) - 1);
    }
    void pop_front() {
        assert(N > 0); N--; A.front().pop_front();
        if (A.front().empty()) A.erase(A.begin());
    }
    void pop_back() {
        assert(N > 0); N--; A.back().pop_back();
        if (A.back().empty()) A.pop_back();
    }
    const T &at(int k) const {
        assert(0 <= k && k < N); int i = 0;
        while (int(A[i].size()) <= k) k -= int(A[i++].size());
        return A[i].at(k);
    }
    T &at(int k) {
        assert(0 <= k && k < N); int i = 0;
        while (int(A[i].size()) <= k) k -= int(A[i++].size());
        return A[i].at(k);
    }
    template <class Comp> pair<int, T *> below(const T &val, Comp cmp) {
        int i = 0, k = 0;
        while (i < int(A.size()) && cmp(A[i].front(), val)) k += int(A[i++].size());
        if (--i >= 0) k -= int(A[i].size());
        else return make_pair(-1, nullptr);
        pair<int, T *> ret = A[i].below(val, cmp); ret.first += k; return ret;
    }
    template <class Comp> pair<int, T *> floor(const T &val, Comp cmp) {
        int i = 0, k = 0;
        while (i < int(A.size()) && !cmp(val, A[i].front())) k += int(A[i++].size());
        if (--i >= 0) k -= int(A[i].size());
        else return make_pair(-1, nullptr);
        pair<int, T *> ret = A[i].floor(val, cmp); ret.first += k; return ret;
    }
    template <class Comp> pair<int, T *> ceiling(const T &val, Comp cmp) {
        int i = 0, k = 0;
        while (i < int(A.size()) && cmp(A[i].back(), val)) k += int(A[i++].size());
        if (i >= int(A.size())) return make_pair(N, nullptr);
        pair<int, T *> ret = A[i].ceiling(val, cmp); ret.first += k; return ret;
    }
    template <class Comp> pair<int, T *> above(const T &val, Comp cmp) {
        int i = 0, k = 0;
        while (i < int(A.size()) && !cmp(val, A[i].back())) k += int(A[i++].size());
        if (i >= int(A.size())) return make_pair(N, nullptr);
        pair<int, T *> ret = A[i].above(val, cmp); ret.first += k; return ret;
    }
    template <class Comp> pair<int, T *> find(const T &val, Comp cmp) {
        pair<int, T *> ret = ceiling(val, cmp);
        if (ret.second == nullptr || cmp(val, *(ret.second)) || cmp(*(ret.second), val)) return make_pair(size(), nullptr);
        return ret;
    }
    vector<T> values() const {
        vector<T> ret; ret.reserve(N);
        for (auto &&ai : A) for (auto &&aij : ai.values()) ret.push_back(aij);
        return ret;
    }
    void clear() { N = 0; A.clear(); }
};

template <class T> struct RootArray<1, T> : public vector<T> {
    using vector<T>::begin; using vector<T>::end; using vector<T>::size; using vector<T>::at; RootArray(double = 6) {}
    template <class It> RootArray(const It st, const It en, double = 6) : vector<T>(st, en) {}
    template <class Comp> void insert(const T &val, Comp cmp) { vector<T>::insert(lower_bound(begin(), end(), val, cmp), val); }
    void insert_at(int k, const T &val) { vector<T>::insert(begin() + k, val); }
    template <class Comp> bool erase(const T &val, Comp cmp) { 
        auto it = lower_bound(begin(), end(), val, cmp);
        if (it == end() || cmp(*it, val) || cmp(val, *it)) return false;
        vector<T>::erase(it); return true;
    }
    void erase_at(int k) { vector<T>::erase(begin() + k); }
    void push_front(const T &val) { vector<T>::insert(begin(), val); }
    void pop_front() { vector<T>::erase(begin()); }
    template <class Comp> pair<int, T *> below(const T &val, Comp cmp) {
        int i = lower_bound(begin(), end(), val, cmp) - begin() - 1; return make_pair(i, i < 0 ? nullptr : &at(i));
    }
    template <class Comp> pair<int, T *> floor(const T &val, Comp cmp) {
        int i = upper_bound(begin(), end(), val, cmp) - begin() - 1; return make_pair(i, i < 0 ? nullptr : &at(i));
    }
    template <class Comp> pair<int, T *> ceiling(const T &val, Comp cmp) {
        int i = lower_bound(begin(), end(), val, cmp) - begin(); return make_pair(i, i >= int(size()) ? nullptr : &at(i));
    }
    template <class Comp> pair<int, T *> above(const T &val, Comp cmp) {
        int i = upper_bound(begin(), end(), val, cmp) - begin(); return make_pair(i, i >= int(size()) ? nullptr : &at(i));
    }
    template <class Comp> pair<int, T *> find(const T &val, Comp cmp) {
        pair<int, T *> ret = ceiling(val, cmp);
        if (ret.second == nullptr || cmp(val, *(ret.second)) || cmp(*(ret.second), val)) return make_pair(size(), nullptr);
        return ret;
    }
    const RootArray<1, T> &values() const { return *this; }
};
