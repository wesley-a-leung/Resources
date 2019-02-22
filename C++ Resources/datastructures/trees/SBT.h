#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Size Balanced Binary Search Tree (no duplicates allowed)
// Time Complexity:
//   constructor, empty, size: O(1)
//   keyValuePairs: O(N)
//   all other operators: O(log N)
// Memory Complexity: O(N)
template <class Key, class Value, class Comparator = less<Key>> struct SBT {
    struct Node {
        Node *l, *r; int size; Key key; Value val;
        Node(const Key &key, const Value &val) : l(nullptr), r(nullptr), size(1), key(key), val(val) {}
    };
    Node *root = nullptr; deque<Node> T; Comparator cmp;
    int Size(Node *x) { return x ? x->size : 0; }
    void update(Node *x) { x->size = 1 + Size(x->l) + Size(x->r); }
    Node *rotateRight(Node *x) { Node *y = x->l; x->l = y->r; y->r = x; update(x); update(y); return y; }
    Node *rotateLeft(Node *x) { Node *y = x->r; x->r = y->l; y->l = x; update(x); update(y); return y; }
    Node *maintain(Node *x, bool flag) {
        if (flag) {
            if (!x->r) return x;
            else if (Size(x->l) < Size(x->r->l)) { x->r = rotateRight(x->r); x = rotateLeft(x); }
            else if (Size(x->l) < Size(x->r->r)) x = rotateLeft(x);
            else return x;
        } else {
            if (!x->l) return x;
            else if (Size(x->r) < Size(x->l->r)) { x->l = rotateLeft(x->l); x = rotateRight(x); }
            else if (Size(x->r) < Size(x->l->l)) x = rotateRight(x);
            else return x;
        }
        x->l = maintain(x->l, false); x->r = maintain(x->r, true); x = maintain(x, true); x = maintain(x, false); return x;
    }
    Node *get(Node *x, const Key &key) {
        if (!x) return 0;
        if (cmp(key, x->key)) return get(x->l, key);
        else if (cmp(x->key, key)) return get(x->r, key);
        else return x;
    }
    Node *put(Node *x, const Key &key, const Value &val) {
        if (!x) { T.emplace_back(key, val); return &T.back(); }
        if (cmp(key, x->key)) x->l = put(x->l, key, val);
        else if (cmp(x->key, key)) x->r = put(x->r, key, val);
        else { x->val = val; return x; }
        update(x); return maintain(x, key > x->key);
    }
    Node *removeMin(Node *x) {
        if (!x->l) return x->r;
        x->l = removeMin(x->l); update(x); return x;
    }
    Node *removeMax(Node *x) {
        if (!x->r) return x->l;
        x->r = removeMax(x->r); update(x); return x;
    }
    Node *getMin(Node *x) { return x->l ? getMin(x->l) : x; }
    Node *getMax(Node *x) { return x->r ? getMax(x->r) : x; }
    Node *remove(Node *x, const Key &key) {
        if (cmp(key, x->key)) x->l = remove(x->l, key);
        else if (cmp(x->key, key)) x->r = remove(x->r, key);
        else {
            if (!x->l) return x->r;
            else if (!x->r) return x->l;
            else { Node *y = x; x = getMin(y->r); x->r = removeMin(y->r); x->l = y->l; }
        }
        update(x); return x;
    }
    Node *floor(Node *x, const Key &key) {
        if (!x) return 0;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(key, x->key)) return floor(x->l, key);
        Node *y = floor(x->r, key); return y ? y : x;
    }
    Node *ceiling(Node *x, const Key &key) {
        if (!x) return 0;
        if (!cmp(key, x->key) && !cmp(x->key, key)) return x;
        if (cmp(x->key, key)) return ceiling(x->r, key);
        Node *y = ceiling(x->l, key); return y ? y : x;
    }
    Node *select(Node *x, int k) {
        if (!x) return 0;
        int t = Size(x->l);
        if (t > k) return select(x->l, k);
        else if (t < k) return select(x->r, k - t - 1);
        return x;
    }
    int getRank(Node *x, const Key &key) { // number of keys less than key
        if (!x) return 0;
        if (cmp(key, x->key)) return getRank(x->l, key);
        else if (cmp(x->key, key)) return 1 + Size(x->l) + getRank(x->r, key);
        else return Size(x->l);
    }
    void keyValuePairsInOrder(Node *x, vector<pair<Key, Value>> &queue) {
        if (!x) return;
        keyValuePairsInOrder(x->l, queue); queue.push_back({x->key, x->val}); keyValuePairsInOrder(x->r, queue);
    }
    void keyValuePairs(Node *x, vector<pair<Key, Value>> &queue, const Key &lo, const Key &hi) {
        if (!x) return;
        if (cmp(lo, x->key)) keyValuePairs(x->l, queue, lo, hi);
        if (!cmp(x->key, lo) && !cmp(hi, x->key)) queue.push_back({x->key, x->val});
        if (cmp(x->key, hi)) keyValuePairs(x->r, queue, lo, hi);
    }
    void clear() { root = nullptr; T.clear(); }
    bool empty() { return !root; }
    int size() { return Size(root); }
    Value get(const Key &key) {
        Node *x = get(root, key);
        if (!x) throw no_such_element_exception("no such key is in the symbol table");
        return x->val;
    }
    bool contains(const Key &key) { return get(root, key); }
    void put(const Key &key, const Value &val) { root = put(root, key, val); }
    void remove(const Key &key) { if (contains(key)) root = remove(root, key); }
    void removeMin() {
        if (empty()) throw runtime_error("called removeMin() with empty symbol table");
        root = removeMin(root);
    }
    void removeMax() {
        if (empty()) throw runtime_error("called removeMax() with empty symbol table");
        root = removeMax(root);
    }
    pair<Key, Value> getMin() {
        if (empty()) throw runtime_error("called getMin() with empty symbol table");
        Node *x = getMin(root); return {x->key, x->val};
    }
    pair<Key, Value> getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        Node *x = getMax(root); return {x->key, x->val};
    }
    pair<Key, Value> floor(const Key &key) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        Node *x = floor(root, key);
        if (!x) throw no_such_element_exception("call to floor() resulted in no such value");
        return {x->key, x->val};
    }
    pair<Key, Value> ceiling(const Key &key) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        Node *x = ceiling(root, key);
        if (!x) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {x->key, x->val};
    }
    pair<Key, Value> select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        Node *x = select(root, k); return {x->key, x->val};
    }
    int getRank(const Key &key) { return getRank(root, key); }
    vector<pair<Key, Value>> keyValuePairs() {
        vector<pair<Key, Value>> queue; keyValuePairsInOrder(root, queue); return queue;
    }
    vector<pair<Key, Value>> keyValuePairs(const Key &lo, const Key &hi) {
        vector<pair<Key, Value>> queue; keyValuePairs(root, queue, lo, hi); return queue;
    }
    int size(const Key &lo, const Key &hi) {
        if (cmp(hi, lo)) return 0;
        else if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};
