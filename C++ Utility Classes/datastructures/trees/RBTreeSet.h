#ifndef DATASTRUCTURES_TREES_RBTREESET_H_
#define DATASTRUCTURES_TREES_RBTREESET_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// does not work with duplicates
template <typename Value, typename Comparator = less<Value>>
struct RBTreeSet {
private:
    Comparator cmp;
    enum Color {BLACK = 0, RED = 1};

    /**
     * Represents an inner node of the tree.
     */
    struct Node {
        Value val;
        bool color;
        int size;
        Node *left = nullptr, *right = nullptr;
        Node(Value val, bool color, int size) {
            this->val = val;
            this->color = color;
            this->size = size;
        }
    };

    /**
     * The root node.
     */
    Node *root = nullptr;

    /**
     * Returns true if the node is red, false otherwise.
     *
     * @param x the node
     * @return true if the node is red, false otherwise
     */
    bool isRed(Node *&x) {
        if (x == nullptr) return false;
        return x->color == RED;
    }

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
     * Flips the colors of the node and its children
     *
     * @param x the node to flip
     */
    void flipColors(Node *&x) {
        x->color = !x->color;
        x->left->color = !x->left->color;
        x->right->color = !x->right->color;
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
        y->color = y->right->color;
        y->right->color = RED;
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
        y->color = y->left->color;
        y->left->color = RED;
        update(x);
        update(y);
        return y;
    }

    /**
     * If x is red, and both x->left and x->left->left are black,
     * make x->left or one of its children red.
     *
     * @param x the subtree
     * @return updated subtree
     */
    Node *moveRedLeft(Node *&x) {
        flipColors(x);
        if (isRed(x->right->left)) {
            x->right = rotateRight(x->right);
            x = rotateLeft(x);
            flipColors(x);
        }
        return x;
    }

    /**
     * If x is red, and both x->right and x->right->left are black,
     * make x->right or one of its children red.
     *
     * @param x the subtree
     * @return updated subtree
     */
    Node *moveRedRight(Node *&x) {
        flipColors(x);
        if (isRed(x->left->left)) {
            x = rotateRight(x);
            flipColors(x);
        }
        return x;
    }

    /**
     * Balances the red-black subtree.
     *
     * @param x the subtree
     * @param fixRight whether a right-leaning fix is needed
     * @return the balanced subtree
     */
    Node *balance(Node *&x, bool fixRight) {
        if (isRed(x->right) && (!fixRight || !isRed(x->left))) x = rotateLeft(x);
        if (isRed(x->left) && isRed(x->left->left)) x = rotateRight(x);
        if (isRed(x->left) && isRed(x->right)) flipColors(x);
        update(x);
        return x;
    }

    // auxiliary function for contains
    bool contains(Node *&x, Value val) {
        if (x == nullptr) return false;
        else if (cmp(val, x->val)) return contains(x->left, val);
        else if (cmp(x->val, val)) return contains(x->right, val);
        return true;
    }

    /**
     * Inserts the specified value into the set, not allowing for duplicates.
     *
     * @param x the subtree
     * @param val the value
     * @return the subtree
     */
    Node *add(Node *&x, Value val) {
        if (x == nullptr) return new Node(val, RED, 1);
        if (cmp(val, x->val)) x->left = add(x->left, val);
        else if (cmp(x->val, val)) x->right = add(x->right, val);
        else return x;
        update(x);
        return balance(x, true);
    }

    /**
     * Removes the smallest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    Node *removeMin(Node *&x) {
        if (x->left == nullptr) {
            delete x;
            return nullptr;
        }
        if (!isRed(x->left) && !isRed(x->left->left)) x = moveRedLeft(x);
        x->left = removeMin(x->left);
        update(x);
        return balance(x, false);
    }

    /**
     * Removes the largest value from the given subtree.
     *
     * @param x the subtree
     * @return the updated subtree
     */
    Node *removeMax(Node *&x) {
        if (x->right == nullptr) {
            delete x;
            return nullptr;
        }
        if (!isRed(x->right) && !isRed(x->right->left)) x = moveRedRight(x);
        x->right = removeMax(x->right);
        update(x);
        return balance(x, false);
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
     * Removes the specified value and its associated value from the given
     * subtree.
     *
     * @param x the subtree
     * @param val the value
     * @return the updated subtree
     */
    Node *remove(Node *&x, Value val) {
        if (cmp(val, x->val)) {
            if (!isRed(x->left) && !isRed(x->left->left)) x = moveRedLeft(x);
            x->left = remove(x->left, val);
        } else {
            if (isRed(x->left)) x = rotateRight(x);
            if (!cmp(val, x->val) && !cmp(x->val, val) && x->right == nullptr) {
                delete x;
                return nullptr;
            }
            if (!isRed(x->right) && !isRed(x->right->left)) x = moveRedRight(x);
            if (!cmp(val, x->val) && !cmp(x->val, val)) {
                Node *y = getMin(x->right);
                x->val = y->val;
                x->right = removeMin(x->right);
            } else {
                x->right = remove(x->right, val);
            }
        }
        update(x);
        return balance(x, false);
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
    Node *floor(Node *&x, Value val) {
        if (x == nullptr) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(val, x->val)) return floor(x->left, val);
        Node *y = floor(x->right, val);
        if (y != nullptr) return y;
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
    Node *ceiling(Node *&x, Value val) {
        if (x == nullptr) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(x->val, val)) return ceiling(x->right, val);
        Node *y = ceiling(x->left, val);
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
     * Returns the number of values in the subtree less than val.
     *
     * @param val the value
     * @param x the subtree
     * @return the number of values in the subtree less than val
     */
    int getRank(Node *&x, Value val) {
        if (x == nullptr) return 0;
        if (!cmp(x->val, val)) return getRank(x->left, val);
        else return 1 + size(x->left) + getRank(x->right, val);
    }

    /**
     * Adds the values in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void valuesInOrder(Node *&x, vector<Value> &queue) {
        if (x == nullptr) return;
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
    void values(Node *&x, vector<Value> &queue, Value lo, Value hi) {
        if (x == nullptr) return;
        if (cmp(lo, x->val)) values(x->left, queue, lo, hi);
        if (!cmp(x->val, lo) && !cmp(hi, x->val)) queue.push_back(x->val);
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
     * Initializes an empty symbol table.
     */
    RBTreeSet() {}

    /**
     * Deletes the symbol table.
     */
    virtual ~RBTreeSet() {
        clear(root);
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
        return root == nullptr;
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
     * Inserts the specified value into the set, not allowing for duplicates.
     *
     * @param val the value
     */
    void add(Value val) {
        root = add(root, val);
        root->color = BLACK;
    }

    /**
     * Removes the specified value from the set
     *
     * @param val the value
     */
    void remove(Value val) {
        if (!contains(val)) return;
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = remove(root, val);
        if (!isEmpty()) root->color = BLACK;
    }

    /**
     * Removes the smallest value from the symbol table.
     *
     * @throws runtime_error if the symbol table is empty
     */
    void removeMin() {
        if (isEmpty()) throw runtime_error("called removeMin() with empty symbol table");
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = removeMin(root);
        if (!isEmpty()) root->color = BLACK;
    }

    /**
     * Removes the largest value from the symbol table.
     *
     * @throws runtime_error if the symbol table is empty
     */
    void removeMax() {
        if (isEmpty()) throw runtime_error("called removeMax() with empty symbol table");
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = removeMax(root);
        if (!isEmpty()) root->color = BLACK;
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
        if (x == nullptr) throw no_such_element_exception("call to floor() resulted in no such value");
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
        if (x == nullptr) throw no_such_element_exception("call to ceiling() resulted in no such value");
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
        if (cmp(hi, lo)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};

#endif /* DATASTRUCTURES_TREES_RBTREESET_H_ */
