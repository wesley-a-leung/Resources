#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Size Balanced Binary Search (Multi) Set with Parallel Arrays
// Time Complexity:
//   constructor, empty, size: O(1)
//   values: O(N)
//   all other operators: O(log N)
// Memory Complexity: O(N)
template <class Value, class Comparator = less<Value>> struct SBTArraySet {
    Comparator cmp; vector<Value> VAL; vector<int> SZ, L, R; int root = 0;
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
        L[x] = maintain(L[x], false); R[x] = maintain(R[x], true); x = maintain(x, true); x = maintain(x, false); return x;
    }
    bool contains(int x, const Value &val) {
        if (!x) return false;
        else if (cmp(val, VAL[x])) return contains(L[x], val);
        else if (cmp(VAL[x], val)) return contains(R[x], val);
        return true;
    }
    int add(int x, const Value &val) {
        if (!x) {
            if (VAL.empty()) VAL.push_back(val);
            VAL.push_back(val); SZ.push_back(1); L.push_back(0); R.push_back(0); return int(VAL.size()) - 1;
        }
        if (cmp(val, VAL[x])) { int l = add(L[x], val); L[x] = l; }
        else { int r = add(R[x], val); R[x] = r; }
        update(x); return maintain(x, !cmp(val, VAL[x]));
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
    int remove(int x, const Value &val) {
        if (cmp(val, VAL[x])) L[x] = remove(L[x], val);
        else if (cmp(VAL[x], val)) R[x] = remove(R[x], val);
        else {
            if (!L[x]) return R[x];
            else if (!R[x]) return L[x];
            else { int y = x; x = getMin(R[y]); R[x] = removeMin(R[y]); L[x] = L[y]; }
        }
        update(x); return x;
    }
    int floor(int x, const Value &val) {
        if (!x) return 0;
        if (!cmp(val, VAL[x]) && !cmp(VAL[x], val)) return x;
        if (cmp(val, VAL[x])) return floor(L[x], val);
        int y = floor(R[x], val); return y ? y : x;
    }
    int ceiling(int x, const Value &val) {
        if (!x) return 0;
        if (!cmp(val, VAL[x]) && !cmp(VAL[x], val)) return x;
        if (cmp(VAL[x], val)) return ceiling(R[x], val);
        int y = ceiling(L[x], val); return y ? y : x;
    }
    int select(int x, int k) {
        if (!x) return 0;
        int t = SZ[L[x]];
        if (t > k) return select(L[x], k);
        else if (t < k) return select(R[x], k - t - 1);
        return x;
    }
    int getRank(int x, const Value &val) { // number of values less than first occurrence
        if (!x) return 0;
        if (!cmp(VAL[x], val)) return getRank(L[x], val);
        else return 1 + SZ[L[x]] + getRank(R[x], val);
    }
    void valuesInOrder(int x, vector<Value> &queue) {
        if (!x) return;
        valuesInOrder(L[x], queue); queue.push_back(VAL[x]); valuesInOrder(R[x], queue);
    }
    void values(int x, vector<Value> &queue, const Value &lo, const Value &hi) {
        if (!x) return;
        if (cmp(lo, VAL[x])) values(L[x], queue, lo, hi);
        if (!cmp(VAL[x], lo) && !cmp(hi, VAL[x])) queue.push_back(VAL[x]);
        if (cmp(VAL[x], hi)) values(R[x], queue, lo, hi);
    }
    SBTArraySet() : SZ(1, 0), L(1, 0), R(1, 0) {}
    void clear() { root = 0; VAL.clear(); SZ.clear(); L.clear(); R.clear(); SZ.push_back(0); L.push_back(0); R.push_back(0); }
    bool empty() { return root == 0; }
    int size() { return SZ[root]; }
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
        return VAL[getMin(root)];
    }
    Value getMax() {
        if (empty()) throw runtime_error("called getMax() with empty symbol table");
        return VAL[getMax(root)];
    }
    Value floor(const Value &val) {
        if (empty()) throw runtime_error("called floor() with empty symbol table");
        int x = floor(root, val);
        if (!x) throw no_such_element_exception("call to floor() resulted in no such value");
        return VAL[x];
    }
    Value ceiling(const Value &val) {
        if (empty()) throw runtime_error("called ceiling() with empty symbol table");
        int x = ceiling(root, val);
        if (!x) throw no_such_element_exception("call to ceiling() resulted in no such value");
        return VAL[x];
    }
    Value select(int k) {
        if (k < 0 || k >= size()) throw invalid_argument("k is not in range 0 to size");
        return VAL[select(root, k)];
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
