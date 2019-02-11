#pragma once
#include <bits/stdc++.h>
using namespace std;

// Palindromic Tree
// Time Complexity:
//   init: O(S * ALPHABET_SIZE)
//   addChar: amortized O(1)
//   add: O(S)
// Memory Complexity: O(S * ALPHABET_SIZE)
template<const int MAXS, const int ALPHABET_SIZE, const int OFFSET> struct PalindromicTree {
    using Data = int; const Data def = 0; Data val[MAXS]; int s[MAXS], len[MAXS], link[MAXS], to[MAXS][ALPHABET_SIZE], cur, last, ind;
    void computeData(); // to be implemented
    void init(int S = MAXS) {
        for (int i = 0; i < min(MAXS, S + 2); i++) { s[i] = len[i] = link[i] = 0; val[i] = def; fill(to[i], to[i] + ALPHABET_SIZE, 0); }
        s[0] = len[1] = -1; link[0] = 1; cur = 2; last = 0; ind = 1;
    }
    int getSufLink(int x, int i) { while (s[i - 1 - len[x]] != s[i]) x = link[x]; return x; }
    void addChar(char c) {
        s[ind] = c - OFFSET; last = getSufLink(last, ind);
        if (to[last][s[ind]] == 0) { len[cur] = len[last] + 2; link[cur] = to[getSufLink(link[last], ind)][s[ind]]; to[last][s[ind]] = cur++; }
        last = to[last][s[ind++]]; computeData();
    }
    void add(const string &s) { for (auto &&c : s) addChar(c); }
};
