#pragma once
#include <bits/stdc++.h>
using namespace std;

// 64-bit Radix Heap
// Comparator convention is same as priority_queue in STL
// Time Complexity:
//   top: O(B) where B is the number of bits
//   empty, size, pop, push: O(1)
template <class T, class Comparator = less<uint64_t>> struct RadixHeap64 {
    Comparator cmp; int n; uint64_t last; vector<pair<uint64_t, T>> x[65];
    int bsr(uint64_t a) { return a ? 63 - __builtin_clzll(a) : -1; }
    void aux(const pair<uint64_t, T> &p) { x[bsr(p.first ^ last) + 1].push_back(p); }
    RadixHeap64() : n(0), last(0) {}
    bool empty() const { return 0 == n; }
    int size() const { return n; }
    pair<uint64_t, T> top() {
        assert(n > 0);
        if (x[0].empty()) {
            int i = 1;
            while (x[i].empty()) ++i;
            last = x[i][0].first;
            for (int j = 1; j < int(x[i].size()); j++) if (cmp(last, x[i][j].first)) last = x[i][j].first;
            for (auto &&p : x[i]) aux(p);
            x[i].clear();
        }
        return x[0].back();
    }
    void pop() { assert(n > 0); top(); n--; x[0].pop_back(); }
    void push(uint64_t key, T value) { n++; aux(make_pair(key, value)); }
};
