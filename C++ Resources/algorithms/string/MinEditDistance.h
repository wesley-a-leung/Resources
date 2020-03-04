#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum edit distance between 2 strings, allowing for custom penalties
// Time Complexity: O(len(s1) * len(s2))
// Memory Complexity: O(len(s2))
template <const int MAXS, class T> struct MinEditDistance {
    T dp[2][MAXS]; string substring;
    T solve(const string &s1, const string &s2, T cpyPen = 0, T repPen = 1, T insPen = 1, T delPen = 1) {
        for (int i = 0; i <= int(s2.length()); i++) dp[0][i] = i * delPen;
        for (int i = 1; i <= int(s1.length()); i++) {
            dp[i % 2][0] = i * insPen;
            for (int j = 1; j <= int(s2.length()); j++) {
                dp[i % 2][j] = min(min(dp[1 - i % 2][j - 1] + repPen, dp[i % 2][j - 1] + insPen), dp[1 - i % 2][j] + delPen);
                if (s1[i - 1] == s2[j - 1]) dp[i % 2][j] = min(dp[i % 2][j], dp[1 - i % 2][j - 1] + cpyPen);
            }
        }
        return dp[int(s1.length()) % 2][int(s2.length())];
    }
};
