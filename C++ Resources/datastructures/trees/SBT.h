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
        int l, r, size; Key key; Value val;
        Node(const Key &key, const Value &val) : l(-1), r(-1), size(1), key(key), val(val) {}
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
    int get(int x, const Key &key) {
        if (x == -1) return 0;
        if (cmp(key, T[x].key)) return get(T[x].l, key);
        else if (cmp(T[x].key, key)) return get(T[x].r, key);
        else return x;
    }
    int put(int x, const Key &key, const Value &val) {
        if (x == -1) { T.emplace_back(key, val); return int(T.size()) - 1; }
        if (cmp(key, T[x].key)) { int l = put(T[x].l, key, val); T[x].l = l; }
        else if (cmp(T[x].key, key)) { int r = put(T[x].r, key, val); T[x].r = r; }
        else { T[x].val = val; return x; }
        update(x); return maintain(x, key > T[x].key);
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
    int remove(int x, const Key &key) {
        if (cmp(key, T[x].key)) T[x].l = remove(T[x].l, key);
        else if (cmp(T[x].key, key)) T[x].r = remove(T[x].r, key);
        else {
            if (T[x].l == -1) return T[x].r;
            else if (T[x].r == -1) return T[x].l;
            else { int y = x; x = getMin(T[y].r); T[x].r = removeMin(T[y].r); T[x].l = T[y].l; }
        }
        update(x); return x;
    }
    int floor(int x, const Key &key) {
        if (x == -1) return -1;
        if (!cmp(key, T[x].key) && !cmp(T[x].key, key)) return x;
        if (cmp(key, T[x].key)) return floor(T[x].l, key);
        int y = floor(T[x].r, key); return y == -1 ? x : y;
    }
    int ceiling(int x, const Key &key) {
        if (x == -1) return -1;
        if (!cmp(key, T[x].key) && !cmp(T[x].key, key)) return x;
        if (cmp(T[x].key, key)) return ceiling(T[x].r, key);
        int y = ceiling(T[x].l, key); return y == -1 ? x : y;
    }
    int select(int x, int k) {
        if (x == -1) return -1;
        int t = Size(T[x].l);
        if (t > k) return select(T[x].l, k);
        else if (t < k) return select(T[x].r, k - t - 1);
        return x;
    }
    int getRank(int x, const Key &key) { // number of keys less than key
        if (x == -1) return 0;
        if (cmp(key, T[x].key)) return getRank(T[x].l, key);
        else if (cmp(T[x].key, key)) return 1 + Size(T[x].l) + getRank(T[x].r, key);
        else return Size(T[x].l);
    }
    void keyValuePairsInOrder(int x, vector<pair<Key, Value>> &queue) {
        if (x == -1) return;
        keyValuePairsInOrder(T[x].l, queue); queue.push_back({T[x].key, T[x].val}); keyValuePairsInOrder(T[x].r, queue);
    }
    void keyValuePairs(int x, vector<pair<Key, Value>> &queue, const Key &lo, const Key &hi) {
        if (x == -1) return;
        if (cmp(lo, T[x].key)) keyValuePairs(T[x].l, queue, lo, hi);
        if (!cmp(T[x].key, lo) && !cmp(hi, T[x].key)) queue.push_back({T[x].key, T[x].val});
        if (cmp(T[x].key, hi)) keyValuePairs(T[x].r, queue, lo, hi);
    }
    void clear() { root = -1; T.clear(); }
    bool empty() { return root == -1; }
    int size() { return Size(root); }
    void reserve(int n) { T.reserve(n); }
    Value get(const Key &key) {
        int x = get(root, key);
        if (x == -1) throw no_such_element_exception("no such key is in the symbol table");
        return T[x].val;
    }
    bool contains(const Key &key) { return get(root, key) != 0; }
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
        int x = getMin(root); return {T[x].key, T[x].val};
    }
    pair<Key, Value> getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        int x = getMax(root); return {T[x].key, T[x].val};
    }
    pair<Key, Value> floor(const Key &key) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, key);
        if (x == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        return {T[x].key, T[x].val};
    }
    pair<Key, Value> ceiling(const Key &key) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, key);
        if (x == -1) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {T[x].key, T[x].val};
    }
    pair<Key, Value> select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        int x = select(root, k); return {T[x].key, T[x].val};
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
