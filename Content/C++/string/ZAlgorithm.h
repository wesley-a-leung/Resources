#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes an array z, where z[i] is the length of the longest substring
//   starting at S[i], which is also a prefix of S (maximum k such
//   that S[i + j] == S[j] for all 0 <= j < k)
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
//   z: a vector of integers where the ith element is the length of the
//     longest substring starting at index i which is also a prefix of S
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/zalgorithm
//   https://open.kattis.com/problems/stringmatching
template <class T> struct ZAlgorithm {
  int N; vector<T> S; vector<int> z;
  template <class F> ZAlgorithm(int N, F f) : N(N), z(N, 0) {
    S.reserve(N); for (int i = 0; i < N; i++) S.push_back(f());
    z[0] = N; for (int i = 1, l = 0, r = 0; i < N; i++) {
      if (i <= r) z[i] = min(r - i + 1, z[i - l]);
      while (i + z[i] < N && S[z[i]] == S[i + z[i]]) z[i]++;
      if (i + z[i] - 1 > r) r = (l = i) + z[i] - 1;
    }
  }
  template <class It> ZAlgorithm(It st, It en)
      : ZAlgorithm(en - st, [&] { return *st++; }) {}
};

// Finds all starting indices of a pattern string in a text string
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the character/element in the string/array
// Constructor Arguments:
//   N: the length of the pattern string/array
//   f: a generating function that returns the ith element of the pattern
//     string/array on the ith call
//   M: the length of the text string/array
//   g: a generating function that returns the ith element of the text
//     string/array on the ith call
//   stPat: an iterator pointing to the first element in the
//     pattern string/array
//   enPat: an iterator pointing to after the last element in
//     pattern string/array
//   stTxt: an iterator pointing to the first element in the
//     text string/array
//   enTxt: an iterator pointing to after the last element in
//     text string/array
// Fields:
//   Z: the associated ZAlgorithm object constructed from the concatenation
//     of the pattern and the text string
//   matches: a vector of all starting indices of the pattern string in
//     the text string
// In practice, has a moderate constant, slightly slower than KMP
// Time Complexity:
//   constructor: O(N + M)
// Memory Complexity: O(N + M)
// Tested:
//   https://open.kattis.com/problems/stringmatching
template <class T> struct StringMatching {
  ZAlgorithm<T> Z; vector<int> matches;
  template <class F, class G> ZAlgorithm<T> init(int N, F f, int M, G g) {
    int i = 0;
    return ZAlgorithm<T>(N + M, [&] { return i++ < N ? f() : g(); });
  }
  template <class F, class G> StringMatching(int N, F f, int M, G g)
      : Z(init(N, f, M, g)) {
    for (int i = 0; i < M; i++) if (Z.z[N + i] >= N) matches.push_back(i);
  }
  template <class ItPat, class ItTxt>
  StringMatching(ItPat stPat, ItPat enPat, ItTxt stTxt, ItTxt enTxt)
      : StringMatching(enPat - stPat, [&] { return *stPat++; },
                       enTxt - stTxt, [&] { return *stTxt++; }) {}
};
