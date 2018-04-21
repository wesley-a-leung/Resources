#ifndef DATASTRUCTURES_ORDEREDSQRTDEQUE_H_
#define DATASTRUCTURES_ORDEREDSQRTDEQUE_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Ordered Sqrt Deque:
 * Decomposes the array into deques of a specificed block size (around 2 to 10 times
 * sqrt(n) is optimal).
 *
 * Usage:
 * OrderedSqrtDeque<4096, int> arr;
 * OrderedSqrtDeque<4096, int, greater<int>> arr;
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
template <const int BLOCK_SIZE, typename Value, typename Comparator = less<Value>,
        typename SmallAlloc = allocator<Value>, typename LargeAlloc = allocator<vector<Value>>>
struct OrderedSqrtDeque {
private:
    Comparator cmp;
    int n;
    deque<deque<Value, SmallAlloc>, LargeAlloc> a;

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
     * Initializes an empty structure.
     */
    OrderedSqrtDeque() : n(0) {}

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en. The elements between the 2 iterators
     * must be sorted.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     */
    template <typename It>
    OrderedSqrtDeque(const It st, const It en) : n(en - st) {
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
    OrderedSqrtDeque(initializer_list<Value> il) : n(il.end() - il.begin()) {
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
        pair<int, int> i = above_ind(val);
        if (n++ == 0) a.emplace_back();
        if (i.first == (int) a.size()) a[--i.first].push_back(val);
        else a[i.first].insert(a[i.first].begin() + i.second, val);
        if (i.first < (int) a.size() / 2) {
            for (int j = i.first - 1; j >= 0; j--) {
                a[j].push_back(a[j + 1].front());
                a[j + 1].pop_front();
            }
            if ((int) a.front().size() > BLOCK_SIZE) {
                a.emplace_front();
                a.front().push_back(a[1].front());
                a[1].pop_front();
            }
        } else {
            for (int j = i.first + 1; j < (int) a.size(); j++) {
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
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size() || a[i.first][i.second] != val) return false;
        --n;
        a[i.first].erase(a[i.first].begin() + i.second);
        if (i.first < (int) a.size() / 2) {
            for (int j = i.first - 1; j >= 0; j--) {
                a[j + 1].push_front(a[j].back());
                a[j].pop_back();
            }
            if (a.front().empty()) a.pop_front();
        } else {
            for (int j = i.first + 1; j < (int) a.size(); j++) {
                a[j - 1].push_back(a[j].front());
                a[j].pop_front();
            }
            if (a.back().empty()) a.pop_back();
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
        pair<int, int> i = ceiling_ind(val);
        return i.first != (int) a.size() && a[i.first][i.second] == val;
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
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
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
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
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
        pair<int, int> i = floor_ind(val);
        if (i.first == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
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
        pair<int, int> i = ceiling_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
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
        pair<int, int> i = above_ind(val);
        if (i.first == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
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
        pair<int, int> i = below_ind(val);
        if (i.first == -1) throw no_such_element_exception("call to below() resulted in no such value");
        if (i.first == 0) return {i.second, a[i.first][i.second]};
        return {(int) a[0].size() + (i.first - 1) * BLOCK_SIZE + i.second, a[i.first][i.second]};
    }

    /**
     * Returns all values in the structure.
     *
     * @return a vector containing all values in the structure
     */
    vector<Value, SmallAlloc> values() const {
        vector<Value, SmallAlloc> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                ret.push_back(a[i][j]);
            }
        }
        return ret;
    }
};

#endif /* DATASTRUCTURES_ORDEREDSQRTDEQUE_H_ */
