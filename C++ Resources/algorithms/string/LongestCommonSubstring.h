#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest common substring between 2 strings
// Time Complexity: O(len(s1) * len(s2))
// Memory Complexity: O(len(s2))
template <const int MAXS> struct LongestCommonSubstring {
    int dp[2][MAXS]; string substring;
    int solve(const string &s1, const string &s2) {
        int st = 0, len = 0; fill(dp[0], dp[0] + s2.length() + 1, 0);
        for (int i = 1; i <= int(s1.length()); i++) {
            dp[i % 2][0] = 0;
            for (int j = 1; j <= int(s2.length()); j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i % 2][j] = 1 + dp[1 - i % 2][j - 1];
                    if (dp[i % 2][j] > len) { len = dp[i % 2][j]; st = i - dp[i % 2][j]; }
                }
            }
        }
        substring = s1.substr(st, len); return len;
    }
};
