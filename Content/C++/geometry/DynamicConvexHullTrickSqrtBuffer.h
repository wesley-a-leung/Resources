#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports adding lines in the form f(x) = mx + b and finding the maximum value of f(x) at any given x
// Dynamic variant (allows for updates and queries in arbitrary order)
// Time Complexity:
//   addLine: O(1)
//   getMax: O(sqrt(N) + log(N)) amortized
// Memory Complexity: O(N) where N is the total number of lines added
template <class T, class Comparator = less<T>> struct DynamicConvexHullTrickSqrtBuffer {
    Comparator cmp; vector<pair<T, T>> large, small; double SCALE_FACTOR;
    DynamicConvexHullTrickSqrtBuffer(const double SCALE_FACTOR = 1) : SCALE_FACTOR(SCALE_FACTOR) {}
    bool ccw(const pair<T, T> &a, const pair<T, T> &b, const pair<T, T> &c) {
        return (b.first - a.first) * (c.second - a.second) <= (b.second - a.second) * (c.first - a.first);
    }
    bool slope(const pair<T, T> &a, const pair<T, T> &b) {
        return !cmp(a.first, b.first) && !cmp(b.first, a.first) && !cmp(a.second, b.second);
    }
    T eval(const pair<T, T> &a, const T &x) { return a.first * x + a.second; }
    void rebuildHull() {
        int back = 0;
        for (auto &&line : large) {
            while (back >= 2 && ccw(line, large[back - 1], large[back - 2])) back--;
            while (back >= 1 && slope(line, large[back - 1])) back--;
            large[back++] = line;
        }
        large.resize(back);
    }
    void rebuild() {
        if (int(small.size()) > SCALE_FACTOR * sqrt(small.size() + large.size())) {
            auto lineCmp = [&] (const pair<T, T> &a, const pair<T, T> &b) { return cmp(a.first, b.first); };
            int largeSz = int(large.size()); sort(small.begin(), small.end(), lineCmp);
            for (auto &&x : small) large.push_back(x);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), lineCmp); rebuildHull();
        }
    }
    void addLine(T m, T b) { small.emplace_back(m, b); }
    T getMax(T x) {
        rebuild();
        int lo = 0, hi = int(large.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (!cmp(eval(large[mid + 1], x), eval(large[mid], x))) lo = mid + 1;
            else hi = mid;
        }
        T mx = large.empty() ? eval(small[0], x) : eval(large[lo], x);
        for (auto &&line : small) mx = max(mx, eval(line, x), cmp);
        return mx;
    }
    int size() { return int(large.size()) + int(small.size()); }
    bool empty() { return large.empty() && small.empty(); }
    void clear() { large.clear(); small.clear(); }
};
