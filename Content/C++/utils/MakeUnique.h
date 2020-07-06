#pragma once
#include <bits/stdc++.h>
using namespace std;

// With C++ 11 not having std::make_unique defined, a simple implementation
//   of make_unique is provided
// Template Arguments:
//   T: the type of the object to construct
//   ...Args: variadic arguments of the types of arguments being pased to the
//     object being constructor
// Return Value:
//   a unique_ptr of type T
// Tested:
//   https://dmoj.ca/problem/set
template <class T, class ...Args> unique_ptr<T> _make_unique(Args &&...args) {
  return unique_ptr<T>(new T(forward<Args>(args)...));
}

#define make_unique _make_unique
