#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the longest common substring of multiple strings
// Indices are 0-indexed
// Template Arguments:
//   SuffixArray: a generic suffix array to be used (should be either
//       SuffixArray in SuffixArray or SAISSuffixArray in SAISSuffixArray)
//     Required Fields:
//       T: the type of each element in the string/array
//       S: a vector of type T representing the string/array
//       ind: a vector of the indices in the original string of the suffixes
//         sorted in lexicographical order (ind[i] is the index in original
//         string of the ith lexicographically smallest suffix)
//       LCP: a vector of the longest common prefixes between the suffixes when
//         sorted in lexicographical order (LCP[i] is the longest common prefix
//         of the ith and (i + 1)th lexicographically smallest suffix, with
//         LCP[N - 1] being 0)
//     Required Functions:
//       constructor(st, en): construts a suffix array with a string/array
//         represented by the elements between the iterators st and en
// Constructor Arguments:
//   A: a vector of the string/array
//   sep: a vector of max(0, A.size() - 1) different separator character
//     that do not appear in any of the string/array
// Fields:
//   SA: the associated suffix array constructed from the strings/arrays
//     concatenated together, separated by the separator character
// In practice, the constructor has a very small constant
// Time Complexity:
//   constructor: time complexity of SuffixArray for the length of the
//     concatenated string
// Memory Complexity: memory complexity of SuffixArray for the length of the
//   concatenated string
// Tested:
//   https://www.spoj.com/problems/LCS2/
template <class SuffixArray> struct LongestCommonSubstring {
  using T = typename SuffixArray::T;
  vector<int> ind; vector<T> lcs; SuffixArray SA;
  SuffixArray init(const vector<vector<T>> &A, const vector<T> &sep) {
    int len = max(0, int(A.size()) - 1); for (auto &&a : A) len += a.size();
    vector<T> S; S.reserve(len); ind.resize(len, -1);
    for (int i = 0; i < int(A.size()); i++) {
      if (i > 0) S.push_back(sep[i - 1]);
      fill(ind.begin() + S.size(), ind.begin() + S.size() + A[i].size(), i);
      S.insert(S.end(), A[i].begin(), A[i].end());
    }
    return SuffixArray(S.begin(), S.end());
  }
  LongestCommonSubstring(const vector<vector<T>> &A, const vector<T> &sep)
      : ind(), SA(init(A, sep)) {
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
      for (int i = 0; i < mx.first; i++) lcs.push_back(SA.S[mx.second + i]);
    }
  }
};
