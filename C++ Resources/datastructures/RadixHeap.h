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
template <class T, class Comparator = less<uint32_t>> struct RadixHeap {
    Comparator cmp; int n; uint32_t last; vector<pair<uint32_t, T>> x[33];
    int bsr(uint32_t a) { return a ? 31 - __builtin_clz(a) : -1; }
    void aux(const pair<uint32_t, T> &p) { x[bsr(p.first ^ last) + 1].push_back(p); }
    RadixHeap() : n(0), last(0) {}
    bool empty() const { return 0 == n; }
    int size() const { return n; }
    pair<uint32_t, T> top() {
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
    void push(uint32_t key, const T &value) { n++; aux(make_pair(key, value)); }
};
