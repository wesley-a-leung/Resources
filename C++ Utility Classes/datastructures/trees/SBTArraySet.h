#ifndef DATASTRUCTURES_TREES_SBTARRAYSET_H_
#define DATASTRUCTURES_TREES_SBTARRAYSET_H_

#include <bits/stdc++.h>

using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

template <typename Value>
struct SBTArraySet {

private:
    Value *VAL; // values
    int *SZ; // size of subtree
    int *L; // index of left child
    int *R; // index of right child

    int root;
    int ind; // current ind
    int capacity; // capacity
    const int INIT_CAPACITY = 4; // default capacity

    /**
     * Resizes the arrays and copies all the values
     */
    void resize() {
        Value *NEW_VAL = new int[capacity * 2];
        int *NEW_SZ = new int[capacity * 2];
        int *NEW_L = new int[capacity * 2];
        int *NEW_R = new int[capacity * 2];
        for (int i = 0; i < capacity; i++) {
            NEW_VAL[i] = VAL[i];
            NEW_SZ[i] = SZ[i];
            NEW_L[i] = L[i];
            NEW_R[i] = R[i];
        }
        swap(NEW_VAL, VAL);
        swap(NEW_SZ, SZ);
        swap(NEW_L, L);
        swap(NEW_R, R);
        delete[](NEW_VAL);
        delete[](NEW_SZ);
        delete[](NEW_L);
        delete[](NEW_R);
        capacity *= 2;
    }

    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    void update(int x) {
        SZ[x] = 1 + SZ[L[x]] + SZ[R[x]];
    }

    /**
     * Rotates the given subtree to the right.
     *
     * @param x the subtree
     * @return the right rotated subtree
     */
    int rotateRight(int x) {
        int y = L[x];
        L[x] = R[y];
        R[y] = x;
        update(x);
        update(y);
        return y;
    }

    /**
     * Rotates the given subtree to the left.
     *
     * @param x the subtree
     * @return the left rotated subtree
     */
    int rotateLeft(int x) {
        int y = R[x];
        R[x] = L[y];
        L[y] = x;
        update(x);
        update(y);
        return y;
    }

    /**
     * Balances the tree by size.
     *
     * @param x the subtree
     * @param flag whether the left subtree should be compared or the right subtree:
     *        {@code true} if the tree is right heavy, {@code false} if the tree is left heavy.
     * @return the balanced subtree
     */
    int maintain(int x, bool flag) {
        if (flag) {
            if (SZ[L[x]] < SZ[L[R[x]]]) {
                R[x] = rotateRight(R[x]);
                x = rotateLeft(x);
            } else if (SZ[L[x]] < SZ[R[R[x]]]) {
                x = rotateLeft(x);
            } else {
                return x;
            }
        } else {
            if (SZ[R[x]] < SZ[R[L[x]]]) {
                L[x] = rotateLeft(L[x]);
                x = rotateRight(x);
            } else if (SZ[R[x]] < SZ[L[L[x]]]) {
                x = rotateRight(x);
            } else {
                return x;
            }
        }
        L[x] = maintain(L[x], false);
        R[x] = maintain(R[x], true);
        x = maintain(x, true);
        x = maintain(x, false);
        return x;
    }

    // auxiliary function for contains
    bool contains(int x, Value val) {
        if (x == 0) return false;
        else if (val < VAL[x]) return contains(L[x], val);
        else if (val > VAL[x]) return contains(R[x], val);
        return true;
    }

    /**
     * Inserts the specified value into the symbol table, allowing for duplicates.
     * Deletes the specified value from this symbol table if the specified value is {@code null}.
     *
     * @param x the subtree
     * @param val the value
     * @return the subtree
     */
    int add(int x, Value val) {
        if (x == 0) {
            if (ind >= capacity) resize();
            VAL[ind] = val;
            SZ[ind] = 1;
            L[ind] = 0;
            R[ind] = 0;
            return ind++;
        }
        if (val < VAL[x]) L[x] = add(L[x], val);
        else R[x] = add(R[x], val);
        update(x);
        return maintain(x, val >= VAL[x]);
    }

    /**
     * Removes the smallest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    int removeMin(int x) {
        if (L[x] == 0) return R[x];
        L[x] = removeMin(L[x]);
        update(x);
        return x;
    }

    /**
     * Removes the largest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    int removeMax(int x) {
        if (R[x] == 0) return L[x];
        R[x] = removeMax(R[x]);
        update(x);
        return x;
    }

    /**
     * Returns the node with the smallest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the smallest value in the subtree
     */
    int getMin(int x) {
        if (L[x] == 0) return x;
        return getMin(L[x]);
    }

    /**
     * Returns the node with the largest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the largest value in the subtree
     */
    int getMax(int x) {
        if (R[x] == 0) return x;
        return getMax(R[x]);
    }

    /**
     * Removes the specified value and its associated value from the given
     * subtree.
     *
     * @param x the subtree
     * @param val the value
     * @return the updated subtree
     */
    int remove(int x, Value val) {
        if (val < VAL[x]) L[x] = remove(L[x], val);
        else if (val > VAL[x]) R[x] = remove(R[x], val);
        else {
            if (L[x] == 0) return R[x];
            else if (R[x] == 0) return L[x];
            else {
                int y = x;
                x = getMin(R[y]);
                R[x] = removeMin(R[y]);
                L[x] = L[y];
            }
        }
        update(x);
        return x;
    }

    /**
     * Returns the node in the subtree with the largest value less than or equal
     * to the given value.
     *
     * @param x the subtree
     * @param val the value
     * @return the node in the subtree with the largest value less than or equal
     *         to the given value
     */
    int floor(int x, Value val) {
        if (x == 0) return 0;
        if (val == VAL[x]) return x;
        if (val < VAL[x]) return floor(L[x], val);
        int y = floor(R[x], val);
        if (y != 0) return y;
        else return x;
    }

    /**
     * Returns the node in the subtree with the smallest value greater than or
     * equal to the given value.
     *
     * @param x the subtree
     * @param val the value
     * @return the node in the subtree with the smallest value greater than or
     *         equal to the given value
     */
    int ceiling(int x, Value val) {
        if (x == 0) return 0;
        if (val == VAL[x]) return x;
        if (val > VAL[x]) return ceiling(R[x], val);
        int y = ceiling(L[x], val);
        if (y != 0) return y;
        else return x;
    }

    /**
     * Returns the node with value the kth smallest value in the subtree.
     *
     * @param x the subtree
     * @param k the kth smallest value in the subtree
     * @return the node with value the kth smallest value in the subtree
     */
    int select(int x, int k) {
        if (x == 0) return 0;
        int t = SZ[L[x]];
        if (t > k) return select(L[x], k);
        else if (t < k) return select(R[x], k - t - 1);
        return x;
    }

    /**
     * Returns the number of values in the subtree less than val.
     *
     * @param val the value
     * @param x the subtree
     * @return the number of values in the subtree less than val
     */
    int getRank(int x, Value val) {
        if (x == 0) return 0;
        if (val <= VAL[x]) return getRank(L[x], val);
        else return 1 + SZ[L[x]] + getRank(R[x], val);
    }

    /**
     * Adds the values in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void valuesInOrder(int x, vector<Value> *queue) {
        if (x == 0) return;
        valuesInOrder(L[x], queue);
        queue->push_back(VAL[x]);
        valuesInOrder(R[x], queue);
    }

    /**
     * Adds the values between {@code lo} and {@code hi} in the subtree
     * to the {@code queue}.
     *
     * @param x the subtree
     * @param queue the queue
     * @param lo the lowest value
     * @param hi the highest value
     */
    void values(int x, vector<Value> *queue, Value lo, Value hi) {
        if (x == 0) return;
        if (lo < VAL[x]) values(L[x], queue, lo, hi);
        if (lo <= VAL[x] && hi >= VAL[x]) queue->push_back(VAL[x]);
        if (hi > VAL[x]) values(R[x], queue, lo, hi);
    }

public:
    /**
     * Initializes an empty symbol table with an initial capacity of 4.
     */
    SBTArraySet() {
        VAL = new Value[INIT_CAPACITY];
        SZ = new int[INIT_CAPACITY];
        L = new int[INIT_CAPACITY];
        R = new int[INIT_CAPACITY];
        root = 0;
        SZ[root] = 0;
        L[root] = 0;
        R[root] = 0;
        ind = 1;
        capacity = INIT_CAPACITY;
    }

    /**
     * Initializes an empty symbol table with a initial capacity.
     *
     * @param N the initial capacity of the symbol table
     */
    SBTArraySet(int N) {
        N++; // zero node is never used
        VAL = new Value[N];
        SZ = new int[N];
        L = new int[N];
        R = new int[N];
        root = 0;
        SZ[root] = 0;
        L[root] = 0;
        R[root] = 0;
        ind = 1;
        capacity = N;
    }

    /**
     * Checks if the symbol table is empty.
     *
     * @return {@code true} if the symbol table is empty.
     */
    bool isEmpty() {
        return root == 0;
    }

    /**
     * Returns the number values in the symbol table.
     *
     * @return the number values pairs in the symbol table
     */
    int size() {
        return SZ[root];
    }

    /**
     * Checks if the symbol table contains the given value.
     *
     * @param val the value
     * @return {@code true} if the symbol table contains {@code val}
     *         and {@code false} otherwise
     */
    bool contains(Value val) {
        return contains(root, val);
    }

    /**
     * Inserts the specified value into the symbol table, allowing for duplicates.
     *
     * @param val the value
     */
    void add(Value val) {
        root = add(root, val);
    }

    /**
     * Removes the specified value from the symbol table
     *
     * @param val the value
     */
    void remove(Value val) {
        if (!contains(val)) return;
        root = remove(root, val);
    }

    /**
     * Removes the smallest value from the symbol table.
     *
     * @throws runtime_error if the symbol table is empty
     */
    void removeMin() {
        if (isEmpty()) throw runtime_error("called removeMin() with empty symbol table");
        root = removeMin(root);
    }

    /**
     * Removes the largest value from the symbol table.
     *
     * @throws runtime_error if the symbol table is empty
     */
    void removeMax() {
        if (isEmpty()) throw runtime_error("called removeMax() with empty symbol table");
        root = removeMax(root);
    }

    /**
     * Returns the smallest value in the symbol table.
     *
     * @return the smallest value in the symbol table
     * @throws runtime_error if the symbol table is empty
     */
    Value getMin() {
        if (isEmpty()) throw runtime_error("called getMin() with empty symbol table");
        return VAL[getMin(root)];
    }

    /**
     * Returns the largest value in the symbol table.
     *
     * @return the largest value in the symbol table
     * @throws runtime_error if the symbol table is empty
     */
    Value getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty symbol table");
        return VAL[getMax(root)];
    }

    /**
     * Returns the largest value in the symbol table less than or equal to {@code val}.
     *
     * @param val the value
     * @return the largest value in the symbol table less than or equal to {@code val}
     * @throws runtime_error if the symbol table is empty
     * @throws no_such_element_exception if there is no value in the symbol table less than or equal to {@code val}
     */
    Value floor(Value val) {
        if (isEmpty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, val);
        if (x == 0) throw no_such_element_exception("call to floor() resulted in no such value");
        return VAL[x];
    }

    /**
     * Returns the smallest value in the symbol table greater than or equal to {@code val}.
     *
     * @param val the value
     * @return a pair containing the smallest value in the symbol table greater than or equal to {@code val}
     * @throws runtime_error if the symbol table is empty
     * @throws no_such_element_exception if there is no value in the symbol table greater than or equal to {@code val}
     */
    Value ceiling(Value val) {
        if (isEmpty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, val);
        if (x == 0) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return VAL[x];
    }

    /**
     * Returns the kth smallest value in the symbol table.
     *
     * @param k the order statistic
     * @return the kth smallest value in the symbol table
     * @throws invalid_argument unless {@code k} is between 0 and
     *             {@code size() -1 }
     */
    Value select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        return VAL[select(root, k)];
    }

    /**
     * Returns the number of values in the symbol table strictly less than
     * {@code val}.
     *
     * @param val the value
     * @return the number of values in the symbol table strictly less than
     *         {@code val}
     */
    int getRank(Value val) {
        return getRank(root, val);
    }

    /**
     * Returns all values in the symbol table following an in-order traversal.
     *
     * @return all values in the symbol table following an in-order traversal
     */
    vector<Value> &values() {
        vector<Value> queue;
        valuesInOrder(root, &queue);
        return queue;
    }

    /**
     * Returns all values in the symbol table in the given range.
     *
     * @param lo the lowest value
     * @param hi the highest value
     * @return all value in the symbol table between {@code lo} (inclusive)
     *         and {@code hi} (exclusive)
     */
    vector<Value> &values(Value lo, Value hi) {
        vector<Value> queue;
        values(root, &queue, lo, hi);
        return queue;
    }

    /**
     * Returns the number of values in the symbol table in the given range.
     *
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of values in the symbol table between {@code lo}
     *         (inclusive) and {@code hi} (exclusive)
     */
    int size(Value lo, Value hi) {
        if (lo > hi) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_SBTARRAYSET_H_ */
