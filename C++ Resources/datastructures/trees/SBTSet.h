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
        int l, r, size; Value val;
        Node(const Value &val) : l(-1), r(-1), size(1), val(val) {}
    };
    int root = -1; vector<Node> T; Comparator cmp;
    int Size(int x) { return x == -1 ? 0 : T[x].size; }
    void update(int x) { T[x].size = 1 + Size(T[x].l) + Size(T[x].r); }
    int rotateRight(int x) { int y = T[x].l; T[x].l = T[y].r; T[y].r = x; update(x); update(y); return y; }
    int rotateLeft(int x) { int y = T[x].r; T[x].r = T[y].l; T[y].l = x; update(x); update(y); return y; }
    int maintain(int x, bool flag) {
        if (flag) {
            if (T[x].r == -1) return x;
            else if (Size(T[x].l) < Size(T[T[x].r].l)) { T[x].r = rotateRight(T[x].r); x = rotateLeft(x); }
            else if (Size(T[x].l) < Size(T[T[x].r].r)) x = rotateLeft(x);
            else return x;
        } else {
            if (T[x].l == -1) return x;
            else if (Size(T[x].r) < Size(T[T[x].l].r)) { T[x].l = rotateLeft(T[x].l); x = rotateRight(x); }
            else if (Size(T[x].r) < Size(T[T[x].l].l)) x = rotateRight(x);
            else return x;
        }
        T[x].l = maintain(T[x].l, false); T[x].r = maintain(T[x].r, true); x = maintain(x, true); x = maintain(x, false); return x;
    }
    bool contains(int x, const Value &val) {
        if (x == -1) return false;
        else if (cmp(val, T[x].val)) return contains(T[x].l, val);
        else if (cmp(T[x].val, val)) return contains(T[x].r, val);
        return true;
    }
    int add(int x, const Value &val) {
        if (x == -1) { T.emplace_back(val); return int(T.size()) - 1; }
        if (cmp(val, T[x].val)) { int l = add(T[x].l, val); T[x].l = l; }
        else { int r = add(T[x].r, val); T[x].r = r; }
        update(x); return maintain(x, !cmp(val, T[x].val));
    }
    int removeMin(int x) {
        if (T[x].l == -1) return T[x].r;
        T[x].l = removeMin(T[x].l); update(x); return x;
    }
    int removeMax(int x) {
        if (T[x].r == -1) return T[x].l;
        T[x].r = removeMax(T[x].r); update(x); return x;
    }
    int getMin(int x) { return T[x].l == -1 ? x : getMin(T[x].l); }
    int getMax(int x) { return T[x].r == -1 ? x : getMax(T[x].r); }
    int remove(int x, const Value &val) {
        if (cmp(val, T[x].val)) T[x].l = remove(T[x].l, val);
        else if (cmp(T[x].val, val)) T[x].r = remove(T[x].r, val);
        else {
            if (T[x].l == -1) return T[x].r;
            else if (T[x].r == -1) return T[x].l;
            else { int y = x; x = getMin(T[y].r); T[x].r = removeMin(T[y].r); T[x].l = T[y].l; }
        }
        update(x); return x;
    }
    int floor(int x, const Value &val) {
        if (x == -1) return -1;
        if (!cmp(val, T[x].val) && !cmp(T[x].val, val)) return x;
        if (cmp(val, T[x].val)) return floor(T[x].l, val);
        int y = floor(T[x].r, val); return y == -1 ? x : y;
    }
    int ceiling(int x, const Value &val) {
        if (x == -1) return -1;
        if (!cmp(val, T[x].val) && !cmp(T[x].val, val)) return x;
        if (cmp(T[x].val, val)) return ceiling(T[x].r, val);
        int y = ceiling(T[x].l, val); return y == -1 ? x : y;
    }
    int select(int x, int k) {
        if (x == -1) return -1;
        int t = Size(T[x].l);
        if (t > k) return select(T[x].l, k);
        else if (t < k) return select(T[x].r, k - t - 1);
        return x;
    }
    int getRank(int x, const Value &val) { // number of values less than first occurrence
        if (x == -1) return 0;
        if (!cmp(T[x].val, val)) return getRank(T[x].l, val);
        else return 1 + Size(T[x].l) + getRank(T[x].r, val);
    }
    void valuesInOrder(int x, vector<Value> &queue) {
        if (x == -1) return;
        valuesInOrder(T[x].l, queue); queue.push_back(T[x].val); valuesInOrder(T[x].r, queue);
    }
    void values(int x, vector<Value> &queue, const Value &lo, const Value &hi) {
        if (x == -1) return;
        if (cmp(lo, T[x].val)) values(T[x].l, queue, lo, hi);
        if (!cmp(T[x].val, lo) && !cmp(hi, T[x].val)) queue.push_back(T[x].val);
        if (cmp(T[x].val, hi)) values(T[x].r, queue, lo, hi);
    }
    void clear() { root = -1; T.clear(); }
    bool empty() { return root == -1; }
    int size() { return Size(root); }
    void reserve(int n) { T.reserve(n); }
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
        return T[getMin(root)].val;
    }
    Value getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        return T[getMax(root)].val;
    }
    Value floor(const Value &val) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, val);
        if (x == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        return T[x].val;
    }
    Value ceiling(const Value &val) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, val);
        if (x == -1) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return T[x].val;
    }
    Value select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        return T[select(root, k)].val;
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
