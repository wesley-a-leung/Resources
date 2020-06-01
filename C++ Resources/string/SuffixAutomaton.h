#pragma once
#include <bits/stdc++.h>
using namespace std;

// Suffix Automaton
// Time Complexity:
//   add: O(S)
//   addLetter: amortized O(1)
//   LCS: O(S)
// Memory Complexity: O(S * ALPHABET_SIZE)
template <const int ALPHABET_SIZE, const int OFFSET> struct SuffixAutomaton {
    vector<array<int, ALPHABET_SIZE>> to; vector<int> len, link; int last;
    void init() {
        to.clear(); len.clear(); link.clear(); last = 0;
        to.emplace_back(); to.back().fill(-1); len.push_back(0); link.push_back(0);
    }
    void addLetter(char c) {
        c -= OFFSET; int p = last, q;
        if (to[p][c] != -1) {
            q = to[p][c];
            if (len[q] == len[p] + 1) { last = q; return; }
            to.push_back(to[q]); len.push_back(len[p] + 1); link.push_back(link[q]); link[q] = int(link.size()) - 1;
            while (to[p][c] == q) { to[p][c] = int(to.size()) - 1; p = link[p]; }
        } else {
            last = int(to.size()); to.emplace_back(); to.back().fill(-1); len.push_back(len[p] + 1); link.push_back(0);
            while (to[p][c] == -1) { to[p][c] = last; p = link[p]; }
            if (to[p][c] == last) { link[last] = p; return; }
            q = to[p][c];
            if (len[q] == len[p] + 1) { link[last] = q; return; }
            to.push_back(to[q]); len.push_back(len[p] + 1);
            link.push_back(link[q]); link[q] = int(link.size()) - 1; link[last] = int(link.size()) - 1;
            while (to[p][c] == q) { to[p][c] = int(to.size()) - 1; p = link[p]; }
        }
    }
    void add(const string &s) { for (auto &&c : s) addLetter(c); }
    // longest common substring
    int LCS(const string &s) {
        int p = 0, curLen = 0, ret = 0;
        for (char c : s) {
            c -= OFFSET;
            while (p != 0 && to[p][c] == -1) { p = link[p]; curLen = len[p]; }
            if (to[p][c] != -1) { p = to[p][c]; curLen++; }
            ret = max(ret, curLen);
        }
        return ret;
    }
};
