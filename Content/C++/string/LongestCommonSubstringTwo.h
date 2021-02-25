#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the longest common substring of two strings
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
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
//       constructor(N, f): construts a suffix array with a string/array of
//         length N with the generating function that returns the ith element
//          on the ith call
// Constructor Arguments:
//   N: the length of the first string/array
//   f: a generating function that returns the ith element of the first
//     string/array on the ith call
//   M: the length of the second string/array
//   g: a generating function that returns the ith element of the second
//     string/array on the ith call
//   stA: an iterator pointing to the first element in the
//     first string/array
//   enA: an iterator pointing to after the last element in
//     first string/array
//   stB: an iterator pointing to the first element in the
//     second string/array
//   enB: an iterator pointing to after the last element in
//     second string/array
//   sep: a separator character that does not appear in either string/array
// Fields:
//   SA: the associated suffix array constructed from the string/array A,
//     concatenated with the separator, conatenated with the string/array B
//   lcs: the string/array of longest common substring of A and B
// In practice, the constructor has a very small constant
// Time Complexity:
//   constructor: time complexity of SuffixArray for a string of
//     length N + M + 1
// Memory Complexity: memory complexity of SuffixArray for a string of
//   length N + M + 1
// Tested:
//   https://www.spoj.com/problems/LCS/
template <class SuffixArray> struct LongestCommonSubstringTwo {
  using T = typename SuffixArray::T; SuffixArray SA; vector<T> lcs;
  template <class F, class G>
  SuffixArray init(int N, F f, int M, G g, T sep) {
    int i = 0;
    return SuffixArray(N + M + 1, [&] {
      T ret = i < N ? f() : i == N ? sep : g(); i++; return ret;
    });
  }
  template <class F, class G>
  LongestCommonSubstringTwo(int N, F f, int M, G g, T sep)
      : SA(init(N, f, M, g, sep)) {
    pair<int, int> mx(-1, -1); for (int i = 0; i < N + M; i++)
      if (SA.ind[i] != N && SA.ind[i + 1] != N
          && (SA.ind[i] < N) != (SA.ind[i + 1] < N))
        mx = max(mx, make_pair(SA.LCP[i], SA.ind[i]));
    if (mx.first != -1) {
      lcs.reserve(mx.first);
      for (int i = 0; i < mx.first; i++) lcs.push_back(SA.S[mx.second + i]);
    }
  }
  template <class ItPat, class ItTxt>
  LongestCommonSubstringTwo(ItPat stPat, ItPat enPat, ItTxt stTxt, ItTxt enTxt,
                            T sep)
      : LongestCommonSubstringTwo(enPat - stPat, [&] { return *stPat++; },
                                  enTxt - stTxt, [&] { return *stTxt++; },
                                  sep) {}
};
