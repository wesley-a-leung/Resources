#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Decomposes the array into N ^ (1 / R) containers of size N ^ ((R - 1) / R) multiplied by a factor
// The factor should be between 1 and 10, and should be smaller for large N
// Time Complexity:
//   constructor: O(N)
//   insert, emplace, erase: O(R * (N ^ (1 / R)) + log(N)) amortized
//   push_front, pop_front, emplace_front: O(R * (N ^ (1 / R))) amortized
//   push_back, pop_back, emplace_back: O(1) amortized
//   front, back, empty, size: O(1)
//   at, accessor: O(log(N))
//   values: O(N)
// Memory Complexity: O(N)
template <const int R, class Value, class Container> struct RootArray {
    int n, SCALE_FACTOR; vector<Container> a; vector<int> prefixSZ;
    RootArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    RootArray(const int n, const int SCALE_FACTOR) : n(n), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (int i = 0; i < n; i += rootn) { a.emplace_back(min(rootn, n - i)); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    template <class It> RootArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (It i = st; i < en; i += rootn) { a.emplace_back(i, min(i + rootn, st + n), SCALE_FACTOR); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    RootArray(initializer_list<Value> il, const int SCALE_FACTOR = 1) : n(il.end() - il.begin()), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (auto i = il.begin(); i < il.end(); i += rootn) { a.emplace_back(i, min(i + rootn, il.end()), SCALE_FACTOR); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void insert(int k, const Value &val) { // inserts value before kth index
        assert(0 <= k && k <= n);
        if (n++ == 0) { a.emplace_back(SCALE_FACTOR); prefixSZ.push_back(0); }
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        k -= prefixSZ[hi]; int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR; a[hi].insert(k, val);
        if ((int) a[hi].size() > 2 * rootn) {
            vector<Value> b;
            while (a[hi].size() > rootn) { b.push_back(a[hi].back()); a[hi].pop_back(); }
            reverse(b.begin(), b.end()); a.emplace(a.begin() + hi + 1, b.begin(), b.end(), SCALE_FACTOR); prefixSZ.push_back(0);
        }
        for (int i = hi + 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void push_front(const Value &val) {
        if (n++ == 0) { a.emplace_back(SCALE_FACTOR); prefixSZ.push_back(0); }
        a.front().push_front(val);
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        if ((int) a.front().size() > 2 * rootn) {
            vector<Value> b;
            while (a.front().size() > rootn) { b.push_back(a.front().back()); a.front().pop_back(); }
            reverse(b.begin(), b.end()); a.emplace(a.begin() + 1, b.begin(), b.end(), SCALE_FACTOR); prefixSZ.push_back(0);
        }
    }
    void push_back(const Value &val) {
        if (n++ == 0) { a.emplace_back(SCALE_FACTOR); prefixSZ.push_back(0); }
        a.back().push_back(val);
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        if ((int) a.back().size() > 2 * rootn) {
            vector<Value> b;
            while (a.back().size() > rootn) { b.push_back(a.back().back()); a.back().pop_back(); }
            reverse(b.begin(), b.end()); a.emplace_back(b.begin(), b.end(), SCALE_FACTOR);
            prefixSZ.push_back(prefixSZ[(int) a.size() - 2] + (int) a[(int) a.size() - 2].size());
        }
    }
    template <class ...Args> void emplace(int k, Args &&...args) { insert(k, T(forward<Args>(args)...)); }
    template <class ...Args> void emplace_front(Args &&...args) { push_front(T(forward<Args>(args)...)); }
    template <class ...Args> void emplace_back(Args &&...args) { push_back(T(forward<Args>(args)...)); }
    void erase(const int k) {
        assert(0 <= k && k < n); --n; int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        a[hi].erase(k - prefixSZ[hi]);
        if (a[hi].empty()) { a.erase(a.begin() + hi); prefixSZ.pop_back(); }
        for (int i = hi + 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void pop_front() {
        assert(n > 0); --n; a.front().pop_front();
        if (a.front().empty()) { a.erase(a.begin()); prefixSZ.pop_back(); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void pop_back() {
        assert(n > 0); --n; a.back().pop_back();
        if (a.back().empty()) { a.pop_back(); prefixSZ.pop_back(); }
    }
    const Value &at(const int k) const {
        assert(0 <= k && k < n); int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi].at(k - prefixSZ[hi]);
    }
    Value &at(const int k) {
        assert(0 <= k && k < n); int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi].at(k - prefixSZ[hi]);
    }
    const Value &operator [](const int k) const { return at(k); }
    Value &operator [](const int k) { return at(k); }
    const Value &front() const { assert(n > 0); return a.front().front(); }
    Value &front() { assert(n > 0); return a.front().front(); }
    const Value &back() const { assert(n > 0); return a.back().back(); }
    Value &back() { assert(n > 0); return a.back().back(); }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    vector<Value> values() const {
        vector<Value> ret;
        for (auto &&ai : a) for (auto &&aij : ai.values()) ret.push_back(aij);
        return ret;
    }
    void clear() { n = 0; a.clear(); prefixSZ.clear(); }
};
