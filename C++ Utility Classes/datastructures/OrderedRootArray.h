#ifndef DATASTRUCTURES_ORDEREDROOTARRAY_H_
#define DATASTRUCTURES_ORDEREDROOTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Ordered Sqrt Array:
 * Decomposes the array into blocks of size sqrt(n) multiplied by a factor.
 * The factor should be between 1 and 10, and should be smaller for large N.
 *
 * Usage:
 * OrderedSqrtArray<int> arr;
 * OrderedSqrtArray<int, greater<int>> arr;
 *
 * Initializing: O(N)
 * Insert: O(sqrt(N) + log(N))
 * Erase: O(sqrt(N) + log(N))
 * Pop Front: O(sqrt(N))
 * Pop Back: O(1) amortized
 * At, Accessor: O(log(N))
 * Front, Back: O(1)
 * Rank, Contains: O(log(N))
 * Lower Bound, Upper Bound, Floor, Ceiling, Above, Below: O(log(N))
 * Empty, Size: O(1)
 * Values: O(N)
 */
template <typename Value, typename Comparator = less<Value>>
struct OrderedSqrtArray {
private:
    Comparator cmp; // the comparator
    int n; // the size of the array
    int SCALE_FACTOR; // the scale factor of sqrt(n)
    vector<vector<Value>> a; // the array
    vector<int> prefixSZ; // the prefix array of the sizes of the blocks

    // returns the 2D index of the smallest value greater than or equal to val
    pair<int, int> ceiling_ind(const Value val) const {
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

        friend class OrderedSqrtArray;

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

        iterator(Object &arr, const iterator &other) : arr(arr), i(other.i), j(other.j), k(other.k) {}

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

    using forward_iterator = iterator<const Value, const OrderedSqrtArray<Value, Comparator>>;
    using reverse_iterator = reverse_iterator<forward_iterator>;

    /**
     * Initializes an empty structure.
     * @param SCALE_FACTOR scales the value of sqrt(n) by this value
     */
    OrderedSqrtArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en. The elements between the 2 iterators
     * must be sorted.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     * @param SCALE_FACTOR scales the value of sqrt(n) by this value
     */
    template <typename It>
    OrderedSqrtArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0);
        assert(is_sorted(st, en, cmp));
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        for (It i = st; i < en; i += sqrtn) {
            a.emplace_back(i, min(i + sqrtn, en));
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    OrderedSqrtArray(const OrderedSqrtArray &other) = default;

    OrderedSqrtArray(OrderedSqrtArray &&other) = default;

    virtual ~OrderedSqrtArray() = default;

    OrderedSqrtArray &operator = (const OrderedSqrtArray &other) = default;

    OrderedSqrtArray &operator = (OrderedSqrtArray &&other) = default;

    /**
     * Inserts a value into the structure, allowing for duplicates.
     *
     * @param val the value to be inserted
     */
    void insert(const Value val) {
        pair<int, int> i = above_ind(val);
        if (n++ == 0) {
            a.emplace_back();
            prefixSZ.push_back(0);
        }
        if (i.first == (int) a.size()) a[--i.first].push_back(val);
        else a[i.first].insert(a[i.first].begin() + i.second, val);
        int sqrtn = (int) sqrt(n) * SCALE_FACTOR;
        if ((int) a[i.first].size() > 2 * sqrtn) {
            a.emplace(a.begin() + i.first + 1, a[i.first].begin() + sqrtn, a[i.first].end());
            a[i.first].resize(sqrtn);
            prefixSZ.push_back(0);
        }
        for (int j = i.first + 1; j < (int) a.size(); j++) {
            prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        }
    }

    /**
     * Erases a value in the structure. If there are multiple, only one is erased.
     * If the value does not exist, then it is not removed.
     *
     * @param val the value to erase
     * @return true if the value was in the structure and removed, false otherwise
     */
    bool erase(const Value val) {
        pair<int, int> i = ceiling_ind(val);
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
     * Checks if the structure contains the given key.
     *
     * @param val the value
     * @return true if the structure contains val, false otherwise
     */
    bool contains(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        return i.first != (int) a.size() && a[i.first][i.second] == val;
    }

    /**
     * Returns a constant forward random access iterator to the first element. Identical to cbegin.
     *
     * @return a constant forward random access iterator to the first element.
     */
    forward_iterator begin() const {
        return forward_iterator(*this, 0, 0, 0);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element. Identical to cend.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    forward_iterator end() const {
        return forward_iterator(*this, a.size(), 0, n);
    }

    /**
     * Returns a constant forward random access iterator to the first element. Identical to begin.
     *
     * @return a constant forward random access iterator to the first element.
     */
    forward_iterator cbegin() const {
        return forward_iterator(*this, 0, 0, 0);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element. Identical to end.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    forward_iterator cend() const {
        return forward_iterator(*this, a.size(), 0, n);
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element. Identical to crbegin.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    reverse_iterator rbegin() const {
        return reverse_iterator(forward_iterator(*this, a.size(), 0, n));
    }

    /**
     * Returns a constant reverse random access iterator to the last element. Identical to crend.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    reverse_iterator rend() const {
        return reverse_iterator(forward_iterator(*this, 0, 0, 0));
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element. Identical to cbegin.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    reverse_iterator crbegin() const {
        return reverse_iterator(forward_iterator(*this, a.size(), 0, n));
    }

    /**
     * Returns a constant reverse random access iterator to the last element. Identical to rend.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    reverse_iterator crend() const {
        return reverse_iterator(forward_iterator(*this, 0, 0, 0));
    }

    /**
     * Returns an iterator to the first element equal to val.
     *
     * @param val the value
     * @return an iterator to the first element equal to val, end() if no such element exists
     */
    forward_iterator find(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator lower_bound(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to above.
     *
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator upper_bound(const Value val) const {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than or equal to val.
     *
     * @param val the value
     * @return an iterator to the largest value less than or equal to val, end() if no such element exists
     */
    forward_iterator floor(const Value val) const {
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator ceiling(const Value val) const {
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator above(const Value val) const {
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
    }

    /**
     * Returns an iterator to the largest value less than val.
     *
     * @param val the value
     * @return an iterator to the largest value less than val, end() if no such element exists
     */
    forward_iterator below(const Value val) const {
        pair<int, int> i = below_ind(val);
        if (i.first == -1) return end();
        return forward_iterator(*this, i.first, i.second, prefixSZ[i.first] + i.second);
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

    bool operator ==(OrderedSqrtArray<Value, Comparator> &other) const {
        if (this == &other) return true;
        if (n != other.n) return false;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return false;
        }
        return true;
    }

    bool operator !=(OrderedSqrtArray<Value, Comparator> &other) const {
        if (this == &other) return false;
        if (n != other.n) return true;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return true;
        }
        return false;
    }
};

/**
 * Ordered Root Array:
 * Decomposes the array into N ^ (1 / R) containers of size N ^ ((R - 1) / R) multiplied by a factor.
 * The factor should be between 1 and 10, and should be smaller for large N.
 *
 * Usage:
 * OrderedRootArray<3, int, OrderedSqrtArray<int>> arr;
 * OrderedRootArray<3, int, OrderedSqrtArray<int, greater<int>>, greater<int>> arr;
 * OrderedRootArray<4, int, OrderedRootArray<3, int, OrderedSqrtArray<int>>> arr;
 *
 * Initializing: O(N)
 * Insert: O(N ^ (1 / R) + log(N))
 * Erase: O(N ^ (1 / R) + log(N))
 * Pop Front: O(N ^ (1 / R))
 * Pop Back: O(1) amortized
 * At, Accessor: O(log(N))
 * Front, Back: O(1)
 * Rank, Contains: O(log(N))
 * Lower Bound, Upper Bound, Floor, Ceiling, Above, Below: O(log(N))
 * Empty, Size: O(1)
 * Values: O(N)
 */
template <const int R, typename Value, typename Container, typename Comparator = less<Value>>
struct OrderedRootArray {
private:
    Comparator cmp; // the comparator
    int n; // the size of the array
    int SCALE_FACTOR; // the scale factor
    vector<Container> a; // the array
    vector<int> prefixSZ; // the prefix array of the sizes of the blocks

    // returns the index of the container with the smallest value greater than or equal to val
    int ceiling_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    // returns the index of the container with the smallest value greater than val
    int above_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }

    // returns the index of the container with largest value less than or equal to val
    int floor_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        return hi;
    }

    // returns the index of the container with largest value less than val
    int below_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        return hi;
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
        int i = 0;
        typename Container::forward_iterator *j;
        int k = 0;

        friend class OrderedRootArray;

        int getBlockIndex() const {
            return i;
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

        iterator(Object &arr, int i, typename Container::forward_iterator *j, int k) : arr(arr), i(i), j(j), k(k) {}

        iterator(const iterator &other) = default;

        virtual ~iterator() {
            delete j;
        }

        iterator &operator = (const iterator &other) = default;

        reference operator *() const {
            return *(*j);
        }

        pointer operator ->() const {
            return &(*(*j));
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
            if (nullptr == j || ++(*j) == arr.a[i].end()) {
                ++i;
                if (i != (int) arr.a.size()) {
                    delete j;
                    j = new typename Container::forward_iterator(arr.a[i], arr.a[i].begin());
                }
            }
            return *this;
        }

        iterator operator ++(int) {
            iterator temp = *this;
            ++k;
            if (j == nullptr || ++(*j) == arr.a[i].end()) {
                ++i;
                if (i != (int) arr.a.size()) {
                    delete j;
                    j = new typename Container::forward_iterator(arr.a[i], arr.a[i].begin());
                }
            }
            return temp;
        }

        iterator &operator --() {
            --k;
            if (i == (int) arr.a.size() || nullptr == j || *j == arr.a[i].begin()) {
                delete j;
                --i;
                j = new typename Container::forward_iterator(arr.a[i], prev(arr.a[i].end()));
            } else {
                j--;
            }
            return *this;
        }

        iterator operator --(int) {
            iterator temp = *this;
            --k;
            if (i == (int) arr.a.size() || nullptr == j || *j == arr.a[i].begin()) {
                delete j;
                --i;
                j = new typename Container::forward_iterator(arr.a[i], prev(arr.a[i].end()));
            } else {
                j--;
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
            if (i != (int) arr.a.size()) {
                delete j;
                j = new typename Container::forward_iterator(arr.a[i], arr.a[i].begin() + (k - arr.prefixSZ[hi]));
            }
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
            typename Container::forward_iterator j = (i == (int) arr.a.size()) ? new typename Container::forward_iterator(arr.a[i - 1], arr.a[i - 1].end())
                : typename Container::forward_iterator(arr.a[i], arr.a[i].begin() + (k - arr.prefixSZ[hi]));
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
            if (i != (int) arr.a.size()) {
                delete j;
                j = new typename Container::forward_iterator(arr.a[i], arr.a[i].begin() + (k - arr.prefixSZ[hi]));
            }
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
            typename Container::forward_iterator j = (i == (int) arr.a.size()) ? new typename Container::forward_iterator(arr.a[i - 1], arr.a[i - 1].end())
                : typename Container::forward_iterator(arr.a[i], arr.a[i].begin() + (k - arr.prefixSZ[hi]));
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

    using forward_iterator = iterator<const Value, const OrderedRootArray<R, Value, Container, Comparator>>;
    using reverse_iterator = reverse_iterator<forward_iterator>;

    /**
     * Initializes an empty structure.
     * @param SCALE_FACTOR scales the value of N ^ (1 / R) by this value
     */
    OrderedRootArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en. The elements between the 2 iterators
     * must be sorted.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     * @param SCALE_FACTOR scales the value of N ^ (1 / R) by this value
     */
    template <typename It>
    OrderedRootArray(const It st, const It en, const int SCALE_FACTOR = 1) : n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0);
        assert(is_sorted(st, en, cmp));
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (It i = st; i < en; i += rootn) {
            a.emplace_back(i, min(i + rootn, st + n), SCALE_FACTOR);
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) {
            prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
        }
    }

    /**
     * Inserts a value into the structure, allowing for duplicates.
     *
     * @param val the value to be inserted
     */
    void insert(const Value val) {
        int i = above_ind(val);
        if (n++ == 0) {
            a.emplace_back();
            prefixSZ.push_back(0);
        }
        if (i == (int) a.size()) a[--i].insert(val);
        else a[i].insert(val);
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        if ((int) a[i].size() > 2 * rootn) {
            vector<Value> b;
            while (a[i].size() > rootn) {
                b.push_back(a[i].back());
                a[i].pop_back();
            }
            reverse(b.begin(), b.end());
            a.emplace(a.begin() + i + 1, b.begin(), b.end());
            prefixSZ.push_back(0);
        }
        for (int j = i + 1; j < (int) a.size(); j++) {
            prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        }
    }

    /**
     * Erases a value in the structure. If there are multiple, only one is erased.
     * If the value does not exist, then it is not removed.
     *
     * @param val the value to erase
     * @return true if the value was in the structure and removed, false otherwise
     */
    bool erase(const Value val) {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) return false;
        if (!a[i].erase(val)) return false;
        --n;
        if (a[i].empty()) {
            a.erase(a.begin() + i);
            prefixSZ.pop_back();
        }
        for (int j = i + 1; j < (int) a.size(); j++) {
            prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        }
        return true;
    }

    /**
     * Erases the first element in the structure.
     */
    void pop_front() {
        assert(n > 0);
        --n;
        a.front().pop_front();
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
        return a[hi].at(k - prefixSZ[hi]);
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
     * Checks if the structure contains the given key.
     *
     * @param val the value
     * @return true if the structure contains val, false otherwise
     */
    bool contains(const Value val) const {
        int i = ceiling_ind(val);
        return i != (int) a.size() && a[i].contains(val);
    }

    /**
     * Returns a constant forward random access iterator to the first element. Identical to cbegin.
     *
     * @return a constant forward random access iterator to the first element.
     */
    forward_iterator begin() const {
        return forward_iterator(*this, 0, a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, 0);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element. Identical to cend.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    forward_iterator end() const {
        return forward_iterator(*this, a.size(), a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, n);
    }

    /**
     * Returns a constant forward random access iterator to the first element. Identical to begin.
     *
     * @return a constant forward random access iterator to the first element.
     */
    forward_iterator cbegin() const {
        return forward_iterator(*this, 0, a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, 0);
    }

    /**
     * Returns a constant forward random access iterator to just past the last element. Identical to end.
     *
     * @return a constant forward random access iterator to just past the last element.
     */
    forward_iterator cend() const {
        return forward_iterator(*this, a.size(), a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, n);
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element. Identical to crbegin.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    reverse_iterator rbegin() const {
        return reverse_iterator(forward_iterator(*this, 0, a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, 0));
    }

    /**
     * Returns a constant reverse random access iterator to the last element. Identical to crend.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    reverse_iterator rend() const {
        return reverse_iterator(forward_iterator(*this, a.size(), a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, n));
    }

    /**
     * Returns a constant reverse random access iterator to just before the first element. Identical to cbegin.
     *
     * @return a constant reverse random access iterator to just before the first element.
     */
    reverse_iterator crbegin() const {
        return reverse_iterator(forward_iterator(*this, a.size(), a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, n));
    }

    /**
     * Returns a constant reverse random access iterator to the last element. Identical to rend.
     *
     * @return a constant reverse random access iterator to the last element.
     */
    reverse_iterator crend() const {
        return reverse_iterator(forward_iterator(*this, a.size(), a.size() > 0 ? new typename Container::forward_iterator(a[0], a[0].begin()) : nullptr, n));
    }

    /**
     * Returns an iterator to the first element equal to val.
     *
     * @param val the value
     * @return an iterator to the first element equal to val, end() if no such element exists
     */
    forward_iterator find(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].find(val);
        if (j == a[i].end()) return end();
        return forward_iterator(*this, i, &j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to ceiling.
     *
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator lower_bound(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].lower_bound(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to above.
     *
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator upper_bound(const Value val) const {
        int i = above_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].upper_bound(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the largest value less than or equal to val.
     *
     * @param val the value
     * @return an iterator to the largest value less than or equal to val, end() if no such element exists
     */
    forward_iterator floor(const Value val) const {
        int i = floor_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].floor(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the smallest value less than or equal to val. Identical to lower_bound.
     *
     * @param val the value
     * @return an iterator to the smallest value less than or equal to val, end() if no such element exists
     */
    forward_iterator ceiling(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].ceiling(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the smallest value greater than to val. Identical to upper_bound.
     *
     * @param val the value
     * @return an iterator to the smallest value greater than to val, end() if no such element exists
     */
    forward_iterator above(const Value val) const {
        int i = above_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].above(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns an iterator to the largest value less than val.
     *
     * @param val the value
     * @return an iterator to the largest value less than val, end() if no such element exists
     */
    forward_iterator below(const Value val) const {
        int i = below_ind(val);
        if (i == (int) a.size()) return end();
        typename Container::forward_iterator j = a[i].below(val);
        return forward_iterator(*this, i, j, prefixSZ[i] + (j - a[i].begin()));
    }

    /**
     * Returns all values in the structure.
     *
     * @return a vector containing all values in the structure
     */
    vector<Value> values() const {
        vector<Value> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (Value x : a[i].values()) {
                ret.push_back(x);
            }
        }
        return ret;
    }

    bool operator ==(OrderedRootArray<R, Value, Comparator> &other) const {
        if (this == &other) return true;
        if (n != other.n) return false;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return false;
        }
        return true;
    }

    bool operator !=(OrderedRootArray<R, Value, Comparator> &other) const {
        if (this == &other) return false;
        if (n != other.n) return true;
        for (forward_iterator i = begin(), j = other.begin(); i < end(); i++, j++) {
            if (*i != *j) return true;
        }
        return false;
    }
};

#endif /* DATASTRUCTURES_ORDEREDROOTARRAY_H_ */
