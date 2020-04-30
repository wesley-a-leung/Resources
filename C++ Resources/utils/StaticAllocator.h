#pragma once
#include <bits/stdc++.h>
using namespace std;

constexpr const int MB = 200;
char buf[MB << 20];
size_t buf_ind = sizeof(buf);
void *operator new (size_t n) { return (void *) &buf[buf_ind -= n]; }
void *operator new[] (size_t n) { return (void *) &buf[buf_ind -= n]; }
void operator delete (void *) {}
void operator delete[] (void *) {}

template <class T> struct StaticAllocator {
    typedef T value_type; StaticAllocator() {}
    template <class U> StaticAllocator(const U &) {}
    T *allocate(size_t n) { (buf_ind -= n * sizeof(T)) &= 0 - alignof(T); return (T*) (buf + buf_ind); }
    void deallocate(T *, size_t) {}
};

template <class T> struct small_ptr {
    unsigned ind; small_ptr(T *p = 0) : ind(p ? unsigned((char *) p - buf) : 0) {}
    T &operator * () const { return *(T *) (buf + ind); }
    T *operator -> () const { return &**this; }
    T &operator [] (int a) const { return (&**this)[a]; }
    explicit operator bool () const { return ind; }
};
