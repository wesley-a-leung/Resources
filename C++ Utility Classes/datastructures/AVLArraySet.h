/*
 * AVLArraySet.h
 *
 *  Created on: Jul 17, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_AVLTREESET_H_
#define DATASTRUCTURES_AVLTREESET_H_

#include <bits/stdc++.h>

using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

template <typename Value>
struct AVLArraySet {
private:
    Value *VAL; // values
    int *HT; // height of subtree
    int *SZ; // size of subtree
    int *L; // index of left child
    int *R; // index of right child

    int root;
    int cnt;

    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    void update(int x) {
        SZ[x] = 1 + SZ[L[x]] + SZ[R[x]];
        HT[x] = 1 + max(HT[L[x]], HT[R[x]]);
    }

    /**
     * Returns the balance factor of the subtree. The balance factor is defined
     * as the difference in height of the left subtree and right subtree, in
     * this order. Therefore, a subtree with a balance factor of -1, 0 or 1 has
     * the AVL property since the heights of the two child subtrees differ by at
     * most one.
     *
     * @param x the subtree
     * @return the balance factor of the subtree
     */
    int balanceFactor(int x) {
        return HT[L[x]] - HT[R[x]];
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
     * Restores the AVL tree property of the subtree.
     *
     * @param x the subtree
     * @return the subtree with restored AVL property
     */
    int balance(int x) {
        if (balanceFactor(x) < -1) {
            if (balanceFactor(R[x]) > 0) R[x] = rotateRight(R[x]);
            x = rotateLeft(x);
        }
        else if (balanceFactor(x) > 1) {
            if (balanceFactor(L[x]) < 0) L[x] = rotateLeft(L[x]);
            x = rotateRight(x);
        }
        update(x);
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
     * Inserts the specified value into the set, allowing for duplicates.
     * Deletes the specified value from this set if the specified value is {@code null}.
     *
     * @param x the subtree
     * @param val the value
     * @return the subtree
     */
    int add(int x, Value val) {
        if (x == 0) {
            VAL[cnt] = val;
            HT[cnt] = 0;
            SZ[cnt] = 1;
            L[cnt] = 0;
            R[cnt] = 0;
            return cnt++;
        }
        if (val < VAL[x]) L[x] = add(L[x], val);
        else R[x] = add(R[x], val);
        return balance(x);
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
        return balance(x);
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
        return balance(x);
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
        return balance(x);
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
     * Initializes an empty set with a fixed size.
     *
     * @param N the maximum size of the set
     */
    AVLArraySet(int N) {
        VAL = new Value[N];
        HT = new int[N];
        SZ = new int[N];
        L = new int[N];
        R = new int[N];
        root = 0;
        HT[root] = -1;
        SZ[root] = 0;
        L[root] = 0;
        R[root] = 0;
        cnt = 1;
    }

    /**
     * Checks if the set is empty.
     *
     * @return {@code true} if the set is empty.
     */
    bool isEmpty() {
        return root == 0;
    }

    /**
     * Returns the number values in the set.
     *
     * @return the number values pairs in the set
     */
    int size() {
        return SZ[root];
    }

    /**
     * Returns the height of the internal AVL tree. It is assumed that the
     * height of an empty tree is -1 and the height of a tree with just one node
     * is 0.
     *
     * @return the height of the internal AVL tree
     */
    int height() {
        return HT[root];
    }

    /**
     * Checks if the set contains the given value.
     *
     * @param val the value
     * @return {@code true} if the set contains {@code val}
     *         and {@code false} otherwise
     */
    bool contains(Value val) {
        return contains(root, val);
    }

    /**
     * Inserts the specified value into the set, allowing for duplicates.
     *
     * @param val the value
     */
    void add(Value val) {
        root = add(root, val);
    }

    /**
     * Removes the specified value from the set
     *
     * @param val the value
     */
    void remove(Value val) {
        if (!contains(val)) return;
        root = remove(root, val);
    }

    /**
     * Removes the smallest value from the set.
     *
     * @throws runtime_error if the set is empty
     */
    void removeMin() {
        if (isEmpty()) throw runtime_error("called removeMin() with empty set");
        root = removeMin(root);
    }

    /**
     * Removes the largest value from the set.
     *
     * @throws runtime_error if the set is empty
     */
    void removeMax() {
        if (isEmpty()) throw runtime_error("called removeMax() with empty set");
        root = removeMax(root);
    }

    /**
     * Returns the smallest value in the set.
     *
     * @return the smallest value in the set
     * @throws runtime_error if the set is empty
     */
    Value getMin() {
        if (isEmpty()) throw runtime_error("called getMin() with empty set");
        return VAL[getMin(root)];
    }

    /**
     * Returns the largest value in the set.
     *
     * @return the largest value in the set
     * @throws runtime_error if the set is empty
     */
    Value getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty set");
        return VAL[getMax(root)];
    }

    /**
     * Returns the largest value in the set less than or equal to {@code val}.
     *
     * @param val the value
     * @return the largest value in the set less than or equal to {@code val}
     * @throws runtime_error if the set is empty
     * @throws no_such_element_exception if there is no value in the set less than or equal to {@code val}
     */
    Value floor(Value val) {
        if (isEmpty()) throw runtime_error("called floor() with empty set");
        int x = floor(root, val);
        if (x == 0) throw no_such_element_exception("call to floor() resulted in no such value");
        return VAL[x];
    }

    /**
     * Returns the smallest value in the set greater than or equal to {@code val}.
     *
     * @param val the value
     * @return a pair containing the smallest value in the set greater than or equal to {@code val}
     * @throws runtime_error if the set is empty
     * @throws no_such_element_exception if there is no value in the set greater than or equal to {@code val}
     */
    Value ceiling(Value val) {
        if (isEmpty()) throw runtime_error("called ceiling() with empty set");
        int x = ceiling(root, val);
        if (x == 0) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return VAL[x];
    }

    /**
     * Returns the kth smallest value in the set.
     *
     * @param k the order statistic
     * @return the kth smallest value in the set
     * @throws invalid_argument unless {@code k} is between 0 and
     *             {@code size() -1 }
     */
    Value select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        return VAL[select(root, k)];
    }

    /**
     * Returns the number of values in the set strictly less than
     * {@code val}.
     *
     * @param val the value
     * @return the number of values in the set strictly less than
     *         {@code val}
     */
    int getRank(Value val) {
        return getRank(root, val);
    }

    /**
     * Returns all values in the set following an in-order traversal.
     *
     * @return all values in the set following an in-order traversal
     */
    vector<Value> values() {
        vector<Value> queue;
        valuesInOrder(root, &queue);
        return queue;
    }

    /**
     * Returns all values in the set in the given range.
     *
     * @param lo the lowest value
     * @param hi the highest value
     * @return all value in the set between {@code lo} (inclusive)
     *         and {@code hi} (exclusive)
     */
    vector<Value> values(Value lo, Value hi) {
        vector<Value> queue;
        values(root, &queue, lo, hi);
        return queue;
    }

    /**
     * Returns the number of values in the set in the given range.
     *
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of values in the set between {@code lo}
     *         (inclusive) and {@code hi} (exclusive)
     */
    int size(Value lo, Value hi) {
        if (lo > hi) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_AVLTREESET_H_ */
