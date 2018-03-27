#ifndef DATASTRUCTURES_SQRTARRAY_H_
#define DATASTRUCTURES_SQRTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

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

    // returns the 2D index of the smallest value greater than or equal to val
    pair<int, int> ceiling_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].back() < val) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid;
        }
        return {i, lo};
    }

    // returns the 2D index of the smallest value greater than or equal to val
    template <typename Comparator> pair<int, int> ceiling_ind(const Value val, Comparator cmp) const {
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

    // returns the 2D index of the smallest value greater than val
    pair<int, int> above_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].back()) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) return {(int) a.size(), 0};
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid;
            else lo = mid + 1;
        }
        return {i, lo};
    }

    // returns the 2D index of the smallest value greater than val
    template <typename Comparator> pair<int, int> above_ind(const Value val, Comparator cmp) const {
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

    // returns the 2D index of the largest value less than or equal to val
    pair<int, int> floor_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].front()) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return {i, hi};
    }

    // returns the 2D index of the largest value less than or equal to val
    template <typename Comparator> pair<int, int> floor_ind(const Value val, Comparator cmp) const {
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

    // returns the 2D index of the largest value less than val
    pair<int, int> below_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].front() < val) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid - 1;
        }
        return {i, hi};
    }

    // returns the 2D index of the largest value less than val
    template <typename Comparator> pair<int, int> below_ind(const Value val, Comparator cmp) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) return {-1, 0};
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid - 1;
        }
        return {i, hi};
    }

public:
    /**
     * Random Access Iterator; value can be made constant.
     *
     * All operators take O(1) time, except for iter += int, iter -= int,
     * iter + int, iter - int, iter[], which take O(log(N)) time.
     */
    template <typename T, typename Object>
    class iterator : public std::iterator<std::random_access_iterator_tag, Value> {
    private:
        Object &arr = 0;
        int i = 0, j = 0, k = 0;

        friend class SqrtArray;

        int getBlockIndex() const {
            return i;
        }

        int getNestedIndex() const {
            return j;
        }

        int getIndex() const {
            return k;
        }

    public:
        using size_type = int;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        iterator() = default;

        iterator(Object &arr, int i, int j, int k) : arr(arr), i(i), j(j), k(k) {}

        iterator(const iterator &other) = default;

        virtual ~iterator() = default;

        iterator &operator = (const iterator &other) = default;

        reference operator *() const {
            return arr.a[i][j];
        }

        pointer operator ->() const {
            return &(arr.a[i][j]);
        }

        reference operator [](size_type x) {
            int k = this->k + x;
            int lo = 0, hi = ((int) arr.a.size()) - 1, mid;
            while (lo <= hi) {
                mid = lo + (hi - lo) / 2;
                if (k < arr.prefixSZ[mid]) hi = mid - 1;
                else lo = mid + 1;
            }
            int i = hi;
            int j = k - arr.prefixSZ[hi];
            return arr.a[i][j];
        }

        iterator &operator ++() {
            ++k;
            if (++j == (int) arr.a[i].size()) {
                j = 0;
                i++;
            }
            return *this;
        }

        iterator operator ++(int) {
            iterator temp = *this;
            ++k;
            if (++j == (int) arr.a[i].size()) {
                j = 0;
                i++;
            }
            return temp;
        }

        iterator &operator --() {
            --k;
            if (--j == -1) {
                j = ((int) arr.a[--i].size()) - 1;
            }
            return *this;
        }

        iterator operator --(int) {
            iterator temp = *this;
            --k;
            if (--j == -1) {
                j = ((int) arr.a[--i].size()) - 1;
            }
            return temp;
        }

        iterator &operator +=(size_type x) {
            k += x;
            int lo = 0, hi = ((int) arr.a.size()) - 1, mid;
            while (lo <= hi) {
                mid = lo + (hi - lo) / 2;
                if (k < arr.prefixSZ[mid]) hi = mid - 1;
                else lo = mid + 1;
            }
            i = hi;
            j = k - arr.prefixSZ[hi];
            return *this;
        }

        iterator operator +(size_type x) const {
            int k = this->k + x;
            int lo = 0, hi = ((int) arr.a.size()) - 1, mid;
            while (lo <= hi) {
                mid = lo + (hi - lo) / 2;
                if (k < arr.prefixSZ[mid]) hi = mid - 1;
                else lo = mid + 1;
            }
            int i = hi;
            int j = k - arr.prefixSZ[hi];
            return iterator(arr, i, j, k);
        }

        friend iterator operator +(size_type x, const iterator &iter) {
            return iter + x;
        }

        iterator &operator -=(size_type x) {
            k -= x;
            int lo = 0, hi = ((int) arr.a.size()) - 1, mid;
            while (lo <= hi) {
                mid = lo + (hi - lo) / 2;
                if (k < arr.prefixSZ[mid]) hi = mid - 1;
                else lo = mid + 1;
            }
            i = hi;
            j = k - arr.prefixSZ[hi];
            return *this;
        }

        iterator operator -(size_type x) const {
            int k = this->k - x;
            int lo = 0, hi = ((int) arr.a.size()) - 1, mid;
            while (lo <= hi) {
                mid = lo + (hi - lo) / 2;
                if (k < arr.prefixSZ[mid]) hi = mid - 1;
                else lo = mid + 1;
            }
            int i = hi;
            int j = k - arr.prefixSZ[hi];
            return iterator(arr, i, j, k);
        }

        difference_type operator -(const iterator &other) const {
            return k - other.k;
        }

        bool operator == (const iterator &other) const {
            return k == other.k;
        }

        bool operator != (const iterator &other) const {
            return k != other.k;
        }

        bool operator < (const iterator &other) const {
            return k < other.k;
        }

        bool operator <= (const iterator &other) const {
            return k <= other.k;
        }

        bool operator > (const iterator &other) const {
            return k > other.k;
        }

        bool operator >= (const iterator &other) const {
            return k >= other.k;
        }
    };

    using const_forward_iterator = iterator<const Value, const SqrtArray<Value>>;
    using forward_iterator = iterator<Value, SqrtArray<Value>>;
    using const_reverse_iterator = reverse_iterator<forward_iterator>;
    using reverse_iterator = reverse_iterator<forward_iterator>;

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

    SqrtArray(const SqrtArray &other) = default;

    SqrtArray(SqrtArray &&other) = default;

    virtual ~SqrtArray() = default;

    SqrtArray &operator = (const SqrtArray &other) = default;

    SqrtArray &operator = (SqrtArray &&other) = default;

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
     * Inserts a value before the iterator location in the structure. If
     * it = end(), then the element is inserted after the last element.
     *
     * @param it the iterator to the element to be inserted right before
     * @param val the value to be inserted
     */
    void insert(forward_iterator it, const Value val) {
        int i = it.getBlockIndex(), j = it.getNestedIndex();
        assert(0 <= i && i <= (int) a.size());
        if (i != (int) a.size()) assert(0 <= j && j < (int) a[i].size());
        if (n++ == 0) {
            a.emplace_back();
            prefixSZ.push_back(0);
        }
        if (i == (int) a.size()) a[--i].push_back(val);
        else a[i].insert(a[i].begin() + j, val);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a[i].size() > 2 * sqrtn) {
            a.emplace(a.begin() + i + 1, a[i].begin() + sqrtn, a[i].end());
            a[i].resize(sqrtn);
            prefixSZ.push_back(0);
        }
        for (int k = i + 1; k < (int) a.size(); k++) {
            prefixSZ[k] = prefixSZ[k - 1] + (int) a[k - 1].size();
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
     * Erases the element at the location of the iterator in the structure.
     *
     * @param it the iterator of the location to erase
     */
    void erase(forward_iterator it) {
        int i = it.getBlockIndex(), j = it.getNestedIndex();
        assert(0 <= i && i <= (int) a.size());
        assert(0 <= j && j < (int) a[i].size());
        --n;
        a[i].erase(a[i].begin() + j);
        if (a[i].empty()) {
            a.erase(a.begin() + i);
            prefixSZ.pop_back();
        }
        for (int k = i + 1; k < (int) a.size(); k++) {
            prefixSZ[k] = prefixSZ[k - 1] + (int) a[k - 1].size();
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
     * Returns a forward random access iterator to the first element.
     *
     * @return a forward random access iterator to the first element.
     */
    forward_iterator begin() {
        return forward_iterator(*this, 0, 0, 0);
    }

    /**
     * Returns a constant forward random access iterator to the first element.
     *
     * @return a constant forward random access iterator to the first element.
     */
    const_forward_iterator begin() const {
        return const_forward_iterator(*this, 0, 0, 0);
    }

    /**
     * Returns a forward random access iterator to just past the last element.
     *
     * @return a forward random access iterator to just past the last element.
     */
    forward_iterator end() {
        return forward_iterator(*this, a.size(), 0, n);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    const_forward_iterator end() const {
        return const_forward_iterator(*this, a.size(), 0, n);
    }

    /**
     * Returns a constant forward random access iterator to the first element.
     *
     * @return a constant forward random access iterator to the first element.
     */
    const_forward_iterator cbegin() const {
        return const_forward_iterator(*this, 0, 0, 0);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    const_forward_iterator cend() const {
        return const_forward_iterator(*this, a.size(), 0, n);
    }

    /**
     * Returns a reverse random access iterator to just before the first element.
     *
     * @return a reverse random access iterator to just before the first element.
     */
    reverse_iterator rbegin() {
        return reverse_iterator(forward_iterator(*this, a.size(), 0, n));
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_forward_iterator(*this, a.size(), 0, n));
    }

    /**
     * Returns a reverse random access iterator to the last element.
     *
     * @return a reverse random access iterator to the last element.
     */
    reverse_iterator rend() {
        return reverse_iterator(forward_iterator(*this, 0, 0, 0));
    }

    /**
     * Returns a constant reverse random access iterator to the last element.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(const_forward_iterator(*this, 0, 0, 0));
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(const_forward_iterator(*this, a.size(), 0, n));
    }

    /**
     * Returns a constant reverse random access iterator to the last element.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    const_reverse_iterator crend() const {
        return const_reverse_iterator(const_forward_iterator(*this, 0, 0, 0));
    }

    /**
     * Returns an iterator to the first element equal to val.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the first element equal to val, end() if no such element exists
     */
    forward_iterator find(const Value val) {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the first element equal to val.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the first element equal to val, end() if no such element exists
     */
    const_forward_iterator find(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the first element equal to val.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @param cmp the comparator
     * @return an iterator to the first element equal to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator find(const Value val, Comparator cmp) {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the first element equal to val.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a constant iterator to the first element equal to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator find(const Value val, Comparator cmp) const {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator lower_bound(const Value val) {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    const_forward_iterator lower_bound(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @param cmp the comparator
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator lower_bound(const Value val, Comparator cmp) {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a constant iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator lower_bound(const Value val, Comparator cmp) const {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to above.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator upper_bound(const Value val) {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value greater than to val. Identical to above.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the smallest value greater than to val, end() if no such element exists
     */
    const_forward_iterator upper_bound(const Value val) const {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to above.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator upper_bound(const Value val, Comparator cmp) {
        pair<int, int> i = above_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value greater than to val. Identical to above.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return a constant iterator to the smallest value greater than to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator upper_bound(const Value val, Comparator cmp) const {
        pair<int, int> i = above_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than or equal to val.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the largest value less than or equal to val, end() if no such element exists
     */
    forward_iterator floor(const Value val) {
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the largest value less than or equal to val.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the largest value less than or equal to val, end() if no such element exists
     */
    const_forward_iterator floor(const Value val) const {
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than or equal to val.
     *
     * @pre the structure must be sorted by the the comparator
     * @param val the value
     * @return an iterator to the largest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator floor(const Value val,  Comparator cmp) {
        pair<int, int> i = floor_ind(val, cmp);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the largest value less than or equal to val.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return a constant iterator to the largest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator floor(const Value val, Comparator cmp) const {
        pair<int, int> i = floor_ind(val, cmp);
        if (i.first == -1) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator ceiling(const Value val) {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    const_forward_iterator ceiling(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator ceiling(const Value val, Comparator cmp) {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return a constant iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator ceiling(const Value val, Comparator cmp) const {
        pair<int, int> i = ceiling_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator above(const Value val) {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the smallest value greater than to val, end() if no such element exists
     */
    const_forward_iterator above(const Value val) const {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator above(const Value val, Comparator cmp) {
        pair<int, int> i = above_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return a constant iterator to the smallest value greater than to val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator above(const Value val, Comparator cmp) const {
        pair<int, int> i = above_ind(val, cmp);
        if (i.first == (int) a.size()) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than val.
     *
     * @param the structure must be sorted by the < operator
     * @param val the value
     * @return an iterator to the largest value less than val, end() if no such element exists
     */
    forward_iterator below(const Value val) {
        pair<int, int> i = below_ind(val);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the largest value less than val.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a constant iterator to the largest value less than val, end() if no such element exists
     */
    const_forward_iterator below(const Value val) const {
        pair<int, int> i = below_ind(val);
        if (i.first == -1) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than val.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return an iterator to the largest value less than val, end() if no such element exists
     */
    template <typename Comparator> forward_iterator below(const Value val, Comparator cmp) {
        pair<int, int> i = below_ind(val, cmp);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns a constant iterator to the largest value less than val.
     *
     * @pre the structure must be sorted by the comparator
     * @param val the value
     * @return a constant iterator to the largest value less than val, end() if no such element exists
     */
    template <typename Comparator> const_forward_iterator below(const Value val, Comparator cmp) const {
        pair<int, int> i = below_ind(val, cmp);
        if (i.first == -1) return end();
        return const_forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
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
        for (forward_iterator i = begin(); i < end(); i++) {
            b.push_back(*i);
        }
        std::sort(b.begin(), b.end());
        int k = 0;
        for (forward_iterator i = begin(); i < end(); i++) {
            *i = b[k++];
        }
    }

    /**
     * Sorts the structure with a comparator by creating an auxiliary array.
     * @param cmp the comparator
     */
    template <typename Comparator> void sort(Comparator cmp) {
        vector<Value> b;
        for (forward_iterator i = begin(); i < end(); i++) {
            b.push_back(*i);
        }
        std::sort(b.begin(), b.end(), cmp);
        int k = 0;
        for (forward_iterator i = begin(); i < end(); i++) {
            *i = b[k++];
        }
    }

    bool operator ==(SqrtArray<Value> &other) const {
        if (this == &other) return true;
        if (n != other.n) return false;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return false;
        }
        return true;
    }

    bool operator !=(SqrtArray<Value> &other) const {
        if (this == &other) return false;
        if (n != other.n) return true;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return true;
        }
        return false;
    }
};

#endif /* DATASTRUCTURES_SQRTARRAY_H_ */
