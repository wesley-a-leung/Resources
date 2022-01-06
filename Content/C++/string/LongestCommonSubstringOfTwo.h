#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the longest common subarray of two arrays
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
//   A: the first vector of type SuffixArray::T
//   B: the second vector of type SuffixArray::T
//   sep: a separator character that does not appear in either array
// Fields:
//   SA: the associated suffix array constructed from the array A,
//     concatenated with the separator, conatenated with the array B
//   lcs: the longest common subarray of A and B
// In practice, the constructor has a very small constant
// Time Complexity:
//   constructor: time complexity of SuffixArray for an array of
//     length N + M + 1
// Memory Complexity: memory complexity of SuffixArray for an array of
//   length N + M + 1
// Tested:
//   https://www.spoj.com/problems/LCS/
template <class SuffixArray> struct LongestCommonSubstringOfTwo {
  using T = typename SuffixArray::T; SuffixArray SA; vector<T> lcs;
  vector<T> init(vector<T> A, const vector<T> &B, T sep) {
    A.reserve(A.size() + B.size() + 1); A.push_back(sep);
    A.insert(A.end(), B.begin(), B.end()); return A;
  }
  LongestCommonSubstringOfTwo(const vector<T> &A, const vector<T> &B, T sep)
      : SA(init(A, B, sep)) {
    pair<int, int> mx(-1, -1); int N = A.size();
    for (int i = 0; i < N + int(B.size()); i++)
      if (SA.ind[i] != N && SA.ind[i + 1] != N
          && (SA.ind[i] < N) != (SA.ind[i + 1] < N))
        mx = max(mx, make_pair(SA.LCP[i], SA.ind[i]));
    if (mx.first != -1) {
      lcs.reserve(mx.first); for (int i = 0; i < mx.first; i++) {
        int j = mx.second + i; lcs.push_back(j < N ? A[j] : B[j - 1 - N]);
      }
    }
  }
};
