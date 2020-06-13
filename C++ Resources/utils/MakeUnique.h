#pragma once
#include <bits/stdc++.h>
using namespace std;

// With C++ 11 not having std::make_unique defined, a simple implementation
//   of std::make_shared is provided
// Tested:
//   https://dmoj.ca/problem/set
template <class T, class ...Args> unique_ptr<T> _make_unique(Args &&...args) {
  return unique_ptr<T>(new T(forward<Args>(args)...));
}

#define make_unique _make_unique
