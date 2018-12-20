#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Indexed Priority Queue supporting changing the key of an index
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   constructor: O(N)
//   empty, size, top, keyOf: O(1)
//   insert, remove, pop, changeKey: O(log N)
// Memory Complexity: O(N)
template <class Key, class Comparator = less<Key>> struct IndexedPQ {
    Comparator cmp; int maxN, N; vector<int> pq, qp; vector<Key> keys;
    void exch(int i, int j) { swap(pq[i], pq[j]); qp[pq[i]] = i; qp[pq[j]] = j; }
    void swim(int k) { while (k > 1 && cmp(keys[pq[k / 2]], keys[pq[k]])) { exch(k, k / 2); k /= 2; } }
    void sink(int k) {
        while (2 * k <= N) {
            int j = 2 * k;
            if (j < N && cmp(keys[pq[j]], keys[pq[j + 1]])) j++;
            if (!cmp(keys[pq[k]], keys[pq[j]])) break;
            exch(k, j); k = j;
        }
    }
    IndexedPQ(int maxN) : maxN(maxN), N(0), pq(maxN + 1), qp(maxN, -1), keys(maxN) { assert(maxN >= 0); }
    bool empty() { return N == 0; }
    bool containsIndex(int i) { assert(0 <= i && i < maxN); return qp[i] != -1; }
    int size() { return N; }
    void push(int i, Key key) {
        assert(0 <= i && i < maxN);
        if (containsIndex(i)) throw invalid_argument("index is already in the priority queue");
        N++; qp[i] = N; pq[N] = i; keys[i] = key; swim(N);
    }
    pair<int, Key> top() {
        if (N == 0) throw no_such_element_exception("Priority queue underflow");
        return make_pair(pq[1], keys[pq[1]]);
    }
    pair<int, Key> pop() {
        if (N == 0) throw no_such_element_exception("Priority queue underflow");
        int minInd = pq[1]; Key minKey = keys[minInd]; exch(1, N--); sink(1); qp[minInd] = -1;
        return make_pair(minInd, minKey);
    }
    Key keyOf(int i) {
        assert(0 <= i && i < maxN);
        if (!containsIndex(i)) throw no_such_element_exception("index is not in the priority queue");
        else return keys[i];
    }
    void changeKey(int i, Key key) {
        assert(0 <= i && i < maxN);
        if (!containsIndex(i)) throw no_such_element_exception("index is not in the priority queue");
        Key old = keys[i]; keys[i] = key;
        if (cmp(old, key)) swim(qp[i]);
        else if (cmp(key, old)) sink(qp[i]);
    }
    void remove(int i) {
        assert(0 <= i && i < maxN);
        if (!containsIndex(i)) throw no_such_element_exception("index is not in the priority queue");
        int index = qp[i]; exch(index, N--); swim(index); sink(index); qp[i] = -1;
    }
};
