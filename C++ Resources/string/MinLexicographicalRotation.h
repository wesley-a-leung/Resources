#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum lexiographical rotation of a string
// Time Complexity: O(S)
// Memory Complexity: O(S) additional memory
template <const int MAXS> struct MinLexicographicalRotation {
    int F[MAXS * 2];
    int run(const string &S) { // returns the index of the start the minimum rotation
        string T = S + S; fill(F, F + T.size(), -1); int k = 0;
        for (int i = 1; i < int(T.size()); i++) {
            int j = F[i - k - 1];
            while (j != -1 && T[i] != T[j + k + 1]) {
                if (T[i] < T[j + k + 1]) k = i - j - 1;
                j = F[j];
            }
            if (T[i] != T[j + k + 1]) {
                if (T[i] < T[k]) k = i;
                F[i - k] = -1;
            } else F[i - k] = j + 1;
        }
        return k;
    }
};
