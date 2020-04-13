#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum value that can be obtained by item in a knapsack (from a selection of N items)
// that can hold a maximum of M weight, allowing for a maximum of f_i items for item i
// Time Complexity: O(NM log M)
// Space Complexity: O(N log M + M)
template <const int MAXN, const int MAXM, class value_type> struct BoundedKnapsack {
    value_type dp[MAXM]; vector<pair<int, value_type>> items;
    void addItem(int w, value_type v, int f) { // adds f items with weight w, and value v
        int sum = 0, mult = 1;
        for (f = min(f, MAXM / w); sum + mult < f; sum += mult, mult *= 2) items.emplace_back(mult * w, mult * v);
        items.emplace_back((f - sum) * w, (f - sum) * v);
    }
    void clear() { items.clear(); }
    value_type solve(int N, int M) {
        fill(dp, dp + M + 1, 0);
        for (auto &&i : items) for (int j = M; j >= i.first; j--) dp[j] = max(dp[j], dp[j - i.first] + i.second);
        return dp[M];
    }
};

// Computes the maximum value that can be obtained by item in a knapsack (from a selection of N items)
// that can hold a maximum of M weight, allowing for a maximum of f_i items for item i,
// by turning the maximization problem into a minimization problem, with K as the maximum bound of the answer
// Time Complexity: O(NK log K)
// Space Complexity: O(N log K + K)
template <const int MAXN, const int MAXK, class weight_type> struct BoundedKnapsackDual {
    weight_type dp[MAXK], INF; vector<pair<weight_type, int>> items;
    BoundedKnapsackDual(weight_type INF) : INF(INF) {}
    void addItem(weight_type w, int v, int f) { // adds f items with weight w, and value v
        int sum = 0, mult = 1;
        for (f = min(f, MAXK / v); sum + mult < f; sum += mult, mult *= 2) items.emplace_back(mult * w, mult * v);
        items.emplace_back((f - sum) * w, (f - sum) * v);
    }
    void clear() { items.clear(); }
    int solve(int N, weight_type M, int K) {
        fill(dp, dp + K + 1, INF); dp[0] = 0;
        for (auto &&i : items) for (int j = K; j >= i.first; j--) dp[j] = min(dp[j], dp[j - i.second] + i.first);
        for (int j = K; j >= 0; j--) if (dp[j] <= M) return j;
        assert(false); return -1;
    }
};
