#ifndef DATASTRUCTURES_TREES_TREAP_H_
#define DATASTRUCTURES_TREES_TREAP_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Treap:
 * To maintain optimized time complexity, memory is not freed for set operations.
 *
 * Size: O(1)
 * Contains / Get: O(log(N))
 * Put: O(log(N))
 * Remove: O(log(N))
 * Get/Remove Min/Max: O(log(N))
 * Floor/Ceiling/Select/GetRank: O(log(N))
 * Merge/Split: O(log(N))
 * SetUnion/SetIntersect/SetDifference: O(M log(N / M)), M < N
 * Key Value Pairs: O(N)
 */
template <typename Key, typename Value, typename Comparator = less<Key>>
struct Treap {
private:
    seed_seq seq {
        (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
        (uint64_t) __builtin_ia32_rdtsc(),
        (uint64_t) (uintptr_t) make_unique<char>().get()
    };
    mt19937 gen;
    uniform_real_distribution<double> dis;
    Comparator cmp;

    /**
     * Represents an inner node of the treap.
     */
    struct Node {
        Key key;
        Value val;
        double priority;
        int size;
        Node *left = nullptr, *right = nullptr;
        Node(Key key, Value val, double priority, int size) {
            this->key = key;
            this->val = val;
            this->priority = priority;
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
     *
     * @return the number of nodes in the subtree
     */
    int size(Node *x) {
        if (nullptr == x) return 0;
        return x->size;
    }

    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    void update(Node *x) {
        if (nullptr != x) x->size = 1 + size(x->left) + size(x->right);
    }

    /**
     * Returns the node associated with the given key in the subtree.
     *
     * @param x the subtree
     * @param key the key
     * @return the node associated with the given key in the subtree or {@code nullptr} if no such key
     */
    Node *get(Node *&x, Key key) {
        if (nullptr == x) return nullptr;
        if (cmp(key, x->key)) return get(x->left, key);
        else if (cmp(x->key, key)) return get(x->right, key);
        else return x;
    }

    /**
     * Splits the subtree into 2 smaller subtrees l and r where l contains
     * all elements not greater than key and r contains all elements greater than key.
     *
     * @param x the subtree to split
     * @param key the keyue to compare to
     * @param l the reference to the subtree containing all elements not greater than key
     * @param r the reference to the subtree containing all elements greater than key
     */
    void split(Node *x, Key key, Node *&l, Node *&r) {
        if (nullptr == x) {
            l = nullptr;
            r = nullptr;
        } else if (cmp(key, x->key)) {
            split(x->left, key, l, x->left);
            r = x;
        } else {
            split(x->right, key, x->right, r);
            l = x;
        }
        update(x);
    }

    /**
     * Merges 2 subtrees into a new tree without violating the priority ordering
     * or the key ordering. l and r must be ordered correctly and all elements
     * in l are less than or equal to all elements in r.
     *
     * @param x the merged subtree
     * @param l the left subtree to be merged
     * @param r the right subtree to be merged
     */
    void merge(Node *&x, Node *l, Node *r) {
        if (nullptr == l || nullptr == r) {
            x = nullptr == l ? r : l;
        } else if (l->priority > r->priority) {
            merge(l->right, l->right, r);
            x = l;
        } else {
            merge(r->left, l, r->left);
            x = r;
        }
        update(x);
    }

    /**
     * Inserts the specified key-value pair into the set, allowing for duplicates.
     *
     * @param x the subtree
     * @param y the node to be added
     */
    void put(Node *&x, Node *y) {
        if (nullptr == x) x = y;
        else if (x->priority < y->priority) {
            split(x, y->key, y->left, y->right);
            x = y;
        } else {
            put(cmp(y->key, x->key) ? x->left : x->right, y);
        }
        update(x);
    }

    /**
     * Removes the smallest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    void removeMin(Node *&x) {
        if (nullptr == x->left) {
            Node *y = x;
            x = x->right;
            delete y;
            return;
        }
        removeMin(x->left);
        update(x);
    }

    /**
     * Removes the largest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    void removeMax(Node *&x) {
        if (nullptr == x->right) {
            Node *y = x;
            x = x->left;
            delete y;
            return;
        }
        removeMax(x->right);
        update(x);
    }

    /**
     * Returns the node with the smallest key in the subtree.
     *
     * @param x the subtree
     * @return the node with the smallest key in the subtree
     */
    Node *getMin(Node *x) {
        if (nullptr == x->left) return x;
        return getMin(x->left);
    }

    /**
     * Returns the node with the largest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the largest value in the subtree
     */
    Node *getMax(Node *x) {
        if (nullptr == x->right) return x;
        return getMax(x->right);
    }

    /**
     * Removes the specified value and its associated key from the given
     * subtree.
     *
     * @param x the subtree
     * @param key the key to be removed
     */
    void remove(Node *&x, Key key) {
        if (!cmp(key, x->key) && !cmp(x->key, key)) {
            Node *y = x;
            merge(x, x->left, x->right);
            delete y;
        }
        else remove(cmp(key, x->key) ? x->left : x->right, key);
        update(x);
    }

    /**
     * Splits the subtree into 2 smaller subtrees l and r where l contains
     * all elements less than key and r contains all elements greater than key,
     * as well as a node eq with a keyue equal to key.
     *
     * @param x the subtree to split
     * @param key the keyue to compare to
     * @param l the reference to the subtree containing all elements less than key
     * @param eq the reference to a node with a keyue equal to key
     * @param r the reference to the subtree containing all elements not less than key
     */
    void split3(Node *x, Key key, Node *&l, Node *&eq, Node *&r) {
        if (nullptr == x) {
            l = nullptr;
            r = nullptr;
            eq = nullptr;
        } else if (cmp(key, x->key)) {
            split3(x->left, key, l, eq, x->left);
            r = x;
        } else if (cmp(x->key, key)) {
            split3(x->right, key, x->right, eq, r);
            l = x;
        } else {
            l = x->left;
            r = x->right;
            eq = x;
        }
        update(x);
    }

    /**
     * Performs a set union on the subtrees l and r.
     *
     * @param x the union of l and r
     * @param l the first subtree
     * @param r the second subtree
     */
    void setUnion(Node *&x, Node *l, Node *r) {
        if (nullptr == l || nullptr == r) {
            x = nullptr == l ? r : l;
            return;
        }
        if (l->priority < r->priority) swap(l, r);
        Node *lt, *eq, *gt;
        split3(r, l->key, lt, eq, gt);
        x = l;
        setUnion(x->left, l->left, lt);
        setUnion(x->right, l->right, gt);
        update(x);
    }

    /**
     * Performs a set intersection on the subtrees l and r.
     *
     * @param x the intersection of l and r
     * @param l the first subtree
     * @param r the second subtree
     */
    void setIntersect(Node *&x, Node *l, Node *r) {
        if (nullptr == l || nullptr == r) {
            x = nullptr;
            return;
        }
        if (l->priority < r->priority) swap(l, r);
        Node *lt, *eq, *gt, *left, *right;
        split3(r, l->key, lt, eq, gt);
        setIntersect(left, l->left, lt);
        setIntersect(right, l->right, gt);
        if (nullptr == eq) {
            merge(x, left, right);
        } else {
            x = l;
            x->left = left;
            x->right = right;
        }
        update(x);
    }

    /**
     * Performs a set intersection on the subtrees l and r.
     *
     * @param x the intersection of l and r
     * @param l the first subtree
     * @param r the second subtree
     */
    void setDifference(Node *&x, Node *l, Node *r, bool subtractR) {
        if (nullptr == l || nullptr == r) {
            x = subtractR ? l : r;
            return;
        }
        if (l->priority < r->priority) {
            swap(l, r);
            subtractR = !subtractR;
        }
        Node *lt, *eq, *gt, *left, *right;
        split3(r, l->key, lt, eq, gt);
        setDifference(left, l->left, lt, subtractR);
        setDifference(right, l->right, gt, subtractR);
        if (nullptr == eq && subtractR) {
            x = l;
            x->left = left;
            x->right = right;
        } else {
            merge(x, left, right);
        }
        update(x);
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
    Node *floor(Node *x, Key key) {
        if (nullptr == x) return nullptr;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(key, x->key)) return floor(x->left, key);
        Node *y = floor(x->right, key);
        if (nullptr != y) return y;
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
    Node *ceiling(Node *x, Key key) {
        if (nullptr == x) return nullptr;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(x->key, key)) return ceiling(x->right, key);
        Node *y = ceiling(x->left, key);
        if (nullptr != y) return y;
        else return x;
    }

    /**
     * Returns the node with key the kth smallest key in the subtree.
     *
     * @param x the subtree
     * @param k the kth smallest key in the subtree
     * @return the node with key the kth smallest key in the subtree
     */
    Node *select(Node *x, int k) {
        if (nullptr == x) return nullptr;
        int t = size(x->left);
        if (t > k) return select(x->left, k);
        else if (t < k) return select(x->right, k - t - 1);
        return x;
    }

    /**
     * Returns the number of key in the subtree less than key.
     *
     * @param key the key
     * @param x the subtree
     * @return the number of keys in the subtree less than key
     */
    int getRank(Node *x, Key key) {
        if (nullptr == x) return 0;
        if (!cmp(x->key, key)) return getRank(x->left, key);
        else return 1 + size(x->left) + getRank(x->right, key);
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

    /**
     * Clears the symbol table.
     * @param x the subtree
     */
    void clear(Node *x) {
        if (x == nullptr) return;
        clear(x->left);
        clear(x->right);
        delete x;
        x = nullptr;
    }

public:
    /**
     * Initializes an empty set.
     */
    Treap() : gen(seq), dis(0.0, 1.0) {}

    /**
     * Clears the symbol table.
     */
    void clear() {
        clear(root);
        root = nullptr;
    }

    /**
     * Checks if the set is empty.
     *
     * @return {@code true} if the set is empty.
     */
    bool isEmpty() {
        return nullptr == root;
    }

    /**
     * Returns the number values in the set.
     *
     * @return the number values pairs in the set
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
        if (nullptr == x) throw no_such_element_exception("no such key is in the symbol table");
        return x->val;
    }

    /**
     * Checks if the symbol table contains the given key.
     *
     * @param key the key
     * @return {@code true} if the symbol table contains {@code key} and {@code false} otherwise
     */
    bool contains(Key key) {
        return nullptr != get(root, key);
    }

    /**
     * Inserts the specified key-value pair into the symbol table, allowing for duplicate keys.
     *
     * @param key the key
     * @param val the value
     * @return true if the key-value pair was inserted, false otherwise
     */
    bool put(Key key, Value val) {
        if (contains(root, key)) return false;
        Node *x = new Node(key, val, dis(gen), 1);
        put(root, x);
        return true;
    }

    /**
     * Removes one instance of the specified key from the set
     *
     * @param key the key
     */
    void remove(Key key) {
        if (!contains(key)) return;
        remove(root, key);
    }

    /**
     * Removes the smallest value from the set.
     *
     * @throws runtime_error if the set is empty
     */
    void removeMin() {
        if (isEmpty()) throw runtime_error("called removeMin() with empty set");
        removeMin(root);
    }

    /**
     * Removes the largest value from the set.
     *
     * @throws runtime_error if the set is empty
     */
    void removeMax() {
        if (isEmpty()) throw runtime_error("called removeMax() with empty set");
        removeMax(root);
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
     * Performs a set union on 2 treaps. Returns a new treap containing a union
     * of the elements in the treaps, keeping only 1 key if it appears in both treaps.
     * Treaps a and b should not be accessed after this.
     *
     * @param a the first treap
     * @param b the second treap
     * @return pointer to a treap that is the union of the 2 treaps.
     */
    template <typename K, typename V, typename C = less<K>>
    static Treap<K, V, C> *setUnion(Treap<K, V, C> *a, Treap<K, V, C> *b) {
        Treap<K, V, C> *c = new Treap<K, V, C>();
        c->setUnion(c->root, a->root, b->root);
        a = nullptr;
        b = nullptr;
        return c;
    }

    /**
     * Performs a set intersect on 2 treaps. Returns a new treap containing only
     * elements that appear in both treaps. Treaps a and b should not be accessed after this.
     *
     * @param a the first treap
     * @param b the second treap
     * @return pointer to a treap that is the intersection of the 2 treaps.
     */
    template <typename K, typename V, typename C = less<K>>
    static Treap<K, V, C> *setIntersect(Treap<K, V, C> *a, Treap<K, V, C> *b) {
        Treap<K, V, C> *c = new Treap<K, V, C>();
        c->setIntersect(c->root, a->root, b->root);
        a = nullptr;
        b = nullptr;
        return c;
    }

    /**
     * Performs a set difference on 2 treaps. Returns a new treap containing only
     * elements in the first treap that do not appear in the second treap.
     * Treaps a and b should not be accessed after this.
     *
     * @param a the first treap
     * @param b the second treap
     * @return pointer to a treap that is the difference of the 2 treaps.
     */
    template <typename K, typename V, typename C = less<K>>
    static Treap<K, V, C> *setDifference(Treap<K, V, C> *a, Treap<K, V, C> *b) {
        Treap<K, V, C> *c = new Treap<K, V, C>();
        c->setDifference(c->root, a->root, b->root, true);
        a = nullptr;
        b = nullptr;
        return c;
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
     * @return all key-value pairs in the symbol table between {@code lo} (inclusive) and {@code hi} (exclusive)
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
     * @return the number of keys in the symbol table between {@code lo} (inclusive) and {@code hi} (exclusive)
     */
    int size(Key lo, Key hi) {
        if (cmp(hi, lo)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_TREAP_H_ */
