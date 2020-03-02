#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum edit distance between 2 strings, allowing for custom penalties
// Time Complexity: O(len(s1) * len(s2))
// Memory Complexity: O(len(s2))
template <const int MAXS> struct MinEditDistance {
    int dp[2][MAXS]; string substring;
    int solve(const string &s1, const string &s2, int repPen = 1, int insPen = 1, int delPen = 1) {
        iota(dp[0], dp[0] + s2.length() + 1, 0);
        for (int i = 1; i <= int(s1.length()); i++) {
            dp[i % 2][0] = i;
            for (int j = 1; j <= int(s2.length()); j++) {
                if (s1[i - 1] == s2[j - 1]) dp[i % 2][j] = dp[1 - i % 2][j - 1];
                else dp[i % 2][j] = min(min(dp[1 - i % 2][j - 1] + repPen, dp[i % 2][j - 1] + insPen), dp[1 - i % 2][j] + delPen);
            }
        }
        return dp[int(s1.length()) % 2][int(s2.length())];
    }
};
