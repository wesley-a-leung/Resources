#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Size Balanced Binary Search (Multi) Set
// Time Complexity:
//   constructor, empty, size: O(1)
//   values: O(N)
//   all other operators: O(log N)
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct SBTSet {
    struct Node {
        Node *l, *r; int size; Value val;
        Node(const Value &val) : l(nullptr), r(nullptr), size(1), val(val) {}
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
    bool contains(Node *x, const Value &val) {
        if (!x) return false;
        else if (cmp(val, x->val)) return contains(x->l, val);
        else if (cmp(x->val, val)) return contains(x->r, val);
        return true;
    }
    Node *add(Node *x, const Value &val) {
        if (!x) { T.emplace_back(val); return &T.back(); }
        if (cmp(val, x->val)) x->l = add(x->l, val);
        else x->r = add(x->r, val);
        update(x); return maintain(x, !cmp(val, x->val));
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
    Node *remove(Node *x, const Value &val) {
        if (cmp(val, x->val)) x->l = remove(x->l, val);
        else if (cmp(x->val, val)) x->r = remove(x->r, val);
        else {
            if (!x->l) return x->r;
            else if (!x->r) return x->l;
            else { Node *y = x; x = getMin(y->r); x->r = removeMin(y->r); x->l = y->l; }
        }
        update(x); return x;
    }
    Node *floor(Node *x, const Value &val) {
        if (!x) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(val, x->val)) return floor(x->l, val);
        Node *y = floor(x->r, val); return y ? y : x;
    }
    Node *ceiling(Node *x, const Value &val) {
        if (!x) return nullptr;
        if (!cmp(val, x->val) && !cmp(x->val, val)) return x;
        if (cmp(x->val, val)) return ceiling(x->r, val);
        Node *y = ceiling(x->l, val); return y ? y : x;
    }
    Node *select(Node *x, int k) {
        if (!x) return nullptr;
        int t = Size(x->l);
        if (t > k) return select(x->l, k);
        else if (t < k) return select(x->r, k - t - 1);
        return x;
    }
    int getRank(Node *x, const Value &val) { // number of values less than first occurrence
        if (!x) return 0;
        if (!cmp(x->val, val)) return getRank(x->l, val);
        else return 1 + Size(x->l) + getRank(x->r, val);
    }
    void valuesInOrder(Node *x, vector<Value> &queue) {
        if (!x) return;
        valuesInOrder(x->l, queue); queue.push_back(x->val); valuesInOrder(x->r, queue);
    }
    void values(Node *x, vector<Value> &queue, const Value &lo, const Value &hi) {
        if (!x) return;
        if (cmp(lo, x->val)) values(x->l, queue, lo, hi);
        if (!cmp(x->val, lo) && !cmp(hi, x->val)) queue.push_back(x->val);
        if (cmp(x->val, hi)) values(x->r, queue, lo, hi);
    }
    void clear() { root = nullptr; T.clear(); }
    bool empty() { return !root; }
    int size() { return Size(root); }
    bool contains(const Value &val) { return contains(root, val); }
    void add(const Value &val) { root = add(root, val); }
    void remove(const Value &val) { if (contains(val)) root = remove(root, val); }
    void removeMin() {
        if (empty()) throw runtime_error("called removeMin() with empty symbol table");
        root = removeMin(root);
    }
    void removeMax() {
        if (empty()) throw runtime_error("called removeMax() with empty symbol table");
        root = removeMax(root);
    }
    Value getMin() {
        if (empty()) throw runtime_error("called getMin() with empty symbol table");
        return getMin(root)->val;
    }
    Value getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        return getMax(root)->val;
    }
    Value floor(const Value &val) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        Node *x = floor(root, val);
        if (!x) throw no_such_element_exception("call to floor() resulted in no such value");
        return x->val;
    }
    Value ceiling(const Value &val) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        Node *x = ceiling(root, val);
        if (!x) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return x->val;
    }
    Value select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        return select(root, k)->val;
    }
    int getRank(const Value &val) { return getRank(root, val); }
    vector<Value> values() { vector<Value> queue; valuesInOrder(root, queue); return queue; }
    vector<Value> values(const Value &lo, const Value &hi) { vector<Value> queue; values(root, queue, lo, hi); return queue; }
    int size(const Value &lo, const Value &hi) {
        if (cmp(hi, lo)) return 0;
        else if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};
