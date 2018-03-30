#ifndef DATASTRUCTURES_RADIXHEAP_H_
#define DATASTRUCTURES_RADIXHEAP_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Radix Heap:
 *
 * Top: O(B) where B is the number of bits
 * Pop: O(1)
 * Push: O(1)
 */
// convention is same as priority_queue in STL
template <typename T, typename Comparator = less<T>>
struct RadixHeap {
private:
    Comparator cmp;
    using uint = uint32_t;
    int n;
    uint last;
    vector<pair<uint, T>> x[33];

    int bsr(uint a) {
        return a ? 31 - __builtin_clz(a) : 1;
    }

    void aux(pair<uint, T> p) {
        x[bsr(p.first ^ last) + 1].push_back(p);
    }

public:
    RadixHeap() : n(0), last(0) {}

    bool empty() const {
        return 0 == size;
    }

    int size() const {
        return n;
    }

    pair<uint, T> top() {
        assert(n > 0);
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = max_element(x[i].begin(), x[i].end(), cmp).first;
            for (pair<uint, T> p : x[i]) aux(p);
            x[i].clear();
        }
        return x[0].back();
    }

    void pop() {
        assert(n > 0);
        n--;
        top();
        x[0].pop_back();
    }

    void push(uint key, T value) {
        n++;
        aux({key, value});
    }
};

#endif /* DATASTRUCTURES_RADIXHEAP_H_ */
