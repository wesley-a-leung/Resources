#pragma once
#include <bits/stdc++.h>
#include "GCD.h"
using namespace std;

// Data structure representing a fraction
// Template Arguments:
//   T: the type of the numerator and denominator, must be integral
// Constructor Arguments:
//   num: the numerator
//   den: the denominator, should be non negative
// Fields:
//   num: the numerator
//   den: the denominator
// Functions:
//   reduce(): reduces the fraction to lowest terms with a positive denominator
//   +, +=, -, -=, *, *=, /, /=: standard arithmetic operators on fractions
//   <, <=, >, >=, ==, !=: comparison operators
//   >>, <<: input and output operators
// Time Complexity:
//   constructor, +, +=, -, -=, *, *=, /, /=, >>, <<: O(1)
//   reduce, <, <=, >, >=, ==, !=: O(log(max(num, den)))
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/bfs19p4
template <class T> struct Fraction {
  static_assert(is_integral<T>::value, "T must be an integral type");
  using F = Fraction<T>; T num, den;
  Fraction(T num = T(), T den = T(1)) : num(num), den(den) {}
  F reduce() const {
    T g = gcd(num, den);
    return den >= 0 ? F(num / g, den / g) : F(-num / g, -den / g);
  }
  F operator + (const F &f) const { return F(*this) += f; }
  F &operator += (const F &f) {
    num = (num * f.den + f.num * den); den = den * f.den; return *this;
  }
  F operator - (const F &f) const { return F(*this) -= f; }
  F &operator -= (const F &f) {
    num = (num * f.den - f.num * den); den = den * f.den; return *this;
  }
  F operator + () const { return *this; }
  F operator - () const { return F(-num, den); }
  F operator * (const F &f) const { return F(*this) *= f; }
  F &operator *= (const F &f) {
    num = num * f.num; den = den * f.den; return *this;
  }
  F operator / (const F &f) const { return F(*this) /= f; }
  F &operator /= (const F &f) {
    T t_num = num * f.den, t_den = den * f.num; num = t_num; den = t_den;
    return *this;
  }
  bool operator < (const F &f) const {
    F a = den >= 0 ? *this : F(-num, -den);
    F b = f.den >= 0 ? f : F(-f.num, -f.den);
    return a.num * b.den < b.num * a.den;
  }
  bool operator <= (const F &f) const { return !(f < *this); }
  bool operator > (const F &f) const { return f < *this; }
  bool operator >= (const F &f) const { return !(*this < f); }
  bool operator == (const F &f) const { return !(*this < f) && !(f < *this); }
  bool operator != (const F &f) const { return *this < f || f < *this; }
  friend istream &operator >> (istream &stream, F &f) {
    return stream >> f.num >> f.den;
  }
  friend ostream &operator << (ostream &stream, const F &f) {
    return stream << f.num << '/' << f.den;
  }
};

#define ftype long double
template <class T> Fraction<T> abs(Fraction<T> a) { return a >= 0 ? a : -a; }
#define FUN(name) \
  template <class T> ftype name(Fraction<T> a) { \
    return name((ftype)(a.num) / (ftype)(a.den)); \
  }
FUN(sqrt) FUN(sin) FUN(cos) FUN(tan) FUN(asin) FUN(acos) FUN(atan)
#undef FUN
template <class T> ftype atan2(Fraction<T> y, Fraction<T> x) {
  return atan2((ftype)(y.num) / (ftype)(y.den),
               (ftype)(x.num) / (ftype)(x.den));
}
#undef ftype
