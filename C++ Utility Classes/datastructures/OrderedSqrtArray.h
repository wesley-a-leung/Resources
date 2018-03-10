#ifndef DATASTRUCTURES_ORDEREDSQRTARRAY_H_
#define DATASTRUCTURES_ORDEREDSQRTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Ordered Sqrt Array:
 * Decomposes into blocks of size sqrt(n).
 *
 * Insert: O(sqrt(N))
 * Erase: O(sqrt(N))
 * Select, Accessor: O(log(N))
 * Rank, Contains: O(log(N))
 * Lower Bound, Upper Bound, Floor, Ceiling: O(log(N))
 * isEmpty, Size: O(1)
 */
template <typename Value, typename Comparator = less<Value>>
struct OrderedSqrtArray {
private:
    Comparator cmp;
    int n;
    vector<vector<Value>> a;
    vector<int> prefixSZ;

    pair<int, int> lower_bound_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        return {i, lo};
    }

    pair<int, int> upper_bound_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        return {i, lo};
    }

    pair<int, int> floor_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid - 1;
            else lo = mid + 1;
        }
        return {i, hi};
    }

public:
    OrderedSqrtArray() : n(0) {}

    OrderedSqrtArray(const int n) : n(n) {
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
    OrderedSqrtArray(const It st, const It en) {
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

    void insert(const Value val) {
        pair<int, int> i = upper_bound_ind(val);
        if (n++ == 0) {
            a.push_back({});
            prefixSZ.push_back(0);
        }
        if (i.first == (int) a.size()) a[--i.first].push_back(val);
        else a[i.first].insert(a[i.first].begin() + i.second, val);
        int sqrtn = (int) sqrt(n);
        if ((int) a[i.first].size() > 2 * sqrtn) {
            vector<int> y(a[i.first].begin() + sqrtn, a[i.first].end());
            a[i.first].resize(sqrtn);
            a.insert(a.begin() + i.first + 1, y);
            prefixSZ.push_back(0);
        }
        for (int j = i.first + 1; j < (int) a.size(); j++) {
            prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        }
    }

    bool erase(const Value val) {
        pair<int, int> i = lower_bound_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return false;
        --n;
        a[i.first].erase(a[i.first].begin() + i.second);
        if (a[i.first].empty()) {
            a.erase(a.begin() + i.first);
            prefixSZ.pop_back();
        }
        for (int j = i.first + 1; j < (int) a.size(); j++) {
            prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        }
        return true;
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

    Value select(int k) const {
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
        return select(k);
    }

    int getRank(const Value val) const {
        pair<int, int> i = lower_bound_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return -1;
        return prefixSZ[i.first] + i.second;
    }

    bool isEmpty() const {
        return n == 0;
    }

    bool contains(const Value val) const {
        pair<int, int> i = lower_bound_ind(val);
        return i.first != (int) a.size() && a[i.first][i.second] == val;
    }

    pair<int, Value> lower_bound(const Value val) const {
        pair<int, int> i = lower_bound_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }

    pair<int, Value> upper_bound(const Value val) const {
        pair<int, int> i = upper_bound_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }

    pair<int, Value> floor(const Value val) const {
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
    }

    pair<int, Value> ceiling(const Value val) const {
        pair<int, int> i = lower_bound_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {prefixSZ[i.first] + i.second, a[i.first][i.second]};
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

#endif /* DATASTRUCTURES_ORDEREDSQRTARRAY_H_ */
