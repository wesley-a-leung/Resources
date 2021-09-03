#pragma once
#include <bits/stdc++.h>
using namespace std;

// KMP array searching
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element in the array
// Constructor Arguments:
//   pat: the pattern array
// Fields:
//   N: the length of the pattern
//   fail: a vector of integers where the ith value is the fallback index when
//     a match failure occures
// Functions:
//   search(txt): returns a vector of all starting indices of the
//     pattern array in the text txt
// In practice, has a moderate constant, slightly faster than ZAlgorithm
// Time Complexity:
//   constructor: O(N)
//   search, multisearch: O(M)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/bf4
//   https://open.kattis.com/problems/stringmatching
//   https://codeforces.com/contest/1466/problem/G
template <class T> struct KMP {
  int N; vector<T> pat; vector<int> fail;
  KMP(const vector<T> &pat) : N(pat.size()), pat(pat), fail(N + 1, -1) {
    for (int i = 0, j = -1; i < N;) {
      while (j >= 0 && pat[i] != pat[j]) j = fail[j];
      i++; j++; fail[i] = (i != N && pat[i] == pat[j] ? fail[j] : j);
    }
  }
  int search(const vector<T> &txt) {
    for (int i = 0, j = 0; i < int(txt.size()); i++, j++) {
      while (j >= 0 && txt[i] != pat[j]) j = fail[j];
      if (j == N - 1) return i - j;
    }
    return -1;
  }
  vector<int> multiSearch(const vector<T> &txt) {
    vector<int> ret; for (int i = 0, j = 0; i < int(txt.size()); i++, j++) {
      while (j >= 0 && (j == N || txt[i] != pat[j])) j = fail[j];
      if (j == N - 1) ret.push_back(i - j);
    }
    return ret;
  }
};
