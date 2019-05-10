#pragma once
#include <bits/stdc++.h>
using namespace std;

// Data structure representing a Fraction
template <class T> struct Fraction {
    static_assert(is_integral<T>::value, "T must be an integral type");
    using F = Fraction<T>; T num, den; Fraction(T num = 0, T den = 0) : num(num), den(den) {}
    F reduce() const { T g = __gcd(abs(num), abs(den)); return den >= 0 ? F(num / g, den / g) : F(-num / g, -den / g); }
    F operator + (const F &f) const { return F((num * f.den + f.num * den), den * f.den); }
    F &operator += (const F &f) { num = (num * f.den + f.num * den); den = den * f.den; return *this; }
    F operator - (const F &f) const { return F((num * f.den - f.num * den), den * f.den); }
    F &operator -= (const F &f) { num = (num * f.den - f.num * den); den = den * f.den; return *this; }
    F operator * (const F &f) const { return F(num * f.num, den * f.den); }
    F &operator *= (const F &f) { num = num * f.num; den = den * f.den; return *this; }
    F operator / (const F &f) const { return F(num * f.den, den * f.num); }
    F &operator /= (const F &f) { T t_num = num * f.den, t_den = den * f.num; num = t_num; den = t_den; return *this; }
    F reduce() { T g = __gcd(num, den); return F(num / g, den / g); }
    bool operator < (const F &f) const { return num * f.den < f.num * den; }
    bool operator <= (const F &f) const { return !(f < *this); }
    bool operator > (const F &f) const { return f < *this; }
    bool operator >= (const F &f) const { return !(*this < f); }
    bool operator == (const F &f) const { return !(*this < f) && !(f < *this); }
    bool operator != (const F &f) const { return !(*this == f); }
};
