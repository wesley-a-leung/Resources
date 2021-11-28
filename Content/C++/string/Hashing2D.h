#pragma once
#include <bits/stdc++.h>
#include "../math/ModularArithmetic.h"
#include "../utils/Random.h"
using namespace std;

// Computes the hash of a 2D array to allow for easy computation of
//   submatrix hashes
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   HTYPE: the type of the hash
//   HASHES: the number of hashes to compute
// Construtor Arguments:
//   A: the elements in the 2D array
//   offset: the amount to subtract from each element, a - offset + 1 should be
//     positive
//   ROWBASE: array<HTYPE, HASHES> that contains the base for the rows for
//     each hash, must be less than the repective mod and positive (preferably
//     in the range [mod / 4, mod / 4 * 3])
//   COLBASE: array<HTYPE, HASHES> that contains the base for the columns for
//     each hash, must be less than the repective mod and positive (preferably
//     in the range [mod / 4, mod / 4 * 3])
//   MOD: array<HTYPE, HASHES> that contains the mods for each hash, should
//     be large (products should be least (NM)^2) and prime
// Fields:
//   N: the number of rows in the 2D array
//   M: the number of columns in the 2D array
//   ROWBASE: array<HTYPE, HASHES> with the bases for the rows for each hash
//   COLBASE: array<HTYPE, HASHES> with the bases for the columns for each hash
//   MOD: array<HTYPE, HASHES> with the mods for each hash
//   H: a vector of vectors of array<HTYPE, HASHES> of size N + 1 by M + 1 with
//     the suffix hashes starting at each cell
//   ROWPOW: a vector of array<HTYPE, HASHES> of size N + 1 with the ith
//     representing row_base^i modulo mod
//   COLPOW: a vector of array<HTYPE, HASHES> of size M + 1 with the jth
//     representing col_base^j modulo mod
// Functions:
//   initBase(MOD): returns an array<HTYPE, HASHES> with randomly initialized
//     values for the bases for each mod in the range [mod / 4, mod / 4 * 3]
//   getHash(u, d, l, r): returns a type HASH with the hashes in the submatrix
//     [u, d] x [l, r]
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(NM HASHES)
//   getHash: O(HASHES)
// Memory Complexity: O(N HASHES)
template <class T, class HTYPE, const int HASHES> struct Hashing2D {
  using arr = array<HTYPE, HASHES>; int N, M; arr ROWBASE, COLBASE, MOD;
  vector<vector<arr>> H; vector<arr> ROWPOW, COLPOW;
  struct HASH : public arr { int rows, cols; };
  Hashing2D(const vector<vector<T>> &A, T offset, const arr &ROWBASE,
            const arr &COLBASE, const arr &MOD)
      : N(A.size()), M(N == 0 ? 0 : A[0].size()), ROWBASE(ROWBASE),
        COLBASE(COLBASE), MOD(MOD), H(N + 1, vector<arr>(M + 1)),
        ROWPOW(N + 1), COLPOW(M + 1) {
    ROWPOW[0].fill(HTYPE(1)); COLPOW[0].fill(HTYPE(1));
    for (int i = 0; i <= N; i++) H[i][M].fill(HTYPE());
    for (int j = 0; j <= M; j++) H[N][j].fill(HTYPE());
    for (int i = 1; i <= N; i++) for (int h = 0; h < HASHES; h++)
      ROWPOW[i][h] = mulMod(ROWPOW[i - 1][h], ROWBASE[h], MOD[h]);
    for (int j = 1; j <= M; j++) for (int h = 0; h < HASHES; h++)
      COLPOW[j][h] = mulMod(COLPOW[j - 1][h], COLBASE[h], MOD[h]);
    for (int i = N - 1; i >= 0; i--) for (int j = M - 1; j >= 0; j--)
      for (int h = 0; h < HASHES; h++) {
        H[i][j][h] = posMod(HTYPE(A[i][j] - offset) + HTYPE(1), MOD[h]);
        H[i][j][h] = addMod(H[i][j][h],
                            mulMod(H[i + 1][j][h], ROWBASE[h], MOD[h]),
                            MOD[h]);
        H[i][j][h] = addMod(H[i][j][h],
                            mulMod(H[i][j + 1][h], COLBASE[h], MOD[h]),
                            MOD[h]);
        H[i][j][h] = subMod(H[i][j][h],
                            mulMod(mulMod(H[i + 1][j + 1][h], ROWBASE[h],
                                          MOD[h]),
                                   COLBASE[h], MOD[h]),
                            MOD[h]);
      }
  }
  static arr initBase(const arr &MOD) {
    arr base;
    for (int h = 0; h < HASHES; h++) base[h] = uniform_int_distribution<HTYPE>(
        MOD[h] / 4, MOD[h] / 4 * 3)(rng64);
    return base;
  }
  Hashing2D(const vector<vector<T>> &A, T offset, const arr &MOD)
      : Hashing2D(A, offset, initBase(MOD), initBase(MOD), MOD) {}
  HASH getHash(int u, int d, int l, int r) {
    HASH ret; ret.rows = ++d - u; ret.cols = ++r - l;
    for (int h = 0; h < HASHES; h++) {
      ret[h] = H[u][l][h];
      ret[h] = subMod(ret[h], mulMod(H[d][l][h], ROWPOW[d - u][h], MOD[h]),
                      MOD[h]);
      ret[h] = subMod(ret[h], mulMod(H[u][r][h], COLPOW[r - l][h], MOD[h]),
                      MOD[h]);
      ret[h] = addMod(ret[h],
                      mulMod(mulMod(H[d][r][h], ROWPOW[d - u][h], MOD[h]),
                             COLPOW[r - l][h], MOD[h]),
                      MOD[h]);
    }
    return ret;
  }
};
