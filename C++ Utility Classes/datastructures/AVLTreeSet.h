/*
 * AVLTreeSet.h
 *
 *  Created on: Jul 5, 2017
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
struct AVLTreeSet {
    /**
     * Represents an inner node of the AVL tree.
     */
    struct Node {
    public:
        Value val;
        int height, size;
        Node *left = nullptr, *right = nullptr;
        Node(Value val, int height, int size) {
            this->val = val;
            this->size = size;
            this->height = height;
        }
    };

private:
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
    int size(Node *&x) {
        if (x == nullptr) return 0;
        return x->size;
    }

    /**
     * Returns the height of the subtree.
     *
     * @param x the subtree
     *
     * @return the height of the subtree.
     */
    int height(Node *&x) {
        if (x == nullptr) return -1;
        return x->height;
    }

    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    void update(Node *&x) {
        x->size = 1 + size(x->left) + size(x->right);
        x->height = 1 + max(height(x->left), height(x->right));
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
    int balanceFactor(Node *&x) {
        return height(x->left) - height(x->right);
    }

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
     * Restores the AVL tree property of the subtree.
     *
     * @param x the subtree
     * @return the subtree with restored AVL property
     */
    Node *balance(Node *&x) {
        if (balanceFactor(x) < -1) {
            if (balanceFactor(x->right) > 0) x->right = rotateRight(x->right);
            x = rotateLeft(x);
        }
        else if (balanceFactor(x) > 1) {
            if (balanceFactor(x->left) < 0) x->left = rotateLeft(x->left);
            x = rotateRight(x);
        }
        update(x);
        return x;
    }

    // auxiliary function for contains
    bool contains(Node *&x, Value val) {
        if (x == nullptr) return false;
        else if (val < x->val) return contains(x->left, val);
        else if (val > x->val) return contains(x->right, val);
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
    Node *add(Node *&x, Value val) {
        if (x == nullptr) return new Node(val, 0, 1);
        if (val < x->val) x->left = add(x->left, val);
        else x->right = add(x->right, val);
        return balance(x);
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
        return balance(x);
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
        return balance(x);
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
        if (val < x->val) x->left = remove(x->left, val);
        else if (val > x->val) x->right = remove(x->right, val);
        else {
            if (x->left == nullptr) return x->right;
            else if (x->right == nullptr) return x->left;
            else {
                Node *y = x;
                x = getMin(y->right);
                x->right = removeMin(y->right);
                x->left = y->left;
            }
        }
        return balance(x);
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
        if (val == x->val) return x;
        if (val < x->val) return floor(x->left, val);
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
        if (val == x->val) return x;
        if (val > x->val) return ceiling(x->right, val);
        Node *y = ceiling(x->left, val);
        if (y != nullptr) return y;
        else return x;
    }

    // auxiliary function for select
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
    int rank(Node *&x, Value val) {
        if (x == nullptr) return -1;
        if (val == x->val) {
            int temp = rank(x->left, val);
            if (temp == -1) return size(x->left);
            else return temp;
        } else if (val < x->val) {
            return rank(x->left, val);
        } else {
            int temp = rank(x->right, val);
            if (temp == -1) return temp;
            else return size(x->left) + 1 + temp;
        }
    }

    /**
     * Adds the values in the subtree to queue following an in-order traversal.
     *
     * @param x the subtree
     * @param queue the queue
     */
    void valuesInOrder(Node *&x, vector<Value> *queue) {
        if (x == nullptr) return;
        valuesInOrder(x->left, queue);
        queue->push_back(x->val);
        valuesInOrder(x->right, queue);
    }

public:
    /**
     * Initializes an empty symbol table.
     */
    AVLTreeSet() {}

    /**
     * Checks if the symbol table is empty.
     *
     * @return {@code true} if the symbol table is empty.
     */
    bool isEmpty() {
        return root == nullptr;
    }

    /**
     * Returns the number values in the symbol table.
     *
     * @return the number values pairs in the symbol table
     */
    int size() {
        return size(root);
    }

    /**
     * Returns the height of the internal AVL tree. It is assumed that the
     * height of an empty tree is -1 and the height of a tree with just one node
     * is 0.
     *
     * @return the height of the internal AVL tree
     */
    int height() {
        return height(root);
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
        root = deleteMax(root);
    }

    /**
     * Returns the smallest value in the symbol table.
     *
     * @return the smallest value in the symbol table
     * @throws runtime_error if the symbol table is empty
     */
    Value getMin() {
        if (isEmpty()) throw runtime_error("called getMin() with empty symbol table");
        return getMin(root).val;
    }

    /**
     * Returns the largest value in the symbol table.
     *
     * @return the largest value in the symbol table
     * @throws runtime_error if the symbol table is empty
     */
    Value getMax() {
        if (isEmpty()) throw runtime_error("called getMax() with empty symbol table");
        return getMax(root).val;
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
        Node *x = floor(root, val);
        if (x == nullptr) throw no_such_element_exception("call to floor() resulted in no such value");
        else return x->val;
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
        Node *x = ceiling(root, val);
        if (x == nullptr) throw no_such_element_exception("call to ceiling() resulted in no such value");
        else return x->val;
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
        return select(root, k)->val;
    }

    /**
     * Returns the number of values in the symbol table strictly less than
     * {@code val}.
     *
     * @param val the value
     * @return the number of values in the symbol table strictly less than
     *         {@code val}
     */
    int rank(Value val) {
        return rank(root, val);
    }

    /**
     * Returns all values in the symbol table following an in-order traversal.
     *
     * @return all values in the symbol table following an in-order traversal
     */
    vector<Value> values() {
        vector<Value> queue;
        valuesInOrder(root, &queue);
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
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else return rank(hi) - rank(lo);
    }
};

#endif /* DATASTRUCTURES_AVLTREESET_H_ */
