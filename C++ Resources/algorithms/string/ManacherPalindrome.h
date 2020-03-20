#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest palindromic substring centered at each half index
// Time Complexity: O(S)
// Memory Complexity: O(S) additional memory
template <const int MAXS> struct ManacherPalindrome {
    int p[MAXS * 2]; string s; char t[MAXS * 2];
    void run(const string &s) {
        this->s = s; t[0] = '$'; t[s.length() * 2 + 1] = '#'; t[s.length() * 2 + 2] = '@';
        for (int i = 0; i < int(s.length()); i++) { t[2 * i + 1] = '#'; t[2 * i + 2] = s[i]; }
        int center = 0, right = 0; fill(p, p + int(s.length()) * 2 + 3, 0);
        for (int i = 1; i < int(s.length()) * 2 + 2; i++) {
            int mirror = 2 * center - i;
            if (right > i) p[i] = min(right - i, p[mirror]);
            while (t[i + (1 + p[i])] == t[i - (1 + p[i])]) p[i]++;
            if (i + p[i] > right) { center = i; right = i + p[i]; }
        }
    }
    // longest palindromic substring, returns a pair containing the starting index and length
    pair<int, int> longestPalindromicSubstring() {
        int length = 0, center = 0;
        for (int i = 1; i < int(s.length()) * 2 + 2; i++) if (p[i] > length) { length = p[i]; center = i; }
        return make_pair((center - 1 - length) / 2, length);
    }
    // longest palindromic substring centered at index i/2, returns a pair containing the starting index and length
    pair<int, int> longestPalindromicSubstring(int i) {
        int length = p[i + 2], center = i + 2;
        return make_pair((center - 1 - length) / 2, length);
    }
};
