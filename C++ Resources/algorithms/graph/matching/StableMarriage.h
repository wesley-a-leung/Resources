#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given N people of type A and N people of type B, and a list of their ranked choices
// for partners, the goal is to arrange N pairs such that if a person x of type A prefers
// a person y of type B more than their current partner, then person y prefers their
// current partner more than x
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
template <const int MAXN> struct StableMarriage {
    int aPrefs[MAXN][MAXN], bPrefs[MAXN][MAXN], curA[MAXN], mateA[MAXN], mateB[MAXN], bRanks[MAXN][MAXN];
    void solve(int N) {
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) bRanks[i][bPrefs[i][j]] = j;
        queue<int> q;
        for (int i = 0; i < N; i++) { curA[i] = mateB[i] = -1; q.push(i); }
        while (!q.empty()) {
            int A = q.front(); q.pop();
            while (true) {
                int B = aPrefs[A][++curA[A]];
                if (mateB[B] == -1) { mateB[B] = A; break; }
                else if (bRanks[B][A] < bRanks[B][mateB[B]]) { q.push(mateB[B]); mateB[B] = A; break; }
            }
        }
        for (int i = 0; i < N; i++) mateA[mateB[i]] = i;
    }
};
