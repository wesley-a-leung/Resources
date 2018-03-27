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
 * Decomposes the array into blocks of size sqrt(n) multiplied by a factor.
 * The factor should be between 1 and 10, and should be smaller for large N.
 *
 * Usage:
 * SqrtArray<int> arr;
 *
 * Initializing: O(N)
 * Insert: O(sqrt(N) + log(N))
 * Erase: O(sqrt(N) + log(N))
 * Push Front, Pop Front: O(sqrt(N))
 * Push Back, Pop Back: O(1) amortized
 * At, Accessor, Mutator: O(log(N))
 * Front, Back: O(1)
 * Lower Bound, Upper Bound, Floor, Ceiling, Above, Below: O(log(N))
 * Empty, Size: O(1)
 * Values: O(N)
 */
template <typename Value>
struct SqrtArray {
private:
    int n; // the size of the array
    int SCALE_FACTOR; // the scale factor of sqrt(n)
    vector<vector<Value>> a; // the array
    vector<int> prefixSZ; // the prefix array of the sizes of the blocks

public:
    /**
     * Initializes an empty structure.
     * @param SCALE_FACTOR scales the value of sqrt(n) by this value
     */
    SqrtArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}

    /**
     * Initializes the structure with an initial size.
     * @param SCALE_FACTOR scales the value of sqrt(n) by this value
     *
     * @param n the initial size
     */
    SqrtArray(const int n, const int SCALE_FACTOR = 1) : n(n), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (int i = 0; i < n; i += sqrtn) {
            a.push_back(vector<Value>(min(sqrtn, n - i)));
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     * @param SCALE_FACTOR scales the value of sqrt(n) by this value
     */
    template <typename It>
    SqrtArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (It i = st; i < en; i += sqrtn) {
            a.emplace_back(i, min(i + sqrtn, en));
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Inserts a value before the current kth index of the structure. If k == n,
     * then the value is inserted at the end of the structure.
     *
     * @param k the 0-based index to insert before, 0 <= k <= n
     * @param val the value to be inserted
     */
    void insert(int k, const Value val) {
        assert(0 <= k && k <= n);
        if (n++ == 0) {
            a.emplace_back();
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
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a[hi].size() > 2 * sqrtn) {
            a.emplace(a.begin() + hi + 1, a[hi].begin() + sqrtn, a[hi].end());
            a[hi].resize(sqrtn);
            prefixSZ.push_back(0);
        }
        for (int i = hi + 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Inserts a value at the beginning of the structure.
     * @param val the value to be inserted
     */
    void push_front(const Value val) {
        if (n++ == 0) {
            a.emplace_back();
            prefixSZ.push_back(0);
        }
        a.front().insert(a.front().begin(), val);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a.front().size() > 2 * sqrtn) {
            a.emplace(a.begin() + 1, a.front().begin() + sqrtn, a.front().end());
            a.front().resize(sqrtn);
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Inserts a value at the end of the structure.
     * @param val the value to be inserted
     */
    void push_back(const Value val) {
        if (n++ == 0) {
            a.emplace_back();
            prefixSZ.push_back(0);
        }
        a.back().push_back(val);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a.back().size() > 2 * sqrtn) {
            a.emplace_back(a.back().begin() + sqrtn, a.back().end());
            a[(int) a.size() - 2].resize(sqrtn);
            prefixSZ.push_back(prefixSZ[(int) a.size() - 2] + (int) a[(int) a.size() - 2].size());
        }
    }

    /**
     * Erases the kth value in the structure.
     *
     * @param k the value to erase, 0 <= k < n
     */
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

    /**
     * Erases the first element in the structure.
     */
    void pop_front() {
        assert(n > 0);
        --n;
        a.front().erase(a.front().begin());
        if (a.front().empty()) {
            a.erase(a.begin());
            prefixSZ.pop_back();
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Erases the last element in the structure.
     */
    void pop_back() {
        assert(n > 0);
        --n;
        a.back().pop_back();
        if (a.back().empty()) {
            a.pop_back();
            prefixSZ.pop_back();
        }
    }

    /**
     * Mutator operator.
     *
     * @param k the 0-based index
     * @return a reference to the kth element in the structure
     */
    Value &operator [](int k) {
        assert(0 <= k && k < n);
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }

    /**
     * Returns a constant reference to the kth value in the structure.
     *
     * @param k the 0-based index
     * @return a constant reference to the kth value in the structure
     */
    const Value &at(const int k) const {
        assert(0 <= k && k < n);
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi][k - prefixSZ[hi]];
    }

    /**
     * Accessor operator.
     * Returns a constant reference to the kth value in the structure.
     *
     * @param k the 0-based index
     * @return a constant reference to the kth value in the structure
     */
    const Value &operator [](const int k) const {
        return at(k);
    }

    /**
     * Returns a constant reference to the first element.
     * @return a constant reference to the first element
     */
    const Value &front() const {
        assert(n > 0);
        return a.front().front();
    }

    /**
     * Returns a constant reference to the last element.
     * @return a constant reference to the last element
     */
    const Value &back() const {
        assert(n > 0);
        return a.back().back();
    }

    /**
     * Checks if the structure is empty.
     *
     * @return true if the structure is empty, false otherwise
     */
    bool empty() const {
        return n == 0;
    }

    /**
     * Returns the number of values in the structure.
     *
     * @return the number of values in the structure
     */
    int size() const {
        return n;
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val by the < operator. Identical to ceiling.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is larger than the largest value
     * in the structure
     */
    pair<int, Value> lower_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].back() < val) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val based on the comparator. Identical to ceiling.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is larger than the largest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> lower_bound(const Value val, Comparator cmp) const {
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

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val by the < operator. Identical to above.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> upper_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].back()) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val based on the comparator. Identical to above.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    template <typename Comparator> pair<int, Value> upper_bound(const Value val, Comparator cmp) const {
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

    /**
     * Returns a pair containing the index and value of the largest value
     * less than or equal to val by the < operator.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than or equal to val
     * @throws no_such_element_exception if val is less than the smallest value
     * in the structure
     */
    pair<int, Value> floor(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].front()) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + hi, a[i][hi]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than or equal to val based on the comparator.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the largest value
     * less than or equal to val
     * @throws no_such_element_exception if val is less than the smallest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> floor(const Value val, Comparator cmp) const {
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

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val by the < operator. Identical to lower_bound.
     *
     * @pre the structure must be sorted
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    pair<int, Value> ceiling(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].back() < val) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val based on the comparator. Identical to lower_bound.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> ceiling(const Value val, Comparator cmp) const {
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

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val by the < operator. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> above(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].back()) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val based on the comparator. Identical to upper_bound.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    template <typename Comparator> pair<int, Value> above(const Value val, Comparator cmp) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        return {prefixSZ[i] + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than val by the < operator.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than val
     * @throws no_such_element_exception if val is less than or equal to the smallest value
     * in the structure
     */
    pair<int, Value> below(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].front() < val) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) throw no_such_element_exception("call to below() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) lo = mid + 1;
            else hi = mid - 1;
        }
        return {prefixSZ[i] + hi, a[i][hi]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than val based on the comparator.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the largest value
     * less than val
     * @throws no_such_element_exception if val is less than or equal to the smallest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> below(const Value val, Comparator cmp) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) throw no_such_element_exception("call to below() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) lo = mid + 1;
            else hi = mid - 1;
        }
        return {prefixSZ[i] + hi, a[i][hi]};
    }

    /**
     * Returns all values in the structure.
     *
     * @return a vector containing all values in the structure
     */
    vector<Value> values() const {
        vector<Value> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                ret.push_back(a[i][j]);
            }
        }
        return ret;
    }

    /**
     * Sorts the structure by creating an auxiliary array.
     */
    void sort() {
        vector<Value> b;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                b.push_back(a[i][j]);
            }
        }
        std::sort(b.begin(), b.end());
        int k = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                a[i][j] = b[k++];
            }
        }
    }

    /**
     * Sorts the structure with a comparator by creating an auxiliary array.
     * @param cmp the comparator
     */
    template <typename Comparator> void sort(Comparator cmp) {
        vector<Value> b;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                b.push_back(a[i][j]);
            }
        }
        std::sort(b.begin(), b.end(), cmp);
        int k = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                a[i][j] = b[k++];
            }
        }
    }
};

#endif /* DATASTRUCTURES_SQRTARRAY_H_ */
