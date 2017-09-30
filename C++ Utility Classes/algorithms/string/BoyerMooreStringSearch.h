#ifndef ALGORITHMS_STRING_BOYERMOORESTRINGSEARCH_H_
#define ALGORITHMS_STRING_BOYERMOORESTRINGSEARCH_H_

#include <bits/stdc++.h>

using namespace std;

class BoyerMooreStringSearch {
private:
    int R;                    // the radix
    int *right;               // the bad-character skip array
    int m;                    // the length of the pattern
    char *pattern = nullptr;  // store the pattern as a character array
    string pat = "";          // or as a string

public:
    /**
     * Preprocesses the pattern string.
     *
     * @param pat the pattern string
     */
    BoyerMooreStringSearch(string pat) {
        this->R = 256;
        this->pat = pat;
        this->m = pat.length();
        // position of rightmost occurrence of c in the pattern
        right = new int[R];
        for (int c = 0; c < R; c++)
            right[c] = -1;
        for (int j = 0; j < m; j++)
            right[pat[j]] = j;
    }

    /**
     * Preprocesses the pattern string.
     *
     * @param pattern the pattern string
     * @param m the length of the pattern
     * @param R the alphabet size
     */
    BoyerMooreStringSearch(char *pattern, int m, int R) {
        this->R = R;
        this->m = m;
        this->pattern = pattern;
        // position of rightmost occurrence of c in the pattern
        right = new int[R];
        for (int c = 0; c < R; c++)
            right[c] = -1;
        for (int j = 0; j < m; j++)
            right[pattern[j]] = j;
    }

    /**
     * Returns the index of the first occurrrence of the pattern string
     * in the text string.
     *
     * @param  txt the text string
     * @return the index of the first occurrence of the pattern string
     *         in the text string; -1 if no such match
     */
    int search(string txt) {
        int n = txt.length();
        int skip;
        for (int i = 0; i <= n - m; i += skip) {
            skip = 0;
            for (int j = m-1; j >= 0; j--) {
                if (pat[j] != txt[i+j]) {
                    skip = max(1, j - right[txt[i+j]]);
                    break;
                }
            }
            if (skip == 0) return i;    // found
        }
        return -1;                      // not found
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
        int skip;
        for (int i = 0; i <= n - m; i += skip) {
            skip = 0;
            for (int j = m-1; j >= 0; j--) {
                if (pattern[j] != text[i+j]) {
                    skip = max(1, j - right[text[i+j]]);
                    break;
                }
            }
            if (skip == 0) return i;    // found
        }
        return -1;                      // not found
    }
};

#endif /* ALGORITHMS_STRING_BOYERMOORESTRINGSEARCH_H_ */
