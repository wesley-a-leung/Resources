#pragma once
#include <bits/stdc++.h>
using namespace std;

// 64-bit Radix Heap
// All new keys pushed to the queue must be less than the last call to top
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   top: O(B) where B is the number of bits
//   empty, size: O(1)
//   pop, push: O(1) amortized
// Memory Complexity: O(N)
template <class Comparator = less<uint64_t>> struct RadixHeap64 {
    Comparator cmp; int n; uint64_t last; vector<uint64_t> x[65];
    int bsr(uint64_t a) { return a ? 63 - __builtin_clzll(a) : -1; }
    void aux(uint64_t a) { x[bsr(a ^ last) + 1].push_back(a); }
    RadixHeap64() : n(0), last(0) {}
    bool empty() const { return 0 == n; }
    int size() const { return n; }
    uint64_t top() {
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = x[i][0];
            for (int j = 1; j < int(x[i].size()); j++) if (cmp(last, x[i][j])) last = x[i][j];
            for (uint64_t a : x[i]) aux(a);
            x[i].clear();
        }
        return x[0].back();
    }
    void pop() { top(); n--; x[0].pop_back(); }
    void push(uint64_t key) { n++; aux(key); }
};
