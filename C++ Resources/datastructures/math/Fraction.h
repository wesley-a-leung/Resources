#pragma once
#include <bits/stdc++.h>
#include "../../algorithms/math/Combinatorics.h"
using namespace std;

// Data structure representing a Fraction
template <class T> struct Fraction {
    T num, den;
    Fraction(T num = 0, T den = 0) : num(num), den(den) {}
    Fraction<T> operator + (const Fraction<T> &f) const {
        return Fraction<T>((num * f.den + f.num * den), den * f.den);
    }
    Fraction<T> &operator += (const Fraction<T> &f) {
        num = (num * f.den + f.num * den); den = den * f.den;
        return *this;
    }
    Fraction<T> operator - (const Fraction<T> &f) const {
        return Fraction<T>((num * f.den - f.num * den), den * f.den);
    }
    Fraction<T> &operator -= (const Fraction<T> &f) {
        num = (num * f.den - f.num * den); den = den * f.den;
        return *this;
    }
    Fraction<T> operator * (const Fraction<T> &f) const {
        return Fraction<T>(num * f.num, den * f.den);
    }
    Fraction<T> &operator *= (const Fraction<T> &f) {
        num = num * f.num; den = den * f.den;
        return *this;
    }
    Fraction<T> operator / (const Fraction<T> &f) const {
        return Fraction<T>(num * f.den, den * f.num);
    }
    Fraction<T> &operator /= (const Fraction<T> &f) {
        T t_num = num * f.den, t_den = den * f.num; num = t_num; den = t_den;
        return *this;
    }
    Fraction<T> reduce() {
        T g = __gcd(num, den);
        return Fraction<T>(num / g, den / g);
    }
    bool operator < (const Fraction<T> &f) { return num * f.den < f.num * den; }
    bool operator <= (const Fraction<T> &f) { return !(f < *this); }
    bool operator > (const Fraction<T> &f) { return f < *this; }
    bool operator >= (const Fraction<T> &f) { return !(*this < f); }
    bool operator == (const Fraction<T> &f) { return !(*this < f) && !(f < *this); }
    bool operator != (const Fraction<T> &f) { return !(*this == f); }
};
