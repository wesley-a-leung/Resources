#ifndef DATASTRUCTURES_TREES_SBT_H_
#define DATASTRUCTURES_TREES_SBT_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

template <typename Key, typename Value, typename Comparator = less<Key>>
struct SBT {
private:
    Comparator cmp;

    /**
     * Represents an inner node of the tree.
     */
    struct Node {
        Key key;
        Value val;
        int size;
        Node *left = nullptr, *right = nullptr;
        Node(Key key, Value val, int size) {
            this->key = key;
            this->val = val;
            this->size = size;
        }
    };

    /**
     * The root node.
     */
    Node *root = nullptr;

    /**
     * Returns the number of nodes in the subtree.
     *
     * @param x the subtree
     * @return the number of nodes in the subtree
     */
    int size(Node *&x) {
        if (x == nullptr) return 0;
        return x->size;
    }


    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    void update(Node *&x) {
        x->size = 1 + size(x->left) + size(x->right);
    }

    /**
     * Rotates the given subtree to the right.
     *
     * @param x the subtree
     * @return the right rotated subtree
     */
    Node *rotateRight(Node *&x) {
        Node *y = x->left;
        x->left = y->right;
        y->right = x;
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
    Node *rotateLeft(Node *&x) {
        Node *y = x->right;
        x->right = y->left;
        y->left = x;
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
    Node *maintain(Node *&x, bool flag) {
        if (flag) {
            if (x->right == nullptr) return x;
            if (size(x->left) < size(x->right->left)) {
                x->right = rotateRight(x->right);
                x = rotateLeft(x);
            } else if (size(x->left) < size(x->right->right)) {
                x = rotateLeft(x);
            } else {
                return x;
            }
        } else {
            if (x->left == nullptr) return x;
            if (size(x->right) < size(x->left->right)) {
                x->left = rotateLeft(x->left);
                x = rotateRight(x);
            } else if (size(x->right) < size(x->left->left)) {
                x = rotateRight(x);
            } else {
                return x;
            }
        }
        x->left = maintain(x->left, false);
        x->right = maintain(x->right, true);
        x = maintain(x, true);
        x = maintain(x, false);
        return x;
    }

    /**
     * Returns value associated with the given key in the subtree.
     *
     * @param x the subtree
     * @param key the key
     * @return value associated with the given key in the subtree or {@code nullptr} if no such key
     * @throws no_such_element_exception if there is no such key
     */
    Node *get(Node *&x, Key key) {
        if (x == nullptr) return nullptr;
        if (cmp(key, x->key)) return get(x->left, key);
        else if (cmp(x->key, key)) return get(x->right, key);
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
    Node *put(Node *&x, Key key, Value val) {
        if (x == nullptr) return new Node(key, val, 1);
        if (cmp(key, x->key)) x->left = put(x->left, key, val);
        else if (cmp(x->key, key)) x->right = put(x->right, key, val);
        else {
            x->val = val;
            return x;
        }
        update(x);
        return maintain(x, key > x->key);
    }

    /**
     * Removes the smallest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    Node *removeMin(Node *&x) {
        if (x->left == nullptr) return x->right;
        x->left = removeMin(x->left);
        update(x);
        return x;
    }

    /**
     * Removes the largest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    Node *removeMax(Node *&x) {
        if (x->right == nullptr) return x->left;
        x->right = removeMax(x->right);
        update(x);
        return x;
    }

    /**
     * Returns the node with the smallest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the smallest value in the subtree
     */
    Node *getMin(Node *&x) {
        if (x->left == nullptr) return x;
        return getMin(x->left);
    }

    /**
     * Returns the node with the largest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the largest value in the subtree
     */
    Node *getMax(Node *&x) {
        if (x->right == nullptr) return x;
        return getMax(x->right);
    }

    /**
     * Removes the specified key and its associated value from the given subtree.
     *
     * @param x the subtree
     * @param key the key
     * @return the updated subtree
     */
    Node *remove(Node *&x, Key key) {
        if (cmp(key, x->key)) x->left = remove(x->left, key);
        else if (cmp(x->key, key)) x->right = remove(x->right, key);
        else {
            if (x->left == nullptr) return x->right;
            else if (x->right == nullptr) return x->left;
            else {
                Node *y = x;
                x = getMin(y->right);
                x->right = removeMin(y->right);
                x->left = y->left;
                free(y);
            }
        }
        update(x);
        return x;
    }

    /**
     * Returns the node in the subtree with the largest key less than or equal to the given key.
     *
     * @param x the subtree
     * @param key the key
     * @return the node in the subtree with the largest key less than or equal to the given key
     */
    Node *floor(Node *&x, Key key) {
        if (x == nullptr) return nullptr;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(key, x->key)) return floor(x->left, key);
        Node *y = floor(x->right, key);
        if (y != nullptr) return y;
        else return x;
    }

    /**
     * Returns the node in the subtree with the smallest key greater than or equal to the given key.
     *
     * @param x the subtree
     * @param key the key
     * @return the node in the subtree with the smallest key greater than or equal to the given key
     */
    Node *ceiling(Node *&x, Key key) {
        if (x == nullptr) return nullptr;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(x->key, key)) return ceiling(x->right, key);
        Node *y = ceiling(x->left, key);
        if (y != nullptr) return y;
        else return x;
    }

    /**
     * Returns the node with value the kth smallest value in the subtree.
     *
     * @param x the subtree
     * @param k the kth smallest value in the subtree
     * @return the node with value the kth smallest value in the subtree
     */
    Node *select(Node *&x, int k) {
        if (x == nullptr) return nullptr;
        int t = size(x->left);
        if (t > k) return select(x->left, k);
        else if (t < k) return select(x->right, k - t - 1);
        return x;
    }

    /**
     * Returns the number of keys in the subtree less than key.
     *
     * @param key the key
     * @param x the subtree
     * @return the number of keys in the subtree less than key
     */
    int getRank(Node *&x, Key key) {
        if (x == nullptr) return 0;
        if (cmp(key, x->key)) return getRank(x->left, key);
        else if (cmp(x->key, key)) return 1 + size(x->left) + getRank(x->right, key);
        else return size(x->left);
    }

    /**
     * Adds the key-value pairs in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void keyValuePairsInOrder(Node *&x, vector<pair<Key, Value>> &queue) {
        if (x == nullptr) return;
        keyValuePairsInOrder(x->left, queue);
        queue.push_back({x->key, x->val});
        keyValuePairsInOrder(x->right, queue);
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
    void keyValuePairs(Node *&x, vector<pair<Key, Value>> &queue, Key lo, Key hi) {
        if (x == nullptr) return;
        if (cmp(lo, x->key)) keyValuePairs(x->left, queue, lo, hi);
        if (!cmp(x->key, lo) && !cmp(hi, x->key)) queue.push_back({x->key, x->val});
        if (cmp(x->key, hi)) keyValuePairs(x->right, queue, lo, hi);
    }

public:
    /**
     * Initializes an empty symbol table.
     */
    SBT() {}

    /**
     * Checks if the symbol table is empty.
     *
     * @return {@code true} if the symbol table is empty.
     */
    bool isEmpty() {
        return root == nullptr;
    }

    /**
     * Returns the number key-value pairs in the symbol table.
     *
     * @return the number key-value pairs in the symbol table
     */
    int size() {
        return size(root);
    }

    /**
     * Returns the value associated with the given key.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     * @throws no_such_element_exception if no such key is in the symbol table
     */
    Value get(Key key) {
        Node *x = get(root, key);
        if (x == nullptr) throw no_such_element_exception("no such key is in the symbol table");
        return x->val;
    }

    /**
     * Checks if the symbol table contains the given key.
     *
     * @param key the key
     * @return {@code true} if the symbol table contains {@code key} and {@code false} otherwise
     */
    bool contains(Key key) {
        return get(root, key) != nullptr;
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
        Node *x = getMin(root);
        return {x->key, x->val};
    }

    /**
     * Returns the largest key in the symbol table and its associated value.
     *
     * @return the largest key in the symbol table and its associated value
     * @throws runtime_error if the symbol table is empty
     */
    pair<Key, Value> getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty symbol table");
        Node *x = getMax(root);
        return {x->key, x->val};
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
        Node *x = floor(root, key);
        if (x == nullptr) throw no_such_element_exception("call to floor() resulted in no such value");
        return {x->key, x->val};
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
        Node *x = ceiling(root, key);
        if (x == nullptr) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {x->key, x->val};
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
        Node *x = select(root, k);
        return {x->key, x->val};
    }

    /**
     * Returns the number of keys in the symbol table strictly less than {@code key}.
     *
     * @param key the key
     * @return the number of keys in the symbol table strictly less than {@code key}
     */
    int getRank(Key key) {
        return getRank(root, key);
    }

    /**
     * Returns all key-value pairs in the symbol table following an in-order traversal.
     *
     * @return all key-value pairs in the symbol table following an in-order traversal
     */
    vector<pair<Key, Value>> &keyValuePairs() {
        vector<pair<Key, Value>> queue;
        keyValuePairsInOrder(root, queue);
        return queue;
    }

    /**
     * Returns all key-value pairs in the symbol table in the given range.
     *
     * @param lo the lowest key
     * @param hi the highest key
     * @return all key-value pairs in the symbol table between {@code lo} (inclusive) and {@code hi} (exclusive)
     */
    vector<pair<Key, Value>> &keyValuePairs(Key lo, Key hi) {
        vector<pair<Key, Value>> queue;
        keyValuePairs(root, queue, lo, hi);
        return queue;
    }

    /**
     * Returns the number of keys in the symbol table in the given range.
     *
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of keys in the symbol table between {@code lo} (inclusive) and {@code hi} (exclusive)
     */
    int size(Key lo, Key hi) {
        if (cmp(hi, lo)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_SBT_H_ */
