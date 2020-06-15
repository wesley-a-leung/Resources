#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the number of trials required to determine which of the F floors an
// egg will break on, using E eggs, assuming eggs cannot be reused if broken
// Time Complexity:
//   preCompute: O(E^2)
//   query: O(log E)
//   querySingle: O(E log F)
// Memory Complexity: O(E^2)
template <const int MAXE> struct EggDropping {
    long long dp[MAXE][MAXE];
    void preCompute() {
        for (int i = 1; i < MAXE; i++) {
            dp[i][0] = 0; dp[i][1] = 1;
            for (int j = 2; j < MAXE; j++) dp[i][j] = dp[i - 1][j - 1] + dp[i][j - 1] + 1;
        }
    }
    int query(int E, int F) { return lower_bound(dp[E], dp[E] + MAXE, F) - dp[E]; }
    long long f(int x, int E, int F) {
        long long sum = 0, term = 1;
        for (int i = 1; i <= E && sum < F; i++) sum += ((term *= x - i + 1) /= i);
        return sum;
    }
    int querySingle(int E, int F) {
        int lo = 1, hi = F, mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (f(mid, E, F) < F) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }
};
