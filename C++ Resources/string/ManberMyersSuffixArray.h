#pragma once
#include <bits/stdc++.h>
#include "../datastructures/SparseTable.h"
using namespace std;

// Manber Myers Suffix Array to sort suffixes of a string in
//   lexicographical order
// Indices are 0-indexed
// rnk[i] is the rank of the suffix starting from index i
// ind[i] is the index in original string of the
//   ith lexicographically smallest suffix
// LCP[i] is the longest common prefix of the ith and (i + 1)th
//   lexicographically smallest suffix
// In practice, has a small constant
// Time Complexity: O(S log S)
// Memory Complexity: O(S)
// Tested:
//   https://dmoj.ca/problem/coci06c5p6
//   https://dmoj.ca/problem/ccc20j4
//   https://dmoj.ca/problem/ccc20s3
//   https://dmoj.ca/problem/wac4p4
//   https://www.spoj.com/problems/DISUBSTR/
template <class T> struct ManberMyersSuffixArray {
  int N; vector<T> S; vector<int> ind, rnk, LCP;
  template <class It> ManberMyersSuffixArray(It st, It en)
      : N(en - st), S(st, en), ind(N + 1), rnk(N + 1), LCP(N) {
    vector<T> SS = S; sort(SS.begin(), SS.end()); rnk[ind[N] = N] = -1;
    vector<int> tmp(N + 1, 0);
    for (int i = 0; i < N; i++) {
      rnk[i] = lower_bound(SS.begin(), SS.end(), S[i]) - SS.begin();
      ind[rnk[i] + tmp[rnk[i]]++] = i;
    }
    for (int h = 1; h < N; h += h) for (int r = 1, cnt = 0; r <= N; r++) {
      if (rnk[ind[r - 1]] == rnk[ind[r]]) cnt++;
      else if (cnt > 0) {
        int l = r - cnt - 1; cnt = 0;
        sort(ind.begin() + l, ind.begin() + r, [&] (int a, int b) {
          return rnk[h + a] < rnk[h + b];
        });
        for (int j = l + 1, cur = rnk[ind[l]]; j < r; tmp[ind[j++]] = cur)
          if (rnk[h + ind[j - 1]] < rnk[h + ind[j]]) cur = rnk[ind[l]] + j - l;
        for (int j = l + 1; j < r; j++) rnk[ind[j]] = tmp[ind[j]];
      }
    }
    for (int i = 0, k = 0; i < N; i++) {
      if (rnk[i] == N - 1) { LCP[rnk[i]] = k = 0; continue; }
      int j = ind[rnk[i] + 1];
      while (i + k < N && j + k < N && S[i + k] == S[j + k]) k++;
      if ((LCP[rnk[i]] = k) > 0) k--;
    }
  }
};

// Computes the longest common substring of two prefixes of a string
// Indices are 0-indexed
// lcsRnk(i, j) computes the longest common substring of the ith and jth
//   lexicographically least suffixes
// lcs(i, j) computes the longest common substring of the prefixes starting
//   from index i and j
// In practice, constructor has a small constant, lcs has a moderate constant,
//   still faster than using segment trees
// Time Complexity:
//   constructor: O(S log S)
//   lcsRnk, lcs: O(1)
// Memory Complexity: O(S log S)
// Tested:
//   https://dmoj.ca/problem/ccc20j4
//   https://dmoj.ca/problem/ccc20s3
//   https://dmoj.ca/problem/wac4p4
template <class T> struct LongestCommonSubstring {
  ManberMyersSuffixArray<T> SA; SparseTable<int, function<int(int, int)>> ST;
  template <class It> LongestCommonSubstring(It st, It en)
    : SA(st, en),
      ST(SA.LCP.begin(), SA.LCP.end(),
         [&] (int a, int b) { return min(a, b); }) {}
  int lcsRnk(int i, int j) {
    if (i > j) swap(i, j);
    return i == j ? int(SA.S.size()) - j : ST.query(i, j - 1);
  }
  int lcs(int i, int j) { return lcsRnk(SA.rnk[i], SA.rnk[j]); }
};
