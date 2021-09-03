#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the longest common subarray of multiple arrays
// Indices are 0-indexed
// Template Arguments:
//   SuffixArray: a generic suffix array to be used (should be either
//       SuffixArray or SAISSuffixArray)
//     Required Fields:
//       N: the length of the array
//       rnk: a vector of the ranks of the suffixes (rnk[i] is the rank of the
//         suffix starting from index i)
//       ind: a vector of the indices in the original array of the suffixes
//         sorted in lexicographical order (ind[i] is the index in original
//         array of the ith lexicographically smallest suffix)
//       LCP: a vector of the longest common prefixes between the suffixes when
//         sorted in lexicographical order (LCP[i] is the longest common prefix
//         of the ith and (i + 1)th lexicographically smallest suffix, with
//         LCP[N - 1] being 0)
//     Required Functions:
//       constructor(S): construts a suffix array from the array S
// Constructor Arguments:
//   A: a vector of the arrays
//   sep: a vector of max(0, A.size() - 1) different separator character
//     that do not appear in any of the arrays
// Fields:
//   SA: the associated suffix array constructed from the arrays
//     concatenated together, separated by the separator character
// In practice, the constructor has a very small constant
// Time Complexity:
//   constructor: time complexity of SuffixArray for the length of the
//     concatenated array
// Memory Complexity: memory complexity of SuffixArray for the length of the
//   concatenated array
// Tested:
//   https://www.spoj.com/problems/LCS2/
//   https://open.kattis.com/problems/longestcommonsubstring
template <class SuffixArray> struct LongestCommonSubstring {
  using T = typename SuffixArray::T;
  vector<int> ind; vector<T> lcs; vector<T> S; SuffixArray SA;
  vector<T> init(const vector<vector<T>> &A, const vector<T> &sep) {
    int len = max(0, int(A.size()) - 1); for (auto &&a : A) len += a.size();
    vector<T> S; S.reserve(len); ind.resize(len, -1);
    for (int i = 0; i < int(A.size()); i++) {
      if (i > 0) S.push_back(sep[i - 1]);
      fill(ind.begin() + S.size(), ind.begin() + S.size() + A[i].size(), i);
      S.insert(S.end(), A[i].begin(), A[i].end());
    }
    return S;
  }
  LongestCommonSubstring(const vector<vector<T>> &A, const vector<T> &sep)
      : ind(), S(init(A, sep)), SA(S) {
    int K = A.size(); if (K == 0) return;
    if (K == 1) { lcs = A[0]; return; }
    vector<int> freq(K, 0), dq(SA.N); pair<int, int> mx(-1, -1);
    for (int l = 0, r = 0, front = 0, back = 0, cnt = 0; r < SA.N; r++) {
      if (ind[SA.ind[r]] != -1) {
        cnt += freq[ind[SA.ind[r]]]++ == 0; if (cnt == K) {
          while (true) {
            int i = ind[SA.ind[l]]; if (i != -1 && freq[i] == 1) break;
            l++; if (i != -1) freq[i]--;
          }
          while (dq[front] < l) front++;
          mx = max(mx, make_pair(SA.LCP[dq[front]], SA.ind[dq[front]]));
        }
      }
      while (front < back && SA.LCP[dq[back - 1]] >= SA.LCP[r]) back--;
      dq[back++] = r;
    }
    if (mx.first != -1) {
      lcs.reserve(mx.first);
      for (int i = 0; i < mx.first; i++) lcs.push_back(S[mx.second + i]);
    }
  }
};
