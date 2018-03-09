#ifndef DATASTRUCTURES_SQRTARRAY_H_
#define DATASTRUCTURES_SQRTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Sqrt Array:
 * Decomposes into blocks of size sqrt(n).
 *
 * Insert: O(sqrt(N))
 * Erase: O(sqrt(N))
 * At, Accessor: O(log(N))
 * Lower Bound, Upper Bound: O(log(N))
 * Size: O(1)
 */
template <typename Value, typename Comparator = less<Value>>
struct SqrtArray {
private:
    Comparator cmp;
    int n;
    vector<vector<Value>> a;
    vector<int> prefixSZ;

    void updateSZ() {
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

public:
    SqrtArray() : n(0) {}

    SqrtArray(const int n) : n(n) {
        int sqrtn = (int) sqrt(n);
        for (int i = n; i > 0; i -= sqrtn) {
            a.push_back(vector<Value>(min(i, sqrtn)));
            prefixSZ.push_back(0);
        }
        updateSZ();
    }

    template <typename It>
    SqrtArray(const It st, const It en) {
        n = en - st;
        int sqrtn = (int) sqrt(n);
        for (It i = en; i > st; i -= sqrtn) {
            a.push_back(vector<Value>(i - min((int) (i - st), sqrtn), i));
            prefixSZ.push_back(0);
        }
        updateSZ();
        reverse(a.begin(), a.end());
    }

    void insert(int k, const Value val) {
        if (n++ == 0) {
            a.push_back({});
            prefixSZ.push_back(0);
        }
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        int i = hi;
        k -= prefixSZ[hi];
        if (i == -1) a[i += (int) a.size()].push_back(val);
        else a[i].insert(a[i].begin() + k, val);
        int sqrtn = (int) sqrt(n);
        if ((int) a[i].size() > 2 * sqrtn) {
            vector<int> y(a[i].begin() + sqrtn, a[i].end());
            a[i].resize(sqrtn);
            a.insert(a.begin() + i + 1, y);
            prefixSZ.push_back(0);
        }
        updateSZ();
    }

    void erase(int k) {
        --n;
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        int i = hi;
        k -= prefixSZ[hi];
        a[i].erase(a[i].begin() + k);
        if (a[i].empty()) {
            a.erase(a.begin() + i);
            prefixSZ.pop_back();
        }
        updateSZ();
    }

    Value &operator [](int k) {
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }

    Value at(int k) const {
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }

    Value operator [](int k) const {
        return at(k);
    }

    pair<int, Value> lower_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) return {n, 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    pair<int, Value> upper_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) return {n, 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    int size() const {
        return n;
    }
};

#endif /* DATASTRUCTURES_SQRTARRAY_H_ */
