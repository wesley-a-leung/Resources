#ifndef DATASTRUCTURES_TREES_SPLAYTREESET_H_
#define DATASTRUCTURES_TREES_SPLAYTREESET_H_

#include <bits/stdc++.h>
using namespace std;

template <typename Value>
struct SplayTreeSet {
    // BST helper node data type
    struct Node {
        Value val;
        Node *left = nullptr, *right = nullptr;   // left and right subtrees
        int size;
        Node(Value val, int size) {
            this->val = val;
            this->size = size;
        }
    };

private:
    Node *root = nullptr;   // root of the BST

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
        if (x == nullptr) return;
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

    // auxiliary method for select
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
        if (val <= x->val) return getRank(x->left, val);
        else return 1 + size(x->left) + getRank(x->right, val);
    }

    void traverse(Node *&x, vector<Value> &order) {
        if (x == nullptr) return;
        traverse(x->left, order);
        order.push_back(x->val);
        traverse(x->right, order);
    }

    /***************************************************************************
     * Splay tree function.
     * **********************************************************************/
     // splay value in the tree rooted at Node x. If a node with that value exists,
     //   it is splayed to the root of the tree. If it does not, the last node
     //   along the search path for the value is splayed to the root.
     Node *splay(Node *&x, Value val) {
         if (x == nullptr) return nullptr;
         if (val < x->val) {
             // key not in tree, so we're done
             if (x->left == nullptr) return x;
             if (val < x->left->val) {
                 x->left->left = splay(x->left->left, val);
                 x = rotateRight(x);
             } else if (val > x->left->val) {
                 x->left->right = splay(x->left->right, val);
                 if (x->left->right != nullptr) x->left = rotateLeft(x->left);
             }
             if (x->left == nullptr) return x;
             else return rotateRight(x);
         } else if (val > x->val) {
             // key not in tree, so we're done
             if (x->right == nullptr) return x;
             if (val < x->right->val) {
                 x->right->left = splay(x->right->left, val);
                 if (x->right->left != nullptr) x->right = rotateRight(x->right);
             } else if (val > x->right->val) {
                 x->right->right = splay(x->right->right, val);
                 x = rotateLeft(x);
             }
             if (x->right == nullptr) return x;
             else return rotateLeft(x);
         } else {
             return x;
         }
     }

public:
    SplayTreeSet() {}

    int size() {
        return size(root);
    }

    bool contains(Value val) {
        if (root == nullptr) return false;
        root = splay(root, val);
        return val == root->val;
    }

    /**
     * Returns the kth smallest value in the symbol table.
     *
     * @param k the order statistic
     * @return the kth smallest value in the symbol table
     * @throws IllegalArgumentException unless {@code k} is between 0 and
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
    int getRank(Value val) {
        root = splay(root, val);
        return getRank(root, val);
    }

   /***************************************************************************
    *  Splay tree insertion.
    ***************************************************************************/
    void add(Value val) {
        // splay value to root
        if (root == nullptr) {
            root = new Node(val, 1);
            return;
        }
        root = splay(root, val);
        // Insert new node at root
        if (val < root->val) {
            Node *n = new Node(val, 1);
            n->left = root->left;
            n->right = root;
            root->left = nullptr;
            update(root);
            root = n;
        }
        // Insert new node at root
        else {
            Node *n = new Node(val, 1);
            n->right = root->right;
            n->left = root;
            root->right = nullptr;
            update(root);
            root = n;
        }
        update(root);
    }

   /***************************************************************************
    *  Splay tree deletion.
    ***************************************************************************/
    /* This splays the value, then does a slightly modified Hibbard deletion on
     * the root (if it is the node to be deleted; if it is not, the value was
     * not in the tree). The modification is that rather than swapping the
     * root (call it node A) with its successor, it's successor (call it Node B)
     * is moved to the root position by splaying for the deletion value in A's
     * right subtree. Finally, A's right child is made the new root's right
     * child.
     */
    void remove(Value val) {
        if (root == nullptr) return; // empty tree
        root = splay(root, val);
        if (val == root->val) {
            if (root->left == nullptr) {
                root = root->right;
            } else {
                Node *x = root->right;
                root = splay(root->left, val);
                root->right = x;
            }
            update(root);
        }
        // else: it wasn't in the tree to remove
    }

    /**
     * Returns an iterator that iterates over the values in order
     *
     * @return an iterator that iterates over the values in order
     */
    vector<Value> &values() {
        vector<Value> order;
        traverse(root, order);
        return order;
    }
};

#endif /* DATASTRUCTURES_TREES_SPLAYTREESET_H_ */
