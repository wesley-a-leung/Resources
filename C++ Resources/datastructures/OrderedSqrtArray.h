#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Decomposes the array into blocks of size sqrt(n) multiplied by a factor
// The factor should be between 1 and 10, and should be smaller for large N
// Time Complexity:
//   constructor: O(N)
//   insert, erase: O(sqrt(N) + log(N))
//   pop_front: O(sqrt(N))
//   pop_back: O(1) amortized
//   front, back, empty, size: O(1)
//   at, accessor, contains, floor, ceiling, above, below: O(log(N))
//   values: O(N)
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct OrderedSqrtArray {
    Comparator cmp; int n, SCALE_FACTOR; vector<vector<Value>> a; vector<int> prefixSZ;
    pair<int, int> ceiling_ind(const Value &val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo; lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        return {i, lo};
    }
    pair<int, int> floor_ind(const Value &val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi; lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid - 1;
            else lo = mid + 1;
        }
        return {i, hi};
    }
    pair<int, int> above_ind(const Value &val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo; lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        return {i, lo};
    }
    pair<int, int> below_ind(const Value &val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi; lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid - 1;
        }
        return {i, hi};
    }
    OrderedSqrtArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    template <typename It> OrderedSqrtArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); assert(is_sorted(st, en, cmp));
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (It i = st; i < en; i += sqrtn) { a.emplace_back(i, min(i + sqrtn, en)); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    OrderedSqrtArray(initializer_list<Value> il, const int SCALE_FACTOR = 1) : n(il.end() - il.begin()), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); assert(is_sorted(il.begin(), il.end(), cmp));
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (auto i = il.begin(); i < il.end(); i += sqrtn) { a.emplace_back(i, min(i + sqrtn, il.end())); prefixSZ.push_back(0); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void insert(const Value &val) {
        pair<int, int> i = above_ind(val);
        if (n++ == 0) { a.emplace_back(); prefixSZ.push_back(0); }
        if (i.first == (int) a.size()) a[--i.first].push_back(val);
        else a[i.first].insert(a[i.first].begin() + i.second, val);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a[i.first].size() > 2 * sqrtn) {
            a.emplace(a.begin() + i.first + 1, a[i.first].begin() + sqrtn, a[i.first].end()); a[i.first].resize(sqrtn); prefixSZ.push_back(0);
        }
        for (int j = i.first + 1; j < (int) a.size(); j++) prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
    }
    bool erase(const Value &val) {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return false;
        --n; a[i.first].erase(a[i.first].begin() + i.second);
        if (a[i.first].empty()) { a.erase(a.begin() + i.first); prefixSZ.pop_back(); }
        for (int j = i.first + 1; j < (int) a.size(); j++) prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        return true;
    }
    void pop_front() {
        assert(n > 0); --n; a.front().erase(a.front().begin());
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
        return a[hi][k - prefixSZ[hi]];
    }
    const Value &operator [](const int k) const { return at(k); }
    const Value &front() const { assert(n > 0); return a.front().front(); }
    const Value &back() const { assert(n > 0); return a.back().back(); }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    bool contains(const Value &val) const {
        pair<int, int> i = ceiling_ind(val);
        return i.first != (int) a.size() && a[i.first][i.second] == val;
    }
    pair<int, Value> floor(const Value &val) const {
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }
    pair<int, Value> ceiling(const Value &val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }
    pair<int, Value> above(const Value &val) const {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }
    pair<int, Value> below(const Value &val) const {
        pair<int, int> i = below_ind(val);
        if (i.first == -1) throw no_such_element_exception("call to below() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }
    vector<Value> values() const {
        vector<Value> ret;
        for (auto &&ai : a) for (auto &&aij : ai) ret.push_back(aij);
        return ret;
    }
    void clear() { n = 0; a.clear(); prefixSZ.clear(); }
};
