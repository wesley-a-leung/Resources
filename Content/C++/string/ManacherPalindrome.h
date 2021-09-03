#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest palindromic subarray centered at each half index
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element in the array
// Constructor Arguments:
//   S: a vector of type T
// Fields:
//   N: the length of the array
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
  int N; vector<int> p;
  ManacherPalindrome(const vector<T> &S) : N(S.size()), p(N * 2 + 1, 0) {
    vector<T> SS(N * 2 + 1, T());
    for (int i = 0; i < N; i++) SS[i * 2 + 1] = S[i];
    for (int i = 0, cen = 0, mxr = 0; i < N * 2 + 1; i++) {
      if (mxr > i) p[i] = min(mxr - i, p[cen * 2 - i]);
      int l = i - p[i], r = i + p[i];
      for (; l > 0 && r < N * 2 && SS[l - 1] == SS[r + 1]; l--, r++) p[i]++;
      if (r > mxr) { cen = i; mxr = r; }
    }
  }
  pair<int, int> lps() {
    int len = 0, cen = 0;
    for (int i = 0; i < N * 2 + 1; i++) if (p[i] > len) len = p[cen = i];
    return make_pair((cen - len) / 2, len);
  }
  pair<int, int> lps(int i) {
    int len = p[i + 1], cen = i + 1; return make_pair((cen - len) / 2, len);
  }
};
