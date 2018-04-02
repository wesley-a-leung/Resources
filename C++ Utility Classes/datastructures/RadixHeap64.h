#ifndef DATASTRUCTURES_RADIXHEAP64_H_
#define DATASTRUCTURES_RADIXHEAP64_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Radix Heap, 64-bit:
 *
 * Top: O(B) where B is the number of bits
 * Pop: O(1)
 * Push: O(1)
 */
// convention is same as priority_queue in STL
template <typename T, typename Comparator = less<uint64_t>>
struct RadixHeap64 {
private:
    Comparator cmp;
    int n;
    uint64_t last;
    vector<pair<uint64_t, T>> x[65];

    int bsr(uint64_t a) {
        return a ? 63 - __builtin_clzll(a) : -1;
    }

    void aux(pair<uint64_t, T> p) {
        x[bsr(p.first ^ last) + 1].push_back(p);
    }

public:
    RadixHeap64() : n(0), last(0) {}

    bool empty() const {
        return 0 == n;
    }

    int size() const {
        return n;
    }

    pair<uint64_t, T> top() {
        assert(n > 0);
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = x[i][0].first;
            for (int j = 1; j < (int) x[i].size(); j++) {
                if (cmp(last, x[i][j].first)) last = x[i][j].first;
            }
            for (pair<uint64_t, T> p : x[i]) aux(p);
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

    void push(uint64_t key, T value) {
        n++;
        aux({key, value});
    }
};

#endif /* DATASTRUCTURES_RADIXHEAP64_H_ */
