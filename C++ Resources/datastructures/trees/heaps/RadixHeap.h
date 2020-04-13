#pragma once
#include <bits/stdc++.h>
using namespace std;

// 32-bit Radix Heap
// All new keys pushed to the queue must be less than the last call to top
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   top: O(B) where B is the number of bits
//   empty, size: O(1)
//   pop, push: O(1) amortized
template <class Comparator = less<uint32_t>> struct RadixHeap {
    Comparator cmp; int n; uint32_t last; vector<uint32_t> x[33];
    int bsr(uint32_t a) { return a ? 31 - __builtin_clz(a) : -1; }
    void aux(uint32_t a) { x[bsr(a ^ last) + 1].push_back(a); }
    RadixHeap() : n(0), last(0) {}
    bool empty() const { return 0 == n; }
    int size() const { return n; }
    uint32_t top() {
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = x[i][0];
            for (int j = 1; j < int(x[i].size()); j++) if (cmp(last, x[i][j])) last = x[i][j];
            for (uint32_t a : x[i]) aux(a);
            x[i].clear();
        }
        return x[0].back();
    }
    void pop() { top(); n--; x[0].pop_back(); }
    void push(uint32_t key) { n++; aux(key); }
};
