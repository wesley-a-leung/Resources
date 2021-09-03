#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes an array z, where z[i] is the length of the longest subarray
//   starting at S[i], which is also a prefix of S (maximum k such
//   that S[i + j] == S[j] for all 0 <= j < k)
// Indices are 0-indexed and ranges are inclusive
//   T: the type of each element in the array
// Constructor Arguments:
//   S: a vector of type T
// Fields:
//   N: the length of the array
//   S: a vector of type T representing the array
//   z: a vector of integers where the ith element is the length of the
//     longest subarray starting at index i which is also a prefix of S
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/zalgorithm
//   https://open.kattis.com/problems/stringmatching
template <class T> struct ZAlgorithm {
  int N; vector<int> z;
  ZAlgorithm(const vector<T> &S) : N(S.size()), z(N, 0) {
    z[0] = N; for (int i = 1, l = 0, r = 0; i < N; i++) {
      if (i <= r) z[i] = min(r - i + 1, z[i - l]);
      while (i + z[i] < N && S[z[i]] == S[i + z[i]]) z[i]++;
      if (i + z[i] - 1 > r) r = (l = i) + z[i] - 1;
    }
  }
};

// Finds all starting indices of a pattern array in a text array
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element in the array
// Constructor Arguments:
//   pat: the pattern array
//   txt: the text array
// Fields:
//   Z: the associated ZAlgorithm object constructed from the concatenation
//     of the pattern and the text array
//   matches: a vector of all starting indices of the pattern array in
//     the text array
// In practice, has a moderate constant, slightly slower than KMP
// Time Complexity:
//   constructor: O(N + M)
// Memory Complexity: O(N + M)
// Tested:
//   https://open.kattis.com/problems/stringmatching
template <class T> struct StringMatching {
  ZAlgorithm<T> Z; vector<int> matches;
  vector<T> init(vector<T> pat, const vector<T> &txt) {
    pat.reserve(pat.size() + txt.size());
    pat.insert(pat.end(), txt.begin(), txt.end()); return pat;
  }
  StringMatching(const vector<T> &pat, const vector<T> &txt)
      : Z(init(pat, txt)) {
    for (int i = 0; i < int(txt.size()); i++)
      if (Z.z[pat.size() + i] >= int(pat.size())) matches.push_back(i);
  }
};
