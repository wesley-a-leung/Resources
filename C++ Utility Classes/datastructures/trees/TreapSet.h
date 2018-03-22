#ifndef DATASTRUCTURES_TREES_TREAPSET_H_
#define DATASTRUCTURES_TREES_TREAPSET_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 * Treap Set:
 * To maintain optimized time complexity, memory is not freed for set operations.
 *
 * Size: O(1)
 * Contains / Get: O(log(N))
 * Add: O(log(N))
 * Remove: O(log(N))
 * Get/Remove Min/Max: O(log(N))
 * Floor/Ceiling/Select/GetRank: O(log(N))
 * Merge/Split: O(log(N))
 * SetUnion/SetIntersect/SetDifference: O(M log(N / M)), M < N
 * Values: O(N)
 */
template <typename Value, typename Comparator = less<Value>>
struct TreapSet {
private:
    mt19937 *gen;
    uniform_real_distribution<double> *dis;
    Comparator cmp;

    double randDouble() {
        return (*dis)(*gen);
    }

    /**
     * Represents an inner node of the treap.
     */
    struct Node {
        Value val;
        double priority;
        int size;
        Node *left = nullptr, *right = nullptr;
        Node(Value val, double priority, int size) {
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

    // auxiliary function for contains
    bool contains(Node *&x, Value val) {
        if (nullptr == x) return false;
        else if (cmp(val, x->val)) return contains(x->left, val);
        else if (cmp(x->val, val)) return contains(x->right, val);
        return true;
    }

    /**
     * Splits the subtree into 2 smaller subtrees l and r where l contains
     * all elements less than val and r contains all elements not less than val.
     *
     * @param x the subtree to split
     * @param val the value to compare to
     * @param l the reference to the subtree containing all elements less than val
     * @param r the reference to the subtree containing all elements not less than val
     */
    void split(Node *x, Value val, Node *&l, Node *&r) {
        if (nullptr == x) {
            l = nullptr;
            r = nullptr;
        } else if (cmp(val, x->val)) {
            split(x->left, val, l, x->left);
            r = x;
        } else {
            split(x->right, val, x->right, r);
            l = x;
        }
        update(x);
    }

    /**
     * Merges 2 subtrees into a new tree without violating the priority ordering
     * or the value ordering. l and r must be ordered correctly and all elements
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
     * Inserts the specified value into the set, allowing for duplicates.
     *
     * @param x the subtree
     * @param y the node to be added
     */
    void add(Node *&x, Node *y) {
        if (nullptr == x) x = y;
        else if (x->priority < y->priority) {
            split(x, y->val, y->left, y->right);
            x = y;
        } else {
            add(cmp(y->val, x->val) ? x->left : x->right, y);
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
     * Returns the node with the smallest value in the subtree.
     *
     * @param x the subtree
     * @return the node with the smallest value in the subtree
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
     * Removes the specified value and its associated value from the given
     * subtree.
     *
     * @param x the subtree
     * @param val the value to be removed
     */
    void remove(Node *&x, Value val) {
        if (!cmp(val, x->val) && !cmp(x->val, val)) {
            Node *y = x;
            merge(x, x->left, x->right);
            delete y;
        }
        else remove(cmp(val, x->val) ? x->left : x->right, val);
        update(x);
    }

    /**
     * Splits the subtree into 2 smaller subtrees l and r where l contains
     * all elements less than val and r contains all elements greater than val,
     * as well as a node eq with a value equal to val.
     *
     * @param x the subtree to split
     * @param val the value to compare to
     * @param l the reference to the subtree containing all elements less than val
     * @param eq the reference to a node with a value equal to val
     * @param r the reference to the subtree containing all elements not less than val
     */
    void split3(Node *x, Value val, Node *&l, Node *&eq, Node *&r) {
        if (nullptr == x) {
            l = nullptr;
            r = nullptr;
            eq = nullptr;
        } else if (cmp(val, x->val)) {
            split3(x->left, val, l, eq, x->left);
            r = x;
        } else if (cmp(x->val, val)) {
            split3(x->right, val, x->right, eq, r);
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
        split3(r, l->val, lt, eq, gt);
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
        split3(r, l->val, lt, eq, gt);
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
        split3(r, l->val, lt, eq, gt);
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
     * Returns the node in the subtree with the largest value less than or equal
     * to the given value.
     *
     * @param x the subtree
     * @param val the value
     * @return the node in the subtree with the largest value less than or equal
     *         to the given value
     */
    Node *floor(Node *x, Value val) {
        if (nullptr == x) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(val, x->val)) return floor(x->left, val);
        Node *y = floor(x->right, val);
        if (nullptr != y) return y;
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
    Node *ceiling(Node *x, Value val) {
        if (nullptr == x) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(x->val, val)) return ceiling(x->right, val);
        Node *y = ceiling(x->left, val);
        if (nullptr != y) return y;
        else return x;
    }

    /**
     * Returns the node with value the kth smallest value in the subtree.
     *
     * @param x the subtree
     * @param k the kth smallest value in the subtree
     * @return the node with value the kth smallest value in the subtree
     */
    Node *select(Node *x, int k) {
        if (nullptr == x) return nullptr;
        int t = size(x->left);
        if (t > k) return select(x->left, k);
        else if (t < k) return select(x->right, k - t - 1);
        return x;
    }

    /**
     * Returns the number of values in the subtree less than val.
     *
     * @param val the value
     * @param x the subtree
     * @return the number of values in the subtree less than val
     */
    int getRank(Node *x, Value val) {
        if (nullptr == x) return 0;
        if (!cmp(x->val, val)) return getRank(x->left, val);
        else return 1 + size(x->left) + getRank(x->right, val);
    }

    /**
     * Adds the values in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void valuesInOrder(Node *x, vector<Value> &queue) {
        if (nullptr == x) return;
        valuesInOrder(x->left, queue);
        queue.push_back(x->val);
        valuesInOrder(x->right, queue);
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
    void values(Node *x, vector<Value> &queue, Value lo, Value hi) {
        if (nullptr == x) return;
        if (cmp(lo, x->val)) values(x->left, queue, lo, hi);
        if (!cmp(x->val, lo) && cmp(hi, x->val)) queue.push_back(x->val);
        if (cmp(x->val, hi)) values(x->right, queue, lo, hi);
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
    TreapSet() {
        gen = new mt19937(time(0));
        dis = new uniform_real_distribution<double>(0.0, 1.0);
    }

    /**
     * Deletes the symbol table.
     */
    virtual ~TreapSet() {
        clear(root);
        delete gen;
        delete dis;
    }

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
        Node *x = new Node(val, randDouble(), 1);
        add(root, x);
    }

    /**
     * Inserts the specified value into the set, not allowing for duplicates.
     *
     * @param val the value
     */
    void addUnique(Value val) {
        if (contains(root, val)) return;
        Node *x = new Node(val, randDouble(), 1);
        add(root, x);
    }

    /**
     * Removes one instance of the specified value from the set
     *
     * @param val the value
     */
    void remove(Value val) {
        if (!contains(val)) return;
        remove(root, val);
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
     * Returns the smallest value in the set.
     *
     * @return the smallest value in the set
     * @throws runtime_error if the set is empty
     */
    Value getMin() {
        if (isEmpty()) throw runtime_error("called getMin() with empty set");
        return getMin(root)->val;
    }

    /**
     * Returns the largest value in the set.
     *
     * @return the largest value in the set
     * @throws runtime_error if the set is empty
     */
    Value getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty set");
        return getMax(root)->val;
    }

    /**
     * Performs a set union on 2 treaps. Returns a new treap containing a union
     * of the elements in the treaps, keeping only 1 value if it appears in both treaps.
     * Treaps a and b should not be accessed after this.
     *
     * @param a the first treap
     * @param b the second treap
     * @return pointer to a treap that is the union of the 2 treaps.
     */
    template <typename V, typename C = less<V>>
    static TreapSet<V, C> *setUnion(TreapSet<V, C> *a, TreapSet<V, C> *b) {
        TreapSet<V, C> *c = new TreapSet<V, C>();
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
    template <typename V, typename C = less<V>>
    static TreapSet<V, C> *setIntersect(TreapSet<V, C> *a, TreapSet<V, C> *b) {
        TreapSet<V, C> *c = new TreapSet<V, C>();
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
    template <typename V, typename C = less<V>>
    static TreapSet<V, C> *setDifference(TreapSet<V, C> *a, TreapSet<V, C> *b) {
        TreapSet<V, C> *c = new TreapSet<V, C>();
        c->setDifference(c->root, a->root, b->root, true);
        a = nullptr;
        b = nullptr;
        return c;
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
        Node *x = floor(root, val);
        if (nullptr == x) throw no_such_element_exception("call to floor() resulted in no such value");
        return x->val;
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
        Node *x = ceiling(root, val);
        if (nullptr == x) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return x->val;
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
        return select(root, k)->val;
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
        valuesInOrder(root, queue);
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
        values(root, queue, lo, hi);
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
        if (cmp(lo, hi)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_TREAPSET_H_ */
