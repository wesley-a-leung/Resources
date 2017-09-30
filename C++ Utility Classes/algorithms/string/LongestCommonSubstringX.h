#ifndef ALGORITHMS_STRING_LONGESTCOMMONSUBSTRINGX_H_
#define ALGORITHMS_STRING_LONGESTCOMMONSUBSTRINGX_H_

#include <bits/stdc++.h>

using namespace std;

string substring;
int length;

/**
 * Computes the longest common continuous substring between two
 * strings in O(NM) time where N is the length of the first string
 * and M is the length of the second string.
 *
 * It uses space of O(2M).
 *
 * @param s1 the first string
 * @param s2 the second string
 */
void solve(string s1, string s2) {
    int dp[2][s2.length() + 1];
    int start = 0, maxLen = 0;
    for (int i = 1; i <= s1.length(); i++) {
        for (int j = 1; j <= s2.length(); j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i % 2][j] = 1 + dp[1 - i % 2][j - 1];
                if (dp[i % 2][j] > maxLen) {
                    maxLen = dp[i % 2][j];
                    start = i - dp[i % 2][j];
                }
            }
        }
    }
    length = maxLen;
    substring = s1.substr(start, maxLen);
}

#endif /* ALGORITHMS_STRING_LONGESTCOMMONSUBSTRINGX_H_ */
