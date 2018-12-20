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
    void clear() { items.clear(); solved = false; }
    value_type solve(int N, int M) {
        fill(dp, dp + M + 1, 0);
        for (auto &&i : items) for (int j = M; j >= i.first; j--) dp[j] = max(dp[j], dp[j - i.first] + i.second);
        solved = true;
        return dp[M];
    }
};
