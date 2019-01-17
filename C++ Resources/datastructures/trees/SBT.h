#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Size Balanced Binary Search Tree
// Time Complexity:
//   constructor, empty, size: O(1)
//   keyValuePairs: O(N)
//   all other operators: O(log N)
// Memory Complexity: O(N)
template <typename Key, typename Value, typename Comparator = less<Key>> struct SBT {
    Comparator cmp; vector<Key> KEY; vector<Value> VAL; vector<int> SZ, L, R; int root = 0;
    void update(int x) { SZ[x] = 1 + SZ[L[x]] + SZ[R[x]]; }
    int rotateRight(int x) { int y = L[x]; L[x] = R[y]; R[y] = x; update(x); update(y); return y; }
    int rotateLeft(int x) { int y = R[x]; R[x] = L[y]; L[y] = x; update(x); update(y); return y; }
    int maintain(int x, bool flag) {
        if (flag) {
            if (SZ[L[x]] < SZ[L[R[x]]]) { R[x] = rotateRight(R[x]); x = rotateLeft(x); }
            else if (SZ[L[x]] < SZ[R[R[x]]]) x = rotateLeft(x);
            else return x;
        } else {
            if (SZ[R[x]] < SZ[R[L[x]]]) { L[x] = rotateLeft(L[x]); x = rotateRight(x); }
            else if (SZ[R[x]] < SZ[L[L[x]]]) x = rotateRight(x);
            else return x;
        }
        L[x] = maintain(L[x], false); R[x] = maintain(R[x], true); x = maintain(x, true); x = maintain(x, false);
        return x;
    }
    int get(int x, Key key) {
        if (!x) return 0;
        if (cmp(key, KEY[x])) return get(L[x], key);
        else if (cmp(KEY[x], key)) return get(R[x], key);
        else return x;
    }
    int put(int x, Key key, Value val) {
        if (!x) {
            if (KEY.empty()) KEY.push_back(key);
            if (VAL.empty()) VAL.push_back(val);
            KEY.push_back(key); VAL.push_back(val); SZ.push_back(1); L.push_back(0); R.push_back(0);
            return int(KEY.size()) - 1;
        }
        if (cmp(key, KEY[x])) { int l = put(L[x], key, val); L[x] = l; }
        else if (cmp(KEY[x], key)) { int r = put(R[x], key, val); R[x] = r; }
        else { VAL[x] = val; return x; }
        update(x); return maintain(x, key > KEY[x]);
    }
    int removeMin(int x) {
        if (!L[x]) return R[x];
        L[x] = removeMin(L[x]); update(x); return x;
    }
    int removeMax(int x) {
        if (!R[x]) return L[x];
        R[x] = removeMax(R[x]); update(x); return x;
    }
    int getMin(int x) { return L[x] ? getMin(L[x]) : x; }
    int getMax(int x) { return R[x] ? getMax(R[x]) : x; }
    int remove(int x, Key key) {
        if (cmp(key, KEY[x])) L[x] = remove(L[x], key);
        else if (cmp(KEY[x], key)) R[x] = remove(R[x], key);
        else {
            if (!L[x]) return R[x];
            else if (!R[x]) return L[x];
            else { int y = x; x = getMin(R[y]); R[x] = removeMin(R[y]); L[x] = L[y]; }
        }
        update(x); return x;
    }
    int floor(int x, Key key) {
        if (!x) return 0;
        if (!cmp(key, KEY[x]) && !cmp(KEY[x], key)) return x;
        if (cmp(key, KEY[x])) return floor(L[x], key);
        int y = floor(R[x], key); return y ? y : x;
    }
    int ceiling(int x, Key key) {
        if (!x) return 0;
        if (!cmp(key, KEY[x]) && !cmp(KEY[x], key)) return x;
        if (cmp(KEY[x], key)) return ceiling(R[x], key);
        int y = ceiling(L[x], key); return y ? y : x;
    }
    int select(int x, int k) {
        if (!x) return 0;
        int t = SZ[L[x]];
        if (t > k) return select(L[x], k);
        else if (t < k) return select(R[x], k - t - 1);
        return x;
    }
    int getRank(int x, Key key) {
        if (!x) return 0;
        if (cmp(key, KEY[x])) return getRank(KEY[x], key);
        else if (cmp(KEY[x], key)) return 1 + SZ[L[x]] + getRank(R[x], key);
        else return SZ[L[x]];
    }
    void keyValuePairsInOrder(int x, vector<pair<Key, Value>> &queue) {
        if (!x) return;
        keyValuePairsInOrder(L[x], queue); queue.push_back({KEY[x], VAL[x]}); keyValuePairsInOrder(R[x], queue);
    }
    void keyValuePairs(int x, vector<pair<Key, Value>> &queue, Key lo, Key hi) {
        if (!x) return;
        if (cmp(lo, KEY[x])) keyValuePairs(L[x], queue, lo, hi);
        if (!cmp(KEY[x], lo) && !cmp(hi, KEY[x])) queue.push_back({KEY[x], VAL[x]});
        if (cmp(KEY[x], hi)) keyValuePairs(R[x], queue, lo, hi);
    }
    SBT() : SZ(1, 0), L(1, 0), R(1, 0) {}
    void clear() {
        KEY.clear(); VAL.clear(); SZ.clear(); L.clear(); R.clear();
        SZ.push_back(0); L.push_back(0); R.push_back(0);
    }
    bool empty() { return root == 0; }
    int size() { return SZ[root]; }
    Value get(Key key) {
        int x = get(root, key);
        if (!x) throw no_such_element_exception("no such key is in the symbol table");
        return VAL[x];
    }
    bool contains(Key key) { return get(root, key) != 0; }
    void put(Key key, Value val) { root = put(root, key, val); }
    void remove(Key key) { if (contains(key)) root = remove(root, key); }
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
        int x = getMin(root); return {KEY[x], VAL[x]};
    }
    pair<Key, Value> getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        int x = getMax(root); return {KEY[x], VAL[x]};
    }
    pair<Key, Value> floor(Key key) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, key);
        if (!x) throw no_such_element_exception("call to floor() resulted in no such value");
        return {KEY[x], VAL[x]};
    }
    pair<Key, Value> ceiling(Key key) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, key);
        if (!x) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return {KEY[x], VAL[x]};
    }
    pair<Key, Value> select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        int x = select(root, k); return {KEY[x], VAL[x]};
    }
    int getRank(Key key) { return getRank(root, key); }
    vector<pair<Key, Value>> keyValuePairs() {
        vector<pair<Key, Value>> queue; keyValuePairsInOrder(root, queue); return queue;
    }
    vector<pair<Key, Value>> keyValuePairs(Key lo, Key hi) {
        vector<pair<Key, Value>> queue; keyValuePairs(root, queue, lo, hi); return queue;
    }
    int size(Key lo, Key hi) {
        if (cmp(hi, lo)) return 0;
        if (contains(hi)) return getRank(hi) - getRank(lo) + 1;
        else return getRank(hi) - getRank(lo);
    }
};
