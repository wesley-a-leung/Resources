#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
#include "Primes.h"
using namespace std;

// Helper function to find a primitive root for the mod p
template <class T> T primitiveRoot(T p) {
  T phi = p - 1, n = phi; vector<T> pf; for (T i = 2; i * i <= n; i++) {
    if (n % i == 0) pf.push_back(i);
    while (n % i == 0) n /= i;
  }
  for (T res = 2; res <= p; res++) {
    bool ok = true; for (int i = 0; i < int(pf.size()) && ok; i++)
      ok &= powMod(res, phi / pf[i], p) != 1;
    if (ok) return res;
  }
  return -1;
}

// Number Theoretic Transform
// Template Arguments:
//   T: the type of the integer, must be integral
//   MOD: the value to mod by, must be in the form C * 2^K + 1 for some
//       integers C, K where size(a) <= 2^K
//     Sample Mods:
//       998244353 = 119 * 2^23 + 1, primitiveRoot = 3
//       754974721 = 45 * 2^24 + 1, primitiveRoot = 11
//       167772161 = 5 * 2^25 + 1, primitiveRoot = 3
//       469762049 = 7 * 2^26 + 1, primitiveRoot = 3
//       1004535809 = 479 * 2^21 + 1, primitiveRoot = 3
//       1004535809 = 483 * 2^21 + 1, primitiveRoot = 5
// Function Arguments:
//   a: a reference to the vector of type T in the range [0, MOD)
// In practice, has a moderate constant, faster than FFT and Karatsuba
// Time Complexity: O(N log N) where N = size(a)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/convolution_mod
//   https://judge.yosupo.jp/problem/convolution_mod_1000000007
template <class T, const T MOD> void ntt(vector<T> &a) {
  static_assert(is_integral<T>::value, "T must be an integral type");
  static_assert(is_signed<T>::value, "T must be a signed type");
#if __cplusplus >= 201402L
  static_assert(isPrime(MOD), "MOD must be prime");
#endif
  static T pk = 1, PK = 0;
  for (; pk < MOD; pk *= 2) if ((MOD - 1) % pk == 0) PK = pk;
  assert(PK != 0 && "MOD must be of the form C * 2^K + 1");
  static T ROOT = powMod(primitiveRoot(MOD), (MOD - 1) / PK, MOD);
  static vector<T> rt(2, 1); static vector<int> ord; static int k = 2, len = 1;
  int N = a.size(); assert(N <= PK); assert(!(N & (N - 1)));
  for (; k < N; k <<= 1, len++) {
    rt.resize(N); T x = powMod(ROOT, PK >> (len + 1), MOD);
    for (int i = k; i < (k << 1); i++)
      rt[i] = i & 1 ? mulMod(rt[i >> 1], x, MOD) : rt[i >> 1];
  }
  if (int(ord.size()) != N) {
    ord.assign(N, 0); int len = __builtin_ctz(N); for (int i = 0; i < N; i++)
      ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
  }
  for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
  for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1)
    for (int j = 0; j < len; j++) {
      T u = a[i + j], v = mulMod(a[len + i + j], rt[len + j], MOD);
      a[i + j] = addMod(u, v, MOD); a[len + i + j] = subMod(u, v, MOD);
    }
}

// Polynomial Multiplication
// Template Arguments:
//   T: the type of the integer, must be integral
//   MOD: the value to mod by, must be in the form C * 2^K + 1 for some
//     integers C, K where size(a) + size(b) - 1 <= 2^K
// Functions Arguments:
//   a: a vector of type T representing the first polynomial, a[i] stores
//     the coefficient of x^i, must be in the range [0, MOD)
//   b: a vector of type T representing the second polynomial, b[i] stores
//     the coefficient of x^i, must be in the range [0, MOD)
//   eq: a boolean indicating whether a and b are equal (to reduce the number
//     of ntt calls)
// Return Value: a vector of type T representing the polynomial a times b with
//   no trailing zeros, all value in the range [0, MOD)
// In practice, has a moderate constant
// Time Complexity: O(N log N) where N = size(a) + size(b)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/convolution_mod
//   https://judge.yosupo.jp/problem/convolution_mod_1000000007
const int NTT_CUTOFF = 30000;
template <class T, const T MOD>
vector<T> mulPolyNTT(const vector<T> &a, const vector<T> &b, bool eq = false) {
  int N = max(0, int(a.size()) + int(b.size()) - 1);
  if ((long long)(a.size()) * (long long)(b.size()) <= NTT_CUTOFF) {
    vector<T> res(N, T()); for (int i = 0; i < int(a.size()); i++)
      for (int j = 0; j < int(b.size()); j++)
        res[i + j] = addMod(res[i + j], mulMod(a[i], b[j], MOD), MOD);
    while (int(res.size()) > 1 && res.back() == T()) res.pop_back();
    return res;
  }
  while (N & (N - 1)) N++;
  vector<T> fa(N, T()), fb, res(N, T());
  copy(a.begin(), a.end(), fa.begin()); ntt<T, MOD>(fa); if (eq) fb = fa;
  else {
    fb.assign(N, T()); copy(b.begin(), b.end(), fb.begin()); ntt<T, MOD>(fb);
  }
  T invN = mulInvModPrime(T(N), MOD);
  res[0] = mulMod(mulMod(fa[0], fb[0], MOD), invN, MOD);
  for (int i = 1; i < N; i++)
    res[N - i] = mulMod(mulMod(fa[i], fb[i], MOD), invN, MOD);
  ntt<T, MOD>(res);
  while (int(res.size()) > 1 && res.back() == T()) res.pop_back();
  return res;
}
