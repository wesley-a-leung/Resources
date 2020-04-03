#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
using namespace std;

// Number Theoretic Transform
// Time Complexity of multiplyPolynomial: O(N log N) where N = size(a) + size(b)

using T = long long; const int CUTOFF = 150;
static_assert(is_integral<T>::value, "T must be an integral type");

T primitiveRoot(T p) {
    vector<T> fact; T phi = p - 1, n = phi;
    for (T i = 2; i * i <= n; i++) {
        if (n % i == 0) fact.push_back(i);
        while (n % i == 0) n /= i;
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
//   C = 119, K = 23 (998244353), primitiveRoot = 3
//   C = 5, K = 25 (167772161), primitiveRoot = 3
//   C = 7, K = 26 (469762049), primitiveRoot = 3
// for MOD > 1e9:
//   C = 479, K = 21 (1004535809), primitiveRoot = 3
//   C = 483, K = 21 (1012924417), primitiveRoot = 5
const T C = 119, K = 23, PK = 1 << K, MOD = C * PK + 1, ROOT = powMod(primitiveRoot(MOD), C, MOD);

void ntt(vector<T> &a) {
    int N = int(a.size()); static vector<T> rt(2, 1); static vector<int> ord;
    for (static int k = 2, len = 1; k < N; k <<= 1, len++) {
        rt.resize(N); T x = powMod(ROOT, PK >> (len + 1), MOD);
        for (int i = k; i < (k << 1); i++) rt[i] = i & 1 ? mulMod(rt[i >> 1], x, MOD) : rt[i >> 1];
    }
    if (int(ord.size()) != N) {
        ord.assign(N, 0); int len = __builtin_ctz(N);
        for (int i = 0; i < N; i++) ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
    }
    for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
    for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1) for (int j = 0; j < len; j++) {
        T u = a[i + j], v = mulMod(a[len + i + j], rt[len + j], MOD);
        a[i + j] = addMod(u, v, MOD); a[len + i + j] = subMod(u, v, MOD);
    }
}

// Multiplies 2 polynomials modulo a prime
void multiply(const vector<T> &a, const vector<T> &b, vector<T> &res, bool eq = false) {
    if (max(int(a.size()), int(b.size())) <= CUTOFF) {
        vector<T> c(int(a.size()) + int(b.size()) - 1, 0);
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) {
            c[i + j] = addMod(c[i + j], mulMod(a[i], b[j], MOD), MOD);
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
