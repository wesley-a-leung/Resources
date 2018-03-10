#ifndef DATASTRUCTURES_TREES_SBTARRAY_H_
#define DATASTRUCTURES_TREES_SBTARRAY_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

template <typename Key, typename Value, typename Comparator = less<Key>>
struct SBTArray {
private:
    Comparator cmp;

    Key *KEY; // keys
    Value *VAL; // values
    int *SZ; // size of subtree
    int *L; // index of left child
    int *R; // index of right child

    int root;
    int ind; // current index
    int capacity; // capacity
    const int INIT_CAPACITY = 4; // default capacity

    /**
     * Resizes the arrays and copies all the keys and values
     */
    void resize() {
        Key *NEW_KEY = new Key[capacity * 2];
        Value *NEW_VAL = new Value[capacity * 2];
        int *NEW_SZ = new int[capacity * 2];
        int *NEW_L = new int[capacity * 2];
        int *NEW_R = new int[capacity * 2];
        for (int i = 0; i < capacity; i++) {
            NEW_KEY[i] = KEY[i];
            NEW_VAL[i] = VAL[i];
            NEW_SZ[i] = SZ[i];
            NEW_L[i] = L[i];
            NEW_R[i] = R[i];
        }
        swap(NEW_KEY, KEY);
        swap(NEW_VAL, VAL);
        swap(NEW_SZ, SZ);
        swap(NEW_L, L);
        swap(NEW_R, R);
        delete[](NEW_KEY);
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

    /**
     * Returns the node associated with the given key in the subtree.
     *
     * @param x the subtree
     * @param key the key
     * @return the node associated with the given key in the subtree or 0 if no such key
     */
    int get(int x, Key key) {
        if (x == 0) return 0;
        if (cmp(key, KEY[x])) return get(L[x], key);
        else if (cmp(KEY[x], key)) return get(R[x], key);
        else return x;
    }

    /**
     * Inserts the key-value pair in the subtree. It overrides the old value
     * with the new value if the symbol table already contains the specified key.
     *
     * @param x the subtree
     * @param key the key
     * @param val the value
     * @return the subtree
     */
    int put(int x, Key key, Value val) {
        if (x == 0) {
            if (ind >= capacity) resize();
            KEY[ind] = key;
            VAL[ind] = val;
            SZ[ind] = 1;
            L[ind] = 0;
            R[ind] = 0;
            return ind++;
        }
        if (cmp(key, KEY[x])) L[x] = put(L[x], key, val);
        else if (cmp(KEY[x], key)) R[x] = put(R[x], key, val);
        else {
            VAL[x] = val;
            return x;
        }
        update(x);
        return maintain(x, key > KEY[x]);
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
     * Removes the specified key and its associated value from the given
     * subtree.
     *
     * @param x the subtree
     * @param key the key
     * @return the updated subtree
     */
    int remove(int x, Key key) {
        if (cmp(key, KEY[x])) L[x] = remove(L[x], key);
        else if (cmp(KEY[x], key)) R[x] = remove(R[x], key);
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
     * Returns the node in the subtree with the largest key less than or equal
     * to the given key.
     *
     * @param x the subtree
     * @param key the key
     * @return the node in the subtree with the largest key less than or equal
     *         to the given key
     */
    int floor(int x, Key key) {
        if (x == 0) return 0;
        if (!cmp(key, KEY[x]) && !cmp(KEY[x], key)) return x;
        if (cmp(key, KEY[x])) return floor(L[x], key);
        int y = floor(R[x], key);
        if (y != 0) return y;
        else return x;
    }

    /**
     * Returns the node in the subtree with the smallest key greater than or
     * equal to the given key.
     *
     * @param x the subtree
     * @param key the key
     * @return the node in the subtree with the smallest key greater than or
     *         equal to the given key
     */
    int ceiling(int x, Key key) {
        if (x == 0) return 0;
        if (!cmp(key, KEY[x]) && !cmp(KEY[x], key)) return x;
        if (cmp(KEY[x], key)) return ceiling(R[x], key);
        int y = ceiling(L[x], key);
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
     * Returns the number of keys in the subtree less than key.
     *
     * @param key the key
     * @param x the subtree
     * @return the number of keys in the subtree less than key
     */
    int getRank(int x, Key key) {
        if (x == 0) return 0;
        if (cmp(key, KEY[x])) return getRank(KEY[x], key);
        else if (cmp(KEY[x], key)) return 1 + SZ[L[x]] + getRank(R[x], key);
        else return SZ[L[x]];
    }

    /**
     * Adds the key-value pairs in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void keyValuePairsInOrder(int x, vector<pair<Key, Value>> &queue) {
        if (x == 0) return;
        keyValuePairsInOrder(L[x], queue);
        queue.push_back({KEY[x], VAL[x]});
        keyValuePairsInOrder(R[x], queue);
    }

    /**
     * Adds the key-value pairs between {@code lo} and {@code hi} in the subtree
     * to the {@code queue}.
     *
     * @param x the subtree
     * @param queue the queue
     * @param lo the lowest key
     * @param hi the highest key
     */
    void keyValuePairs(int x, vector<pair<Key, Value>> &queue, Key lo, Key hi) {
        if (x == 0) return;
        if (cmp(lo, KEY[x])) keyValuePairs(L[x], queue, lo, hi);
        if (!cmp(KEY[x], lo) && !cmp(hi, KEY[x])) queue.push_back({KEY[x], VAL[x]});
        if (cmp(KEY[x], hi)) keyValuePairs(R[x], queue, lo, hi);
    }

public:
    /**
     * Initializes an empty symbol table with an initial capacity of 4.
     */
    SBTArray() {
        KEY = new Key[INIT_CAPACITY];
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
     * Initializes an empty symbol table with an initial capacity.
     *
     * @param N the initial capacity of the symbol table
     */
    SBTArray(int N) {
        assert(N >= 0);
        N++; // zero node is never used
        KEY = new Key[N];
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
     * Deletes the symbol table.
     */
    ~SBTArray() {
        delete[](KEY);
        delete[](VAL);
        delete[](SZ);
        delete[](L);
        delete[](R);
    }

    /**
     * Clears the symbol table.
     */
    void clear() {
        for (int i = 0; i < capacity; i++) {
            L[i] = 0;
            R[i] = 0;
            SZ[i] = 0;
        }
        root = 0;
        SZ[root] = 0;
        L[root] = 0;
        R[root] = 0;
        ind = 1;
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
     * Returns the number key-value pairs in the symbol table.
     *
     * @return the number key-value pairs in the symbol table
     */
    int size() {
        return SZ[root];
    }

    /**
     * Returns the value associated with the given key.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the
     *         symbol table
     * @throws no_such_element_exception if no such key is in the symbol table
     */
    Value get(Key key) {
        int x = get(root, key);
        if (x == 0) throw no_such_element_exception("no such key is in the symbol table");
        return VAL[x];
    }

    /**
     * Checks if the symbol table contains the given key.
     *
     * @param key the key
     * @return {@code true} if the symbol table contains {@code key}
     *         and {@code false} otherwise
     */
    bool contains(Key key) {
        return get(root, key) != 0;
    }

    /**
     * Inserts the specified key-value pair into the symbol table, overwriting
     * the old value with the new value if the symbol table already contains the
     * specified key.
     *
     * @param key the key
     * @param val the value
     */
    void put(Key key, Value val) {
        root = put(root, key, val);
    }

    /**
     * Removes the specified key and its associated value from the symbol table
     * (if the key is in the symbol table).
     *
     * @param key the key
     */
    void remove(Key key) {
        if (!contains(key)) return;
        root = remove(root, key);
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
     * Returns the smallest key in the symbol table and its associated value.
     *
     * @return the smallest key in the symbol table and its associated value
     * @throws runtime_error if the symbol table is empty
     */
    pair<Key, Value> getMin() {
        if (isEmpty()) throw runtime_error("called getMin() with empty symbol table");
        int x = getMin(root);
        return {KEY[x], VAL[x]};
    }

    /**
     * Returns the largest key in the symbol table and its associated value.
     *
     * @return the largest key in the symbol table and its associated value
     * @throws runtime_error if the symbol table is empty
     */
    pair<Key, Value> getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty symbol table");
        int x = getMax(root);
        return {KEY[x], VAL[x]};
    }

    /**
     * Returns the largest key in the symbol table less than or equal to {@code key} and its associated value.
     *
     * @param key the key
     * @return the largest key in the symbol table less than or equal to {@code key} and its associated value
     * @throws runtime_error if the symbol table is empty
     * @throws no_such_element_exception if there is no key in the symbol table less than or equal to {@code key}
     */
    pair<Key, Value> floor(Key key) {
        if (isEmpty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, key);
        if (x == 0) throw no_such_element_exception("call to floor() resulted in no such value");
        return {KEY[x], VAL[x]};
    }

    /**
     * Returns the smallest key in the symbol table greater than or equal to {@code key} and its associated value.
     *
     * @param key the key
     * @return a pair containing the smallest key in the symbol table greater than or equal to {@code key} and its associated value
     * @throws runtime_error if the symbol table is empty
     * @throws no_such_element_exception if there is no key in the symbol table greater than or equal to {@code key}
     */
    pair<Key, Value> ceiling(Key key) {
        if (isEmpty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, key);
        if (x == 0) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {KEY[x], VAL[x]};
    }

    /**
     * Returns the kth smallest key in the symbol table and its associated value.
     *
     * @param k the order statistic
     * @return the kth smallest key in the symbol table and its associated value
     * @throws invalid_argument unless {@code k} is between 0 and
     *             {@code size() -1 }
     */
    pair<Key, Value> select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        int x = select(root, k);
        return {KEY[x], VAL[x]};
    }

    /**
     * Returns the number of keys in the symbol table strictly less than
     * {@code key}.
     *
     * @param key the key
     * @return the number of keys in the symbol table strictly less than
     *         {@code key}
     */
    int getRank(Key key) {
        return getRank(root, key);
    }

    /**
     * Returns all key-value pairs in the symbol table following an in-order traversal.
     *
     * @return all key-value pairs in the symbol table following an in-order traversal
     */
    vector<pair<Key, Value>> keyValuePairs() {
        vector<pair<Key, Value>> queue;
        keyValuePairsInOrder(root, queue);
        return queue;
    }

    /**
     * Returns all key-value pairs in the symbol table in the given range.
     *
     * @param lo the lowest key
     * @param hi the highest key
     * @return all key-value pairs in the symbol table between {@code lo} (inclusive)
     *         and {@code hi} (exclusive)
     */
    vector<pair<Key, Value>> keyValuePairs(Key lo, Key hi) {
        vector<pair<Key, Value>> queue;
        keyValuePairs(root, queue, lo, hi);
        return queue;
    }

    /**
     * Returns the number of keys in the symbol table in the given range.
     *
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of keys in the symbol table between {@code lo}
     *         (inclusive) and {@code hi} (exclusive)
     */
    int size(Key lo, Key hi) {
        if (cmp(hi, lo)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_SBTARRAY_H_ */
