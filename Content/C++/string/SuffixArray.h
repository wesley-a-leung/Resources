#pragma once
#include <bits/stdc++.h>
using namespace std;

// Suffix Array using Sadakane's algorithm to sort suffixes of an array in
//   lexicographical order
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   _T: the type of each element in the array
// Constructor Arguments:
//   S: a vector of type _T
// Fields:
//   T: the type of the character/element in the array
//   N: the length of the array
//   rnk: a vector of the ranks of the suffixes (rnk[i] is the rank of the
//     suffix starting from index i)
//   ind: a vector of the indices in the original array of the suffixes
//     sorted in lexicographical order (ind[i] is the index in original array
//     of the ith lexicographically smallest suffix)
//   LCP: a vector of the longest common prefixes between the suffixes when
//     sorted in lexicographical order (LCP[i] is the longest common prefix of
//     the ith and (i + 1)th lexicographically smallest suffix, with LCP[N - 1]
//     being 0)
// In practice, has a very small constant, usually slower than
//   SAISSuffixArray.h
// Time Complexity:
//   constructor: O(N (log N)^2)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://judge.yosupo.jp/problem/suffixarray
//   https://judge.yosupo.jp/problem/number_of_substrings
//   https://dmoj.ca/problem/coci06c5p6
//   https://dmoj.ca/problem/ccc20s3
template <class _T> struct SuffixArray {
  using T = _T; int N; vector<int> ind, rnk, LCP;
  SuffixArray(const vector<T> &S)
      : N(S.size()), ind(N + 1), rnk(N + 1), LCP(N + 1) {
    vector<int> &tmp = LCP; iota(ind.begin(), ind.end(), 0);
    sort(ind.begin(), ind.begin() + N, [&] (int a, int b) {
      return S[a] < S[b];
    });
    rnk[ind[N]] = -1; for (int i = 0; i < N; i++) {
      rnk[ind[i]] = i > 0 && S[ind[i]] == S[ind[i - 1]] ? rnk[ind[i - 1]] : i;
    }
    for (int h = 1; h < N; h += h) for (int l = 0, r = 1; r <= N; r++) {
      if (rnk[ind[r - 1]] != rnk[ind[r]] && l + 1 < r) {
        sort(ind.begin() + l, ind.begin() + r, [&] (int a, int b) {
          return rnk[h + a] < rnk[h + b];
        });
        tmp[l] = l; for (int j = l + 1; j < r; j++)
          tmp[j] = rnk[h + ind[j - 1]] < rnk[h + ind[j]] ? j : tmp[j - 1];
        for (l++; l < r; l++) rnk[ind[l]] = tmp[l];
      } else if (rnk[ind[r - 1]] != rnk[ind[r]]) l++;
    }
    ind.pop_back(); rnk.pop_back(); tmp.pop_back();
    for (int i = 0, k = 0; i < N; i++) {
      if (rnk[i] == N - 1) { LCP[rnk[i]] = k = 0; continue; }
      int j = ind[rnk[i] + 1];
      while (i + k < N && j + k < N && S[i + k] == S[j + k]) k++;
      if ((LCP[rnk[i]] = k) > 0) k--;
    }
  }
};
