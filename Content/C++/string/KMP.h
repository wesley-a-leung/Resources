#pragma once
#include <bits/stdc++.h>
using namespace std;

// KMP string searching
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the character/element in the string/array
// Constructor Arguments:
//   N: the length of the pattern string/array
//   f: a generating function that returns the ith element of the pattern
//     string/array on the ith call
//   stPat: an iterator pointing to the first element in the
//     pattern string/array
//   enPat: an iterator pointing to after the last element in
//     pattern string/array
// Fields:
//   N: the length of the string/array
//   pat: a vector of type T representing the pattern string/array
//   LCP: a vector of integers where the ith value is the longest proper prefix
//     of the first i characters that is also a suffix of the
//     first i characters in the string/array, LCP[0] = -1
// Functions:
//   search(M, g): returns a vector of all starting indices of the
//     pattern string in the text string/array generated by the function g
//     which returns the ith element of the text string/array on the ith call
//   search(stTxt, enTxt): returns a vector of all starting indices of the
//     pattern string in the text string/array in the range [stTxt, enTxt)
// In practice, has a moderate constant, slightly faster than ZAlgorithm
// Time Complexity:
//   constructor: O(N)
//   search: O(M)
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/stringmatching
template <class T> struct KMP {
  int N; vector<T> pat; vector<int> LPS;
  template <class F> KMP(int N, F f) : N(N), LPS(N + 1, -1) {
    pat.reserve(N); for (int i = 0, j = -1; i < N;) {
      pat.push_back(f()); while (j >= 0 && pat[i] != pat[j]) j = LPS[j];
      i++; j++; LPS[i] = (i != N && pat[i] == pat[j] ? LPS[j] : j);
    }
  }
  template <class ItPat> KMP(ItPat stPat, ItPat enPat)
      : KMP(enPat - stPat, [&] { return *stPat++; }) {}
  template <class G> vector<int> search(int M, G g) {
    vector<int> matches; vector<T> txt; txt.reserve(M);
    for (int i = 0; i < M; i++) txt.push_back(g());
    for (int i = 0, j = 0; i < M; i++, j++) {
      while (j >= 0 && (j == N || txt[i] != pat[j])) j = LPS[j];
      if (j == N - 1) matches.push_back(i - j);
    }
    return matches;
  }
  template <class ItTxt> vector<int> search(ItTxt stTxt, ItTxt enTxt) {
    return search(enTxt - stTxt, [&] { return *stTxt++; });
  }
};
