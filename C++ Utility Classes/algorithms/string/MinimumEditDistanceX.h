/*
 * MinimumEditDistanceX.h
 *
 *  Created on: Aug 5, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_STRING_MINIMUMEDITDISTANCEX_H_
#define ALGORITHMS_STRING_MINIMUMEDITDISTANCEX_H_

#include <bits/stdc++.h>

using namespace std;

// Penalty Value
const int REPLACE_PENALTY = 1;
const int INSERT_PENALTY = 1;
const int DELETE_PENALTY = 1;

/**
 * Uses dynamic programming to compute the minimum
 * edit distance between 2 string in O(NM) time where N is the
 * length of the first string and M is the length of the second
 * string
 *
 * This class uses optimized space of O(2M)
 *
 * @author Wesley Leung
 */
int minDistance(string word1, string word2) {
    int len1 = word1.length();
    int len2 = word2.length();

    int dp[2][len2 + 1];

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j <= len2; j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0) {
                dp[i % 2][j] = j;
            } else if (j == 0) {
                dp[i % 2][j] = i;
            } else if (word1[i - 1] == word2[j - 1]) {
                dp[i % 2][j] = dp[1 - i % 2][j - 1];
            } else {
                int replace = dp[1 - i % 2][j - 1] + REPLACE_PENALTY;
                int insert = dp[1 - i % 2][j] + INSERT_PENALTY;
                int del = dp[i % 2][j - 1] + DELETE_PENALTY;

                int min = replace > insert ? insert : replace;
                min = del > min ? min : del;
                dp[i % 2][j] = min;
            }
        }
    }
    return dp[len1 % 2][len2];
}

#endif /* ALGORITHMS_STRING_MINIMUMEDITDISTANCEX_H_ */
