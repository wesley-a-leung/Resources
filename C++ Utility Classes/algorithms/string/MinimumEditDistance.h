/*
 * MinimumEditDistance.h
 *
 *  Created on: Aug 5, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_STRING_MINIMUMEDITDISTANCE_H_
#define ALGORITHMS_STRING_MINIMUMEDITDISTANCE_H_

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
 * It uses space of O(NM)
 *
 * @author Wesley Leung
 */
int minDistance(string word1, string word2) {
    int len1 = word1.length();
    int len2 = word2.length();

    // len1+1, len2+1, because finally return dp[len1][len2]
    int dp[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }

    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }

    //iterate though, and check last char
    for (int i = 1; i <= len1; i++) {
        char c1 = word1[i - 1];
        for (int j = 1; j <= len2; j++) {
            char c2 = word2[j - 1];

            //if last two chars equal
            if (c1 == c2) {
                //update dp value for +1 length
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                int replace = dp[i - 1][j - 1] + REPLACE_PENALTY;
                int insert = dp[i - 1][j] + INSERT_PENALTY;
                int del = dp[i][j - 1] + DELETE_PENALTY;
                dp[i][j] = min(replace, min(insert, del));
            }
        }
    }
    return dp[len1][len2];
}

#endif /* ALGORITHMS_STRING_MINIMUMEDITDISTANCE_H_ */
