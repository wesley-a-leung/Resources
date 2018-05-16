#ifndef ALGORITHMS_GRAPH_MATCHING_STABLEMARRIAGE_H_
#define ALGORITHMS_GRAPH_MATCHING_STABLEMARRIAGE_H_

#include <bits/stdc++.h>
using namespace std;

int *curA;
int *mateA;
int *mateB;
int **bRanks;

/*
 * Given N people of type A and N people of type B, and the list of their
 * ranked choices for partners, the goal is to arrange N pairs such that
 * if a person x of type A prefers a person y of type B more than their current
 * partner, then person y prefers their current partner more than x.
 */
void stableMarriage(int N, int **aPrefs, int **bPrefs) {
    curA = new int[N];
    mateA = new int[N];
    mateB = new int[N];
    bRanks = new int*[N];
    for (int i = 0; i < N; i++) {
        bRanks[i] = new int[N];
        for (int j = 0; j < N; j++) bRanks[i][bPrefs[i][j]] = j;
    }
    queue<int> q;
    for (int i = 0; i < N; i++) {
        curA[i] = -1;
        mateB[i] = -1;
        q.push(i);
    }
    while (!q.empty()) {
        int A = q.front();
        q.pop();
        while (true) {
            int B = aPrefs[A][++curA[A]];
            if (mateB[B] == -1) {
                mateB[B] = A;
                break;
            } else if (bRanks[B][A] < bRanks[B][mateB[B]]) {
                q.push(mateB[B]);
                mateB[B] = A;
                break;
            }
        }
    }
    for (int i = 0; i < N; i++) mateA[mateB[i]] = i;
}

#endif /* ALGORITHMS_GRAPH_MATCHING_STABLEMARRIAGE_H_ */
