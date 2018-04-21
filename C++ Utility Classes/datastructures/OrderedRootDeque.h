#ifndef DATASTRUCTURES_ORDEREDROOTDEQUE_H_
#define DATASTRUCTURES_ORDEREDROOTDEQUE_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Ordered Root Array:
 * Decomposes the array into containers of a specified block size (around 2 to 10 times
 * N ^ (1 / R) is optimal).
 *
 * Usage:
 * OrderedRootDeque<65536, int, OrderedSqrtDeque<2048, int>> arr;
 * OrderedRootDeque<65536, int, OrderedSqrtDeque<2048, int, greater<int>>, greater<int>> arr;
 * OrderedRootDeque<131072, int, OrderedRootDeque<8192, int, OrderedSqrtDeque<512, int>>> arr;
 *
 * Initializing: O(N)
 * Insert: O(sqrt(N) + log(N))
 * Erase: O(sqrt(N) + log(N))
 * Pop Front: O(1) amortized
 * Push Front: O(1) amortized
 * Pop Back: O(1) amortized
 * Push Back: O(1) amortized
 * At, Accessor: O(1)
 * Front, Back: O(1)
 * Rank, Contains: O(log(N))
 * Lower Bound, Upper Bound, Floor, Ceiling, Above, Below: O(log(N))
 * Empty, Size: O(1)
 * Values: O(N)
 * Sort: O(N log N) time, O(N) extra space
 */
template <const int BLOCK_SIZE, typename Value, typename Container, typename Comparator = less<Value>,
        typename SmallAlloc = allocator<Value>, typename LargeAlloc = allocator<Container>, typename IntAlloc = allocator<int>>
struct OrderedRootDeque {
private:
    Comparator cmp;
    int n;
    deque<Container, LargeAlloc> a;

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
     * Initializes an empty structure.
     */
    OrderedRootDeque() : n(0) {}

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en. The elements between the 2 iterators
     * must be sorted.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     */
    template <typename It>
    OrderedRootDeque(const It st, const It en) : n(en - st) {
        assert(n >= 0);
        assert(is_sorted(st, en, cmp));
        for (It i = st; i < en; i += BLOCK_SIZE) {
            a.emplace_back(i, min(i + BLOCK_SIZE, en));
        }
    }

    /**
     * Initializes the structures with an initializer list. The elements must be sorted.
     *
     * @param il the initializer list
     */
    OrderedRootDeque(initializer_list<Value> il) : n(il.end() - il.begin()) {
        assert(n >= 0);
        assert(is_sorted(il.begin(), il.end(), cmp));
        for (auto i = il.begin(); i < il.end(); i += BLOCK_SIZE) {
            a.emplace_back(i, min(i + BLOCK_SIZE, il.end()));
        }
    }

    /**
     * Inserts a value into the structure, allowing for duplicates.
     *
     * @param val the value to be inserted
     */
    void insert(const Value val) {
        int i = above_ind(val);
        if (n++ == 0) a.emplace_back();
        if (i == (int) a.size()) a[--i].insert(val);
        else a[i].insert(val);
        if (i < (int) a.size() / 2) {
            for (int j = i - 1; j >= 0; j--) {
                a[j].push_back(a[j + 1].front());
                a[j + 1].pop_front();
            }
            if ((int) a.front().size() > BLOCK_SIZE) {
                a.emplace_front();
                a.front().push_back(a[1].front());
                a[1].pop_front();
            }
        } else {
            for (int j = i + 1; j < (int) a.size(); j++) {
                a[j].push_front(a[j - 1].back());
                a[j - 1].pop_back();
            }
            if ((int) a.back().size() > BLOCK_SIZE) {
                a.emplace_back();
                a.back().push_front(a[((int) a.size()) - 2].back());
                a[((int) a.size()) - 2].pop_back();
            }
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
        if (i < (int) a.size() / 2) {
            for (int j = i - 1; j >= 0; j--) {
                a[j + 1].push_front(a[j].back());
                a[j].pop_back();
            }
            if ((int) a.front().size() == 0) a.pop_front();
        } else {
            for (int j = i + 1; j < (int) a.size(); j++) {
                a[j - 1].push_back(a[j].front());
                a[j].pop_front();
            }
            if ((int) a.back().size() == 0) a.pop_back();
        }
        return true;
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
     * Erases the first element in the structure.
     */
    void pop_front() {
        assert(n > 0);
        --n;
        a.front().pop_front();
        if (a.front().empty()) a.pop_front();
    }

    /**
     * Inserts an element at the front of the structure. val must be no greater than
     * the front element.
     * @param val the value to be inserted
     */
    void push_front(const Value &val) {
        if (n > 0) assert(!cmp(front(), val));
        if (n++ == 0) a.emplace_front();
        a.front().push_front(val);
        if ((int) a.front().size() > BLOCK_SIZE) {
            a.emplace_front();
            a.front().push_back(a[1].front());
            a[1].pop_front();
        }
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
     * Erases the last element in the structure.
     */
    void pop_back() {
        assert(n > 0);
        --n;
        a.back().pop_back();
        if (a.back().empty()) a.pop_back();
    }

    /**
     * Inserts an element at the back of the structure. val must be no less than
     * the back element.
     * @param val the value to be inserted
     */
    void push_back(const Value &val) {
        if (n > 0) assert(!cmp(val, back()));
        if (n++ == 0) a.emplace_back();
        a.back().push_back(val);
        if ((int) a.back().size() > BLOCK_SIZE) {
            a.emplace_back();
            a.back().push_front(a[((int) a.size()) - 2].back());
            a[((int) a.size()) - 2].pop_back();
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
        if (k < (int) a[0].size()) return a[0][k];
        int m = k - (int) a[0].size();
        return a[1 + m / BLOCK_SIZE][m % BLOCK_SIZE];
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
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val. Identical to ceiling.
     *
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    pair<int, Value> lower_bound(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        pair<int, Value> j = a[i].lower_bound(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val. Identical to above.
     *
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> upper_bound(const Value val) const {
        int i = above_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        pair<int, Value> j = a[i].upper_bound(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than or equal to val.
     *
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than or equal to val
     * @throws no_such_element_exception if val is less than the smallest value
     * in the structure
     */
    pair<int, Value> floor(const Value val) const {
        int i = floor_ind(val);
        if (i == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        pair<int, Value> j = a[i].floor(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val. Identical to upper_bound.
     *
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> above(const Value val) const {
        int i = above_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        pair<int, Value> j = a[i].above(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than val.
     *
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than val
     * @throws no_such_element_exception if val is less than or equal to the smallest value
     * in the structure
     */
    pair<int, Value> below(const Value val) const {
        int i = below_ind(val);
        if (i == -1) throw no_such_element_exception("call to below() resulted in no such value");
        pair<int, Value> j = a[i].below(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val. Identical to lower_bound.
     *
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    pair<int, Value> ceiling(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        pair<int, Value> j = a[i].ceiling(val);
        if (i == 0) return j;
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + j.first, j.second};
    }

    /**
     * Returns all values in the structure.
     *
     * @return a vector containing all values in the structure
     */
    vector<Value, SmallAlloc> values() const {
        vector<Value, SmallAlloc> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (Value x : a[i].values()) {
                ret.push_back(x);
            }
        }
        return ret;
    }
};

#endif /* DATASTRUCTURES_ORDEREDROOTDEQUE_H_ */
