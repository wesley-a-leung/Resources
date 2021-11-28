#pragma once
#include <bits/stdc++.h>
#include "../math/ModularArithmetic.h"
#include "../utils/Random.h"
using namespace std;

// Computes the hash of an array to allow for easy computation of
//   subarray hashes
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   HTYPE: the type of the hash
//   HASHES: the number of hashes to compute
// Construtor Arguments:
//   S: the elements in the array
//   offset: the amount to subtract from each element, a - offset + 1 should be
//     positive
//   BASE: array<HTYPE, HASHES> that contains the base for each hash, must
//     be less than the repective mod and positive (preferably in the range
//     [mod / 4, mod / 4 * 3])
//   MOD: array<HTYPE, HASHES> that contains the mods for each hash, should
//     be large (products should be least N^2) and prime
// Fields:
//   N: the length of the array
//   BASE: array<HTYPE, HASHES> with the bases for each hash
//   MOD: array<HTYPE, HASHES> with the mods for each hash
//   H: a vector of array<HTYPE, HASHES> of size N + 1 with the suffix hashes
//     starting at each index
//   POW: a vector of array<HTYPE, HASHES> of size N + 1 with the ith
//     representing base^i modulo mod
// Functions:
//   initBase(MOD): returns an array<HTYPE, HASHES> with randomly initialized
//     values for the bases for each mod in the range [mod / 4, mod / 4 * 3]
//   getHash(l, r): returns a type HASH with the hashes in the subarray [l, r]
//   merge(h1, h2): returns a type HASH of 2 hashes merged together as if
//     the subarray represented by h2 was immediately after h1
//   concat(l1, r1, l2, r2): returns a type HASH of
//     the subarray [l1, r1] contactenated with the subarray [l2, r2]
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N HASHES)
//   getHash, merge, concat: O(HASHES)
// Memory Complexity: O(N HASHES)
// Tested:
//   https://dmoj.ca/problem/globexcup19j5hard
template <class T, class HTYPE, const int HASHES> struct Hashing {
  using arr = array<HTYPE, HASHES>; int N; arr BASE, MOD; vector<arr> H, POW;
  struct HASH : public arr { int len; };
  Hashing(const vector<T> &S, T offset, const arr &BASE, const arr &MOD)
      : N(S.size()), BASE(BASE), MOD(MOD), H(N + 1), POW(N + 1) {
    POW[0].fill(HTYPE(1)); H[N].fill(HTYPE());
    for (int i = 1; i <= N; i++) for (int h = 0; h < HASHES; h++)
      POW[i][h] = mulMod(POW[i - 1][h], BASE[h], MOD[h]);
    for (int i = N - 1; i >= 0; i--) for (int h = 0; h < HASHES; h++)
      H[i][h] = addMod(mulMod(H[i + 1][h], BASE[h], MOD[h]),
                       posMod(HTYPE(S[i] - offset) + HTYPE(1), MOD[h]),
                       MOD[h]);
  }
  static arr initBase(const arr &MOD) {
    arr base;
    for (int h = 0; h < HASHES; h++) base[h] = uniform_int_distribution<HTYPE>(
        MOD[h] / 4, MOD[h] / 4 * 3)(rng64);
    return base;
  }
  Hashing(const vector<T> &S, T offset, const arr &MOD)
      : Hashing(S, offset, initBase(MOD), MOD) {}
  HASH getHash(int l, int r) {
    HASH ret; ret.len = ++r - l; for (int h = 0; h < HASHES; h++)
      ret[h] = subMod(H[l][h], mulMod(H[r][h], POW[r - l][h], MOD[h]), MOD[h]);
    return ret;
  }
  HASH merge(const HASH &h1, const HASH &h2) {
    HASH ret; ret.len = h1.len + h2.len; for (int h = 0; h < HASHES; h++)
      ret[h] = addMod(h1[h], mulMod(h2[h], POW[h1.len][h], MOD[h]), MOD[h]);
    return ret;
  }
  HASH concat(int l1, int r1, int l2, int r2) {
    return merge(getHash(l1, r1), getHash(l2, r2));
  }
};
