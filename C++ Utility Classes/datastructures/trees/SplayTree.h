#ifndef DATASTRUCTURES_TREES_SPLAYTREE_H_
#define DATASTRUCTURES_TREES_SPLAYTREE_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

template <typename Key, typename Value, typename Comparator = less<Key>>
struct SplayTree {
private:
    Comparator cmp;

    // BST helper node data type
    struct Node {
        Key key;
        Value val;
        Node *left = nullptr, *right = nullptr;   // left and right subtrees
        int size;
        Node(Key key, Value val, int size) {
            this->key = key;
            this->val = val;
            this->size = size;
        }
    };

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

    void traverse(Node *&x, vector<pair<Key, Value>> &order) {
        if (x == nullptr) return;
        traverse(x->left, order);
        order.push_back({x->key, x->val});
        traverse(x->right, order);
    }

    /***************************************************************************
     * Splay tree function.
     * **********************************************************************/
     // splay key in the tree rooted at Node x. If a node with that key exists,
     //   it is splayed to the root of the tree. If it does not, the last node
     //   along the search path for the key is splayed to the root.
     Node *splay(Node *&x, Key key) {
         if (x == nullptr) return nullptr;
         if (cmp(key, x->key)) {
             // key not in tree, so we're done
             if (x->left == nullptr) return x;
             if (cmp(key, x->left->key)) {
                 x->left->left = splay(x->left->left, key);
                 x = rotateRight(x);
             } else if (cmp(x->left->key, key)) {
                 x->left->right = splay(x->left->right, key);
                 if (x->left->right != nullptr) x->left = rotateLeft(x->left);
             }
             if (x->left == nullptr) return x;
             else return rotateRight(x);
         } else if (cmp(x->key, key)) {
             // key not in tree, so we're done
             if (x->right == nullptr) return x;
             if (cmp(key, x->right->key)) {
                 x->right->left  = splay(x->right->left, key);
                 if (x->right->left != nullptr) x->right = rotateRight(x->right);
             } else if (cmp(x->right->key, key)) {
                 x->right->right = splay(x->right->right, key);
                 x = rotateLeft(x);
             }
             if (x->right == nullptr) return x;
             else return rotateLeft(x);
         } else {
             return x;
         }
     }

     void clear(Node *x) {
         if (x == nullptr) return;
         clear(x->left);
         clear(x->right);
         delete x;
         x = nullptr;
     }

public:
    SplayTree() {}

    ~SplayTree() {
        clear(root);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    int size() {
        return size(root);
    }

    // return value associated with the given key
    // if no such value, return null
    Value get(Key key) {
        root = splay(root, key);
        if (!cmp(key, root->key) && !cmp(root->key, key)) return root->val;
        else throw no_such_element_exception("no such key is in the symbol table");
    }

    bool contains(Key key) {
        try {
            get(key);
            return true;
        } catch (no_such_element_exception &e) {
            return false;
        }
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
     * Returns the number of keys in the symbol table strictly less than
     * {@code key}.
     *
     * @param key the key
     * @return the number of keys in the symbol table strictly less than
     *         {@code key}
     */
    int getRank(Key key) {
        root = splay(root, key);
        return getRank(root, key);
    }


   /***************************************************************************
    *  Splay tree insertion.
    ***************************************************************************/
    void put(Key key, Value val) {
        // splay key to root
        if (root == nullptr) {
            root = new Node(key, val, 1);
            return;
        }
        root = splay(root, key);
        // Insert new node at root
        if (cmp(key, root->key)) {
            Node *n = new Node(key, val, 1);
            n->left = root->left;
            n->right = root;
            root->left = nullptr;
            update(root);
            root = n;
        }
        // Insert new node at root
        else if (cmp(root->key, key)) {
            Node *n = new Node(key, val, 1);
            n->right = root->right;
            n->left = root;
            root->right = nullptr;
            update(root);
            root = n;
        }
        // It was a duplicate key-> Simply replace the value
        else {
            root->val = val;
        }
        update(root);
    }

   /***************************************************************************
    *  Splay tree deletion.
    ***************************************************************************/
    /* This splays the key, then does a slightly modified Hibbard deletion on
     * the root (if it is the node to be deleted; if it is not, the key was
     * not in the tree). The modification is that rather than swapping the
     * root (call it node A) with its successor, it's successor (call it Node B)
     * is moved to the root position by splaying for the deletion key in A's
     * right subtree. Finally, A's right child is made the new root's right
     * child.
     */
    void remove(Key key) {
        if (root == nullptr) return; // empty tree
        root = splay(root, key);
        if (!cmp(key, root->key) && !cmp(root->key, key)) {
            if (root->left == nullptr) {
                Node *x = root->right;
                delete root;
                root = x;
            } else {
                Node *x = root->right;
                Node *y = root->left;
                delete root;
                root = splay(y, key);
                root->right = x;
            }
            update(root);
        }
        // else: it wasn't in the tree to remove
    }

    /**
     * Returns an iterator that iterates over the key-value pairs in order
     *
     * @return an iterator that iterates over the key-value pairs in order
     */
    vector<pair<Key, Value>> keyValuePairs() {
        vector<pair<Key, Value>> order;
        traverse(root, order);
        return order;
    }
};

#endif /* DATASTRUCTURES_TREES_SPLAYTREE_H_ */
