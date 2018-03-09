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
 * At: O(sqrt(N))
 * Lower Bound, Upper Bound: O(sqrt(N))
 * Size: O(1)
 */
template <typename Value, typename Comparator = less<Value>>
struct SqrtArray {
private:
    Comparator cmp;
    int n;
    vector<vector<Value>> a;

public:
    SqrtArray() : n(0) {}

    SqrtArray(const int n) : n(n) {
        int sqrtn = (int) sqrt(n);
        for (int i = n; i > 0; i -= sqrtn) {
            a.push_back(vector<Value>(min(i, sqrtn)));
        }
    }

    template <typename It>
    SqrtArray(const It st, const It en) {
        n = en - st;
        int sqrtn = (int) sqrt(n);
        for (It i = en; i > st; i -= sqrtn) {
            a.push_back(vector<Value>(i - min((int) (i - st), sqrtn), i));
        }
        reverse(a.begin(), a.end());
    }

    void insert(int k, const Value val) {
        if (n++ == 0) a.push_back({});
        int i;
        for (i = 0; i < (int) a.size() && k >= (int) a[i].size(); k -= (int) a[i++].size());
        if (i == (int) a.size()) a[--i].push_back(val);
        else a[i].insert(a[i].begin() + k, val);
        int sqrtn = (int) sqrt(n);
        if ((int) a[i].size() > 2 * sqrtn) {
            vector<Value> y(a[i].begin() + sqrtn, a[i].end());
            a[i].resize(sqrtn);
            a.insert(a.begin() + i + 1, y);
        }
    }

    void erase(int k) {
        --n;
        int i;
        for (i = 0; k >= (int) a[i].size(); k -= (int) a[i++].size());
        a[i].erase(a[i].begin() + k);
        if (a[i].empty()) a.erase(a.begin() + i);
    }

    Value &operator [](int k) {
        int i;
        for (i = 0; k >= (int) a[i].size(); k -= (int) a[i++].size());
        return a[i][k];
    }

    Value at(int k) const {
        int i;
        for (i = 0; k >= (int) a[i].size(); k -= (int) a[i++].size());
        return a[i][k];
    }

    Value operator [](int k) const {
        return at(k);
    }

    pair<int, Value> lower_bound(const Value val) const {
        int i, j, k;
        for (i = 0, k = 0; i < (int) a.size() && cmp(a[i].back(), val); k += a[i++].size());
        if (i == (int) a.size()) return {n, 0};
        for (j = 0; cmp(a[i][j], val); j++, k++);
        return {k, a[i][j]};
    }

    pair<int, Value> upper_bound(const Value val) const {
        int i, j, k;
        for (i = 0, k = 0; i < (int) a.size() && !cmp(val, a[i].back()); k += a[i++].size());
        if (i == (int) a.size()) return {n, 0};
        for (j = 0; !cmp(val, a[i][j]); j++, k++);
        return {k, a[i][j]};
    }

    int size() const {
        return n;
    }
};

#endif /* DATASTRUCTURES_SQRTARRAY_H_ */
