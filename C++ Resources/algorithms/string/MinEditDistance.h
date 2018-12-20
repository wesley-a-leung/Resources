#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum edit distance between 2 strings, allowing for custom penalties
// Time Complexity: O(len(s1) * len(s2))
// Memory Complexity: O(len(s2))
template <const int MAXS> struct MinEditDistance {
    int dp[2][MAXS]; string substring;
    int solve(const string &s1, const string &s2, int repPen = 1, int insPen = 1, int delPen = 1) {
        for (int i = 0; i < 2; i++) fill(dp[i], dp[i] + s2.length() + 1, 0);
        for (int i = 0; i <= int(s1.length()); i++) {
            for (int j = 0; j <= int(s2.length()); j++) {
                if (i == 0) dp[i % 2][j] = j;
                else if (j == 0) dp[i % 2][j] = i;
                else if (s1[i - 1] == s2[j - 1]) dp[i % 2][j] = dp[1 - i % 2][j - 1];
                else dp[i % 2][j] = min(min(dp[1 - i % 2][j - 1] + repPen, dp[1 - i % 2][j] + insPen), dp[i % 2][j - 1] + delPen);
            }
        }
        return dp[int(s1.length()) % 2][int(s2.length())];
    }
};
