#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given N people of type A and M people of type B, and a list of their ranked
//   preferences for partners, the goal is to arrange min(N, M) pairs such that
//   if a person x of type A prefers a person y of type B more than their
//   current partner, then person y prefers their current partner more than x;
//   it is guaranteed that a solution always exists
// Constructor Arguments:
//   aPrefs: a matrix of size N x M with aPrefs[i][j] representing the jth
//     preferred choice for the partner of type B for person i of type A;
//     aPrefs[i] must be a permutation from 0 to M - 1
//   bPrefs: a matrix of size M x N with bPrefs[j][i] representing the ith
//     preferred choice for the partner of type A for person j of type B;
//     bPrefs[j] must be a permutation from 0 to N - 1
// Fields:
//   N: the number of people of type A
//   M: the number of people of type A
//   bForA: a vector representing the partner of type B for each person of
//     type A, or -1 if that person of type A is unmatched
//   aForB: a vector representing the partner of type A for each person of
//     type B, or -1 if that person of type B is unmatched
// Time Complexity:
//   constructor O(NM)
// Memory Complexity: O(NM)
// Tested:
//   https://www.spoj.com/problems/STABLEMP/
//   https://open.kattis.com/problems/jealousyoungsters
struct StableMarriage {
  int N, M; vector<int> bForA, aForB;
  StableMarriage(const vector<vector<int>> &aPrefs,
                 const vector<vector<int>> &bPrefs)
      : N(aPrefs.size()), M(bPrefs.size()), bForA(N, -1), aForB(M, -1) {
    bool rev = N > M; if (rev) { swap(N, M); bForA.swap(aForB); }
    auto &A = rev ? bPrefs : aPrefs, &B = rev ? aPrefs : bPrefs;
    vector<vector<int>> bRankOfA(M, vector<int>(N));
    for (int b = 0; b < M; b++) for (int a = 0; a < N; a++)
      bRankOfA[b][B[b][a]] = a;
    queue<int> q; for (int a = 0; a < N; a++) q.push(a);
    vector<int> cur(N, 0); while (!q.empty()) {
      int a = q.front(); q.pop(); while (true) {
        int b = A[a][cur[a]++]; if (aForB[b] == -1) { aForB[b] = a; break; }
        else if (bRankOfA[b][a] < bRankOfA[b][aForB[b]]) {
          q.push(aForB[b]); aForB[b] = a; break;
        }
      }
    }
    for (int b = 0; b < M; b++) if (aForB[b] != -1) bForA[aForB[b]] = b;
    if (rev) { swap(N, M); bForA.swap(aForB); }
  }
};
