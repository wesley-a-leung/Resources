#pragma once
#include <bits/stdc++.h>
#include "../../algorithms/math/Combinatorics.h"
using namespace std;

using T = long long;
T MOD = 1e9 + 7;

// Data structure representing a Fraction modulo MOD
struct FractionMod {
    T num, den;
    FractionMod(T num = 0, T den = 0) {
        num %= MOD; den %= MOD;
        if (num < 0) num += MOD;
        if (den < 0) den += MOD;
        this->num = num; this->den = den;
    }
    FractionMod operator + (const FractionMod &f) const {
        return FractionMod((num * f.den % MOD + f.num * den % MOD) % MOD, den * f.den % MOD);
    }
    FractionMod &operator += (const FractionMod &f) {
        num = (num * f.den % MOD + f.num * den % MOD) % MOD; den = den * f.den % MOD;
        return *this;
    }
    FractionMod operator - (const FractionMod &f) const {
        return FractionMod((num * f.den % MOD - f.num * den % MOD + MOD) % MOD, den * f.den % MOD);
    }
    FractionMod &operator -= (const FractionMod &f) {
        num = (num * f.den % MOD - f.num * den % MOD + MOD) % MOD; den = den * f.den % MOD;
        return *this;
    }
    FractionMod operator * (const FractionMod &f) const {
        return FractionMod(num * f.num % MOD, den * f.den % MOD);
    }
    FractionMod &operator *= (const FractionMod &f) {
        num = num * f.num % MOD; den = den * f.den % MOD;
        return *this;
    }
    FractionMod operator / (const FractionMod &f) const {
        return FractionMod(num * f.den % MOD, den * f.num % MOD);
    }
    FractionMod &operator /= (const FractionMod &f) {
        T t_num = num * f.den % MOD, t_den = den * f.num % MOD; num = t_num; den = t_den;
        return *this;
    }
    bool operator == (const FractionMod &f) const { return divMod(num, den, MOD) == divMod(f.num, f.den, MOD); }
    bool operator != (const FractionMod &f) const { return !(*this == f); }
};
