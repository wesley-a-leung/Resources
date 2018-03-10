#ifndef DATASTRUCTURES_SQRTARRAY_H_
#define DATASTRUCTURES_SQRTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Sqrt Array:
 * Decomposes into blocks of size sqrt(n).
 *
 * Insert: O(sqrt(N))
 * Erase: O(sqrt(N))
 * At, Accessor: O(log(N))
 * Lower Bound, Upper Bound, Floor, Ceiling: O(log(N))
 * Values: O(N)
 * Size: O(1)
 */
template <typename Value, typename Comparator = less<Value>>
struct SqrtArray {
private:
    Comparator cmp;
    int n;
    vector<vector<Value>> a;
    vector<int> prefixSZ;

public:
    SqrtArray() : n(0) {}

    SqrtArray(const int n) : n(n) {
        assert(n >= 0);
        int sqrtn = (int) sqrt(n);
        for (int i = n; i > 0; i -= sqrtn) {
            a.push_back(vector<Value>(min(i, sqrtn)));
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    template <typename It>
    SqrtArray(const It st, const It en) {
        n = en - st;
        assert(n >= 0);
        int sqrtn = (int) sqrt(n);
        for (It i = en; i > st; i -= sqrtn) {
            a.push_back(vector<Value>(i - min((int) (i - st), sqrtn), i));
            prefixSZ.push_back(0);
        }
        reverse(a.begin(), a.end());
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    void insert(int k, const Value val) {
        assert(0 <= k && k <= n);
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
        k -= prefixSZ[hi];
        if (hi == -1) a[hi += (int) a.size()].push_back(val);
        else a[hi].insert(a[hi].begin() + k, val);
        int sqrtn = (int) sqrt(n);
        if ((int) a[hi].size() > 2 * sqrtn) {
            vector<int> y(a[hi].begin() + sqrtn, a[hi].end());
            a[hi].resize(sqrtn);
            a.insert(a.begin() + hi + 1, y);
            prefixSZ.push_back(0);
        }
        for (int i = hi + 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    void erase(int k) {
        assert(0 <= k && k < n);
        --n;
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        k -= prefixSZ[hi];
        a[hi].erase(a[hi].begin() + k);
        if (a[hi].empty()) {
            a.erase(a.begin() + hi);
            prefixSZ.pop_back();
        }
        for (int i = hi + 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    Value &operator [](int k) {
        assert(0 <= k && k < n);
        int lo = 0, hi = (int) (a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }

    Value at(int k) const {
        assert(0 <= k && k < n);
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

    bool isEmpty() const {
        return n == 0;
    }

    pair<int, Value> lower_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
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
        if (lo == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    pair<int, Value> floor(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid - 1;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + hi, a[i][hi]};
    }

    pair<int, Value> ceiling(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    vector<Value> values() const {
        vector<Value> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                ret.push_back(a[i][j]);
            }
        }
        return ret;
    }

    int size() const {
        return n;
    }
};

#endif /* DATASTRUCTURES_SQRTARRAY_H_ */
