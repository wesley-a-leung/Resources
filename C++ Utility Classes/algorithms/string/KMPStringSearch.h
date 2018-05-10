#ifndef ALGORITHMS_STRING_KMPSTRINGSEARCH_H_
#define ALGORITHMS_STRING_KMPSTRINGSEARCH_H_

#include <bits/stdc++.h>
using namespace std;

class KMPFastStringSearch {
private:
    int *LCP;
    int m;                      // the pattern length;
    char *pattern = nullptr;    // either the character array for the pattern
    string pat = "";            // or the pattern string

public:
    /**
     * Preprocesses the pattern string.
     *
     * @param pat the pattern string
     */
    KMPFastStringSearch(string pat) {
        this->pat = pat;
        m = pat.length();
        LCP = new int[m + 1];
        LCP[0] = -1;
        for (int i = 0, j = -1; i < m; i++, j++, LCP[i] = (i != m && pat[i] == pat[j]) ? LCP[j] : j) {
            while (j >= 0 && pat[i] != pat[j]) j = LCP[j];
        }
    }

    /**
     * Preprocesses the pattern string.
     *
     * @param pattern the pattern string
     * @param m the length of the pattern
     */
    KMPFastStringSearch(char *pattern, int m) {
        this->pattern = pattern;
        this->m = m;
        LCP = new int[m + 1];
        LCP[0] = -1;
        for (int i = 0, j = -1; i < m; i++, j++, LCP[i] = (i != m && pattern[i] == pattern[j]) ? LCP[j] : j) {
            while (j >= 0 && pattern[i] != pattern[j]) j = LCP[j];
        }
    }

    /**
     * Returns the index of the first occurrence of the pattern string
     * in the text string.
     *
     * @param  txt the text string
     * @return the index of the first occurrence of the pattern string
     *         in the text string; -1 if no such match
     */
    int search(string txt) {
        int n = txt.length();
        for (int i = 0, j = 0; i < n; i++, j++) {
            while (j >= 0 && txt[i] != pat[j]) j = LCP[j];
            if (j == m - 1) return i - j;
        }
        return -1;
    }

    /**
     * Returns the index of the first occurrence of the pattern string
     * in the text string.
     *
     * @param  text the text string
     * @param  n the length of the text
     * @return the index of the first occurrence of the pattern string
     *         in the text string; -1 if no such match
     */
    int search(char *text, int n) {
        for (int i = 0, j = 0; i < n; i++, j++) {
            while (j >= 0 && text[i] != pattern[j]) j = LCP[j];
            if (j == m - 1) return i - j;
        }
        return -1;
    }
};

#endif /* ALGORITHMS_STRING_KMPSTRINGSEARCH_H_ */
