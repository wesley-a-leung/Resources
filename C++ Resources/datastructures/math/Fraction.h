#pragma once
#include <bits/stdc++.h>
#include "../../algorithms/math/Combinatorics.h"
using namespace std;

using T = long long; static_assert(is_integral<T>::value, "T must be an integral type");

// Data structure representing a Fraction
struct Fraction {
    T num, den;
    Fraction(T num = 0, T den = 0) : num(num), den(den) {}
    Fraction reduce() const {
        T g = __gcd(abs(num), abs(den));
        return den >= 0 ? Fraction(num / g, den / g) : Fraction(-num / g, -den / g);
    }
    Fraction operator + (const Fraction &f) const {
        return Fraction((num * f.den + f.num * den), den * f.den);
    }
    Fraction &operator += (const Fraction &f) {
        num = (num * f.den + f.num * den); den = den * f.den;
        return *this;
    }
    Fraction operator - (const Fraction &f) const {
        return Fraction((num * f.den - f.num * den), den * f.den);
    }
    Fraction &operator -= (const Fraction &f) {
        num = (num * f.den - f.num * den); den = den * f.den;
        return *this;
    }
    Fraction operator * (const Fraction &f) const {
        return Fraction(num * f.num, den * f.den);
    }
    Fraction &operator *= (const Fraction &f) {
        num = num * f.num; den = den * f.den;
        return *this;
    }
    Fraction operator / (const Fraction &f) const {
        return Fraction(num * f.den, den * f.num);
    }
    Fraction &operator /= (const Fraction &f) {
        T t_num = num * f.den, t_den = den * f.num; num = t_num; den = t_den;
        return *this;
    }
    Fraction reduce() {
        T g = __gcd(num, den);
        return Fraction(num / g, den / g);
    }
    bool operator < (const Fraction &f) const { return num * f.den < f.num * den; }
    bool operator <= (const Fraction &f) const { return !(f < *this); }
    bool operator > (const Fraction &f) const { return f < *this; }
    bool operator >= (const Fraction &f) const { return !(*this < f); }
    bool operator == (const Fraction &f) const { return !(*this < f) && !(f < *this); }
    bool operator != (const Fraction &f) const { return !(*this == f); }
};
