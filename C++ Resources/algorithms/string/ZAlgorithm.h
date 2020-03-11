#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes an array z, where z[i] is the length of the longest substring starting at S[i],
// which is also a prefix of S (maximum k such that S[i + j] == S[j] for all 0 <= j < k)
// Time Complexity: O(S)
// Memory Complexity: O(S) additional memory
template <const int MAXS> struct ZAlgorithm {
    int z[MAXS];
    void run(const string &S) {
        if (int(S.length()) > 0) z[0] = int(S.length());
        for (int i = 1, l = 0, r = 0; i < int(S.length()); i++) {
            if (i <= r) z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < int(S.length()) && S[z[i]] == S[i + z[i]]) z[i]++;
            if (i + z[i] - 1 > r) r = (l = i) + z[i] - 1;
        }
    }
};
