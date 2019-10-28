#pragma once
#include <bits/stdc++.h>
#include "Combinatorics.h"
using namespace std;

// Number Theoretic Transform
// Time Complexity of multiplyPolynomial: O(N log N) where N = size(a) + size(b)

using T = long long; const int CUTOFF = 150;

T primitiveRoot(T p) {
    vector<T> fact; T phi = p - 1, n = phi;
    for (T i = 2; i * i <= n; i++) {
        if (n % i == 0) fact.push_back(i);
        while(n % i == 0) n /= i;
    }
    if (n > 1) fact.push_back(n);
    for (T res = 2; res <= p; res++) {
        bool ok = true;
        for (int i = 0; i < int(fact.size()) && ok; i++) ok &= powMod(res, phi / fact[i], p) != 1;
        if (ok) return res;
    }
    assert(false); return -1;
}

// MOD = C * 2 ^ K + 1, prime
// for MOD < 1e9:
//   C = 119, K = 23 (998244353)
//   C = 5, K = 25 (167772161)
//   C = 7, K = 26 (469762049)
// for MOD > 1e9:
//   C = 479, K = 21 (1004535809)
//   C = 483, K = 21 (1012924417)
const T C = 119, K = 23, PK = 1 << K, MOD = C * PK + 1, ROOT = powMod(primitiveRoot(MOD), C, MOD);

vector<int> ord; vector<T> roots;

void computeRoots(int N) {
    roots = {0, 1};
    int len = __builtin_ctz(int(roots.size())); roots.resize(N);
    for (; (1 << len) < N; len++) {
        T z = powMod(ROOT, PK >> (len + 1), MOD);
        for (int i = 1 << (len - 1); i < (1 << len); i++) { roots[2 * i] = roots[i]; roots[2 * i + 1] = mulMod(roots[i], z, MOD); }
    }
}

void reorder(vector<T> &a) {
    int N = int(a.size());
    if (int(ord.size()) != N) {
        ord.assign(N, 0); int len = __builtin_ctz(N);
        for (int i = 0; i < N; i++) ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
    }
    for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
}

void ntt(vector<T> &a) {
    int N = int(a.size()); computeRoots(N); reorder(a);
    for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1) for (int j = 0; j < len; j++) {
        T u = a[i + j], v = mulMod(a[len + i + j], roots[len + j], MOD);
        a[i + j] = addMod(u, v, MOD); a[len + i + j] = subMod(u, v, MOD);
    }
}

// Multiplies 2 polynomials modulo a prime
template <class T> void multiplyPolynomial(const vector<T> &a, const vector<T> &b, vector<T> &res, bool eq = false) {
    static_assert(is_integral<T>::value, "T must be an integral type");
    if (max(int(a.size()), int(b.size())) <= CUTOFF) {
        vector<T> c(int(a.size()) + int(b.size()) - 1, 0);
        for (int i = 0; i < int(a.size()); i++) {
            T ai = posMod(a[i], MOD);
            for (int j = 0; j < int(b.size()); j++) c[i + j] = addMod(c[i + j], mulMod(ai, posMod(b[j], MOD), MOD), MOD);
        }
        res.resize(int(a.size()) + int(b.size()) - 1, 0); copy(c.begin(), c.end(), res.begin());
        while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
        return;
    }
    int N = int(a.size()) + int(b.size()) - 1;
    while (N & (N - 1)) N++;
    vector<T> fa(N, 0), fb; copy(a.begin(), a.end(), fa.begin()); ntt(fa);
    if (eq) fb = fa;
    else { fb.assign(N, 0); copy(b.begin(), b.end(), fb.begin()); ntt(fb); }
    res.resize(N); T invN = mulInvPrime(T(N), MOD); res[0] = mulMod(mulMod(fa[0], fb[0], MOD), invN, MOD);
    for (int i = 1; i < N; i++) res[N - i] = mulMod(mulMod(fa[i], fb[i], MOD), invN, MOD);
    ntt(res);
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
