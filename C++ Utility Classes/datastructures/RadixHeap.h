#ifndef DATASTRUCTURES_RADIXHEAP_H_
#define DATASTRUCTURES_RADIXHEAP_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Radix Heap, 32-bit:
 *
 * Top: O(B) where B is the number of bits
 * Pop: O(1)
 * Push: O(1)
 */
// convention is same as priority_queue in STL
template <typename T, typename Comparator = less<uint32_t>>
struct RadixHeap {
private:
    Comparator cmp;
    int n;
    uint32_t last;
    vector<pair<uint32_t, T>> x[33];

    int bsr(uint32_t a) {
        return a ? 31 - __builtin_clz(a) : -1;
    }

    void aux(pair<uint32_t, T> p) {
        x[bsr(p.first ^ last) + 1].push_back(p);
    }

public:
    RadixHeap() : n(0), last(0) {}

    bool empty() const {
        return 0 == n;
    }

    int size() const {
        return n;
    }

    pair<uint32_t, T> top() {
        assert(n > 0);
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = x[i][0].first;
            for (int j = 1; j < (int) x[i].size(); j++) {
                if (cmp(last, x[i][j].first)) last = x[i][j].first;
            }
            for (pair<uint32_t, T> p : x[i]) aux(p);
            x[i].clear();
        }
        return x[0].back();
    }

    void pop() {
        assert(n > 0);
        top();
        n--;
        x[0].pop_back();
    }

    void push(uint32_t key, T value) {
        n++;
        aux({key, value});
    }
};

#endif /* DATASTRUCTURES_RADIXHEAP_H_ */
