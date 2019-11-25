#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given N people of type A and M people of type B, and a list of their ranked choices
// for partners, the goal is to arrange N pairs such that if a person x of type A prefers
// a person y of type B more than their current partner, then person y prefers their
// current partner more than x
// A solution is guaranteed to exist if N <= M
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
template <const int MAXNM> struct StableMarriage {
    int aPrefs[MAXNM][MAXNM], bPrefs[MAXNM][MAXNM], curA[MAXNM], mateA[MAXNM], mateB[MAXNM], bRanks[MAXNM][MAXNM], q[MAXNM * MAXNM];
    bool solve(int N, int M) { // returns true if there's a solution, always guaranteed if N <= M
        if (N > M) return false;
        for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) bRanks[i][bPrefs[i][j]] = j;
        int front = 0, back = 0; fill(curA, curA + N, -1); fill(mateA, mateA + N, -1); fill(mateB, mateB + M, -1);
        for (int i = 0; i < N; i++) q[back++] = i;
        while (front < back) {
            int A = q[front++];
            while (true) {
                int B = aPrefs[A][++curA[A]];
                if (mateB[B] == -1) { mateB[B] = A; break; }
                else if (bRanks[B][A] < bRanks[B][mateB[B]]) { q[back++] = mateB[B]; mateB[B] = A; break; }
            }
        }
        for (int i = 0; i < M; i++) if (mateB[i] != -1) mateA[mateB[i]] = i;
        return true;
    }
};
