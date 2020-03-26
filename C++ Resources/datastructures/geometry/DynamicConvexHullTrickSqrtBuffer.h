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
    bool bad3(const pair<T, T> &a, const pair<T, T> &b, const pair<T, T> &c) {
        return (a.second - b.second) * (c.first - b.first) >= (b.second - c.second) * (b.first - a.first);
    }
    bool bad2(const pair<T, T> &a, const pair<T, T> &b) {
        return !cmp(a.first, b.first) && !cmp(b.first, a.first) && !cmp(b.second, a.second);
    }
    T eval(const pair<T, T> &a, const T &x) { return a.first * x + a.second; }
    void buildHull() {
        int back = 0;
        for (auto &&line : large) {
            while (back >= 2 && bad3(large[back - 2], large[back - 1], line)) back--;
            while (back >= 1 && bad2(large[back - 1], line)) back--;
            large[back++] = line;
        }
        large.resize(back);
    }
    void rebuild() {
        if (int(small.size()) > SCALE_FACTOR * sqrt(small.size() + large.size())) {
            auto lineCmp = [&] (const pair<T, T> &a, const pair<T, T> &b) { return cmp(a.first, b.first); };
            int largeSz = int(large.size()); sort(small.begin(), small.end(), lineCmp);
            for (auto &&x : small) large.push_back(x);
            small.clear(); inplace_merge(large.begin(), large.begin() + largeSz, large.end(), lineCmp); buildHull();
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
        for (auto &&line : small) {
            T y = eval(line, x);
            if (cmp(mx, y)) mx = y;
        }
        return mx;
    }
    void clear() { large.clear(); small.clear(); }
};
