#ifndef ALGORITHMS_STRING_MANACHERPALINDROME_H_
#define ALGORITHMS_STRING_MANACHERPALINDROME_H_

#include <bits/stdc++.h>

using namespace std;

class ManacherPalindrome {
private:
    int* p;  // p[i] = length of longest palindromic substring of t, centered at i
    string s;  // original string
    char *t;  // transformed string

    // Transform s into t.
    // For example, if s = "abba", then t = "$#a#b#b#a#@"
    // the # are interleaved to avoid even/odd-length palindromes uniformly
    // $ and @ are prepended and appended to each end to avoid bounds checking
    void preprocess() {
        t = new char[s.length()*2 + 3];
        t[0] = '$';
        t[s.length()*2 + 2] = '@';
        for (int i = 0; i < s.length(); i++) {
            t[2*i + 1] = '#';
            t[2*i + 2] = s[i];
        }
        t[s.length()*2 + 1] = '#';
    }

public:
    ManacherPalindrome(string s) {
        this->s = s;
        preprocess();
        p = new int[s.length()*2 + 3];

        int center = 0, right = 0;
        for (int i = 1; i < s.length()*2 + 2; i++) {
            int mirror = 2*center - i;

            if (right > i)
                p[i] = min(right - i, p[mirror]);

            // attempt to expand palindrome centered at i
            while (t[i + (1 + p[i])] == t[i - (1 + p[i])])
                p[i]++;

            // if palindrome centered at i expands past right,
            // adjust center based on expanded palindrome.
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }

    }

    // longest palindromic substring
    string longestPalindromicSubstring() {
        int length = 0;   // length of longest palindromic substring
        int center = 0;   // center of longest palindromic substring
        for (int i = 1; i < s.length()*2 + 2; i++) {
            if (p[i] > length) {
                length = p[i];
                center = i;
            }
        }
        return s.substr((center - 1 - length) / 2, (center - 1 + length) / 2 - (center - 1 - length) / 2);
    }

    // longest palindromic substring centered at index i/2
    string longestPalindromicSubstring(int i) {
        int length = p[i + 2];
        int center = i + 2;
        return s.substr((center - 1 - length) / 2, (center - 1 + length) / 2 - (center - 1 - length) / 2);
    }
};

#endif /* ALGORITHMS_STRING_MANACHERPALINDROME_H_ */
