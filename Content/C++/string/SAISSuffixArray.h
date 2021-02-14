#pragma once
#include <bits/stdc++.h>
using namespace std;

// Suffix Array Induced Sort to sort suffixes of a string in
//   lexicographical order
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   _T: the type of the character/element in the string/array
// Constructor Arguments:
//   N: the length of the string/array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the string/array
//   en: an iterator pointing to after the last element in the string/array
// Fields:
//   T: the type of the character/element in the string/array
//   N: the length of the string/array
//   S: a vector of type T representing the string/array
//   rnk: a vector of the ranks of the suffixes (rnk[i] is the rank of the
//     suffix starting from index i)
//   ind: a vector of the indices in the original string of the suffixes
//     sorted in lexicographical order (ind[i] is the index in original string
//     of the ith lexicographically smallest suffix)
//   LCP: a vector of the longest common prefixes between the suffixes when
//     sorted in lexicographical order (LCP[i] is the longest common prefix of
//     the ith and (i + 1)th lexicographically smallest suffix, with LCP[N - 1]
//     being 0)
// In practice, has a moderate constant, usually faster than SuffixArray.h
// Time Complexity:
//   constructor: O(N + K) where K is the range of the string/array
// Memory Complexity: O(N + K)
// Tested:
//   Fuzz and Stress Tested
//   https://judge.yosupo.jp/problem/suffixarray
//   https://judge.yosupo.jp/problem/number_of_substrings
//   https://dmoj.ca/problem/coci06c5p6
//   https://dmoj.ca/problem/ccc20s3
template <class _T> struct SAISSuffixArray {
  using T = _T;
  static vector<int> SAIS(const vector<int> &S) {
    if (S.empty()) return vector<int>();
    int N = S.size(), K = *max_element(S.begin(), S.end()) + 1;
    vector<bool> isL(N + 1, false); vector<int> ind(N), cnt(K + 1, 0), lms;
    lms.reserve(N); for (auto &&a : S) cnt[a + 1]++;
    partial_sum(cnt.begin(), cnt.end(), cnt.begin());
    isL[N - 1] = true; for (int i = N - 2; i >= 0; i--)
      isL[i] = S[i] == S[i + 1] ? isL[i + 1] : S[i] > S[i + 1];
    auto c = [&] (int i) { return i > 0 && !isL[i] && isL[i - 1]; };
    for (int i = 1; i < N; i++) if (c(i)) lms.push_back(i);
    auto IS = [&] {
      vector<int> tmp(cnt.begin() + 1, cnt.end());
      fill(ind.begin(), ind.end(), -1);
      for (int i = int(lms.size()) - 1; i >= 0; i--)
        ind[--tmp[S[lms[i]]]] = lms[i];
      tmp = vector<int>(cnt.begin(), cnt.end() - 1);
      for (int i = -1, j; i < N; i++)
        if ((j = i < 0 ? N - 1 : ind[i] - 1) >= 0 && isL[j])
          ind[tmp[S[j]]++] = j;
      tmp = vector<int>(cnt.begin() + 1, cnt.end());
      for (int i = N - 1, j; i >= 0; i--)
        if ((j = ind[i] - 1) >= 0 && !isL[j]) ind[--tmp[S[j]]] = j;
    };
    if (int(lms.size()) > 1) {
      IS(); vector<int> tmp(ind.begin(), ind.end());
      for (int i = 0, j = 0, a = -1; i < N; i++) if (c(tmp[i])) {
        for (int b = tmp[i]; a >= 0 && S[a] == S[b];) {
          a++; b++; if (c(a) || c(b)) { j -= int(c(a) && c(b)); break; }
        }
        ind[a = tmp[i]] = ++j;
      }
      tmp = vector<int>(lms.size());
      for (int i = 0; i < int(tmp.size()); i++) tmp[i] = ind[lms[i]];
      tmp = SAIS(tmp);
      for (int i = 0; i < int(tmp.size()); i++) tmp[i] = lms[tmp[i]];
      lms = tmp;
    }
    IS(); return ind;
  }
  static vector<int> init(const vector<T> &S) {
    if (S.empty()) return vector<int>();
    T offset = *min_element(S.begin(), S.end()); vector<int> A(S.size()); 
    for (int i = 0; i < int(S.size()); i++) A[i] = S[i] - offset;
    return A;
  }
  int N; vector<T> S; vector<int> ind, rnk, LCP;
  template <class F> SAISSuffixArray(int N, F f) : N(N) {
    S.reserve(N); for (int i = 0; i < N; i++) S.push_back(f());
    ind = SAIS(init(S)); rnk.resize(N); LCP.resize(N);
    for (int i = 0; i < N; i++) rnk[ind[i]] = i;
    for (int i = 0, k = 0; i < N; i++) {
      if (rnk[i] == N - 1) { LCP[rnk[i]] = k = 0; continue; }
      int j = ind[rnk[i] + 1];
      while (i + k < N && j + k < N && S[i + k] == S[j + k]) k++;
      if ((LCP[rnk[i]] = k) > 0) k--;
    }
  }
  template <class It>
  SAISSuffixArray(It st, It en)
      : SAISSuffixArray(en - st, [&] { return *st++; }) {}
};
