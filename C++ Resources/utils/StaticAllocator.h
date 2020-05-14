#pragma once
#include <bits/stdc++.h>
using namespace std;

// Static allocator to improve speed of memory access
// Tested:
//   https://dmoj.ca/problem/wac4p5
constexpr const int MB = 200;
char buf[MB << 20]; size_t buf_ind = sizeof(buf);

// overloading new and delete operators
void *operator new (size_t n) { return (void *) &buf[buf_ind -= n]; }
void *operator new[] (size_t n) { return (void *) &buf[buf_ind -= n]; }
void operator delete (void *) {}
void operator delete[] (void *) {}

// Allocator class to be used with stl data structures
template <class T> struct StaticAllocator {
    typedef T value_type; StaticAllocator() {}
    template <class U> StaticAllocator(const U &) {}
    T *allocate(size_t n) { (buf_ind -= n * sizeof(T)) &= 0 - alignof(T); return (T*) (buf + buf_ind); }
    void deallocate(T *, size_t) {}
};

// 32-bit pointer
template <class T> struct small_ptr {
    unsigned ind; small_ptr(T *p = 0) : ind(p ? unsigned((char *) p - buf) : 0) {}
    T &operator * () const { return *(T *) (buf + ind); }
    T *operator -> () const { return &**this; }
    T &operator [] (int a) const { return (&**this)[a]; }
    explicit operator bool () const { return ind; }
};
