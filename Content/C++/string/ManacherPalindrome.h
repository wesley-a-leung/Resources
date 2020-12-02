#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest palindromic substring/subarray centered at each
//   half index
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the character/element in the string/array
// Constructor Arguments:
//   N: the length of the string/array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the string/array
//   en: an iterator pointing to after the last element in the string/array
// Fields:
//   N: the length of the string/array
//   S: a vector of type T representing the string/array
//   SS: a vector of type T of length N * 2 + 1 where the even indices are
//     the element T(), and odd indices are the elements in S
//   p: the length of the longest palindromic substring/subarray centered
//     at each half index (including paddings at each end)
// Functions:
//   lps(): returns a pair containing the starting index and length (in the
//     original string) of the longest palindromic substring/subarray
//   lps(i): returns a pair containing the starting index and length (in the
//     original string) of the longest palindromic substring/subarray centered
//     at index i/2.0
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
//   lps(): O(N)
//   lps(i): O(1)
// Memory Complexity: O(S)
// Tested:
//   https://judge.yosupo.jp/problem/enumerate_palindromes
//   https://www.spoj.com/problems/LPS/
template <class T> struct ManacherPalindrome {
  int N; vector<T> S, SS; vector<int> p;
  template <class F> ManacherPalindrome(int N, F f)
      : N(N), SS(N * 2 + 1, T()), p(N * 2 + 1, 0) {
    S.reserve(N); for (int i = 0; i < N; i++) S.push_back(SS[i * 2 + 1] = f());
    for (int i = 0, cen = 0, mxr = 0; i < N * 2 + 1; i++) {
      if (mxr > i) p[i] = min(mxr - i, p[cen * 2 - i]);
      int l = i - p[i], r = i + p[i];
      for (; l > 0 && r < N * 2 && SS[l - 1] == SS[r + 1]; l--, r++) p[i]++;
      if (r > mxr) { cen = i; mxr = r; } 
    }
  }
  template <class It> ManacherPalindrome(It st, It en)
      : ManacherPalindrome(en - st, [&] { return *st++; }) {}
  pair<int, int> lps() {
    int len = 0, cen = 0;
    for (int i = 0; i < N * 2 + 1; i++) if (p[i] > len) len = p[cen = i];
    return make_pair((cen - len) / 2, len);
  }
  pair<int, int> lps(int i) {
    int len = p[i + 1], cen = i + 1; return make_pair((cen - len) / 2, len);
  }
};
