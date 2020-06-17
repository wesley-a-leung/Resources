#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes an array z, where z[i] is the length of the longest substring starting at S[i],
// which is also a prefix of S (maximum k such that S[i + j] == S[j] for all 0 <= j < k)
// Time Complexity:
//   run: O(S)
//   search: O(pat + txt)
//   multisearch: O(pat + txt)
// Memory Complexity: O(S) additional memory
template <const int MAXS> struct ZAlgorithm {
    int z[MAXS]; vector<int> matches;
    void run(const string &S) {
        fill(z, z + int(S.length()), 0); z[0] = int(S.length());
        for (int i = 1, l = 0, r = 0; i < int(S.length()); i++) {
            if (i <= r) z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < int(S.length()) && S[z[i]] == S[i + z[i]]) z[i]++;
            if (i + z[i] - 1 > r) r = (l = i) + z[i] - 1;
        }
    }
    int search(const string &pat, const string &txt) { // returns the first index in the text of a match, -1 if none
        run(pat + txt);
        for (int i = 0; i < int(txt.length()); i++) if (z[i] >= int(pat.length())) return i;
        return -1;
    }
    int multiSearch(const string &pat, const string &txt) { // finds all indices in the text of a match
        run(pat + txt); matches.clear();
        for (int i = 0; i < int(txt.length()); i++) if (z[i] >= int(pat.length())) matches.push_back(i);
        return int(matches.size());
    }
};
