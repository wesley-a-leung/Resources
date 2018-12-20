#pragma once
#include <bits/stdc++.h>
using namespace std;

// KMP String Search
// Time Complexity:
//   preCompute: O(len(pat))
//   search, multiSearch: O(len(txt))
// Memory Complexity: O(len(pat)) additional memory
template <const int MAXS> struct KMP {
    int LCP[MAXS]; string pat; vector<int> matches;
    void preCompute(const string &pat) {
        this->pat = pat; LCP[0] = -1;
        for (int i = 0, j = -1; i < int(pat.length()); i++, j++, LCP[i] = (i != int(pat.length()) && pat[i] == pat[j]) ? LCP[j] : j)
            while (j >= 0 && pat[i] != pat[j]) j = LCP[j];
    }
    int search(const string &txt) { // returns the first index of a match, -1 if none
        for (int i = 0, j = 0, n = int(txt.length()); i < n; i++, j++) {
            while (j >= 0 && txt[i] != pat[j]) j = LCP[j];
            if (j == int(pat.length()) - 1) return i - j;
        }
        return -1;
    }
    void multiSearch(const string &txt) { // finds all matches
        matches.clear();
        for (int i = 0, j = 0; i < int(txt.length()); i++, j++) {
            while (j >= 0 && (j == int(pat.length()) || txt[i] != pat[j])) j = LCP[j];
            if (j == int(pat.length()) - 1) matches.push_back(i - j);
        }
    }
};
