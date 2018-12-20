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
        int l = 0, r = 0;
        if (int(S.length()) > 0) z[0] = int(S.length());
        for (int i = 1; i < int(S.length()); i++) {
            if (i > r) {
                l = r = i;
                while (r < int(S.length()) && S[r] == S[r - l]) r++;
                r--; z[i] = r - l + 1;
            } else {
                int j = i - l;
                if (z[j] < r - i + 1) z[i] = z[j];
                else {
                    l = i;
                    while (r < int(S.length()) && S[r] == S[r - l]) r++;
                    r--; z[i] = r - l + 1;
                }
            }
        }
    }
};
