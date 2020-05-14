#pragma once
#include <bits/stdc++.h>
using namespace std;

// 32-bit and 64-bit random number generators using a time-based seed sequence
// Tested:
//   https://dmoj.ca/problem/set
//   https://dmoj.ca/problem/fallingsnowflakes
//   https://atcoder.jp/contests/agc026/tasks/agc026_c
seed_seq seq {
    uint64_t(chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now().time_since_epoch()).count()),
    uint64_t(__builtin_ia32_rdtsc()),
    uint64_t(uintptr_t(make_unique<char>().get()))
};

mt19937 rng(seq); mt19937_64 rng64(seq);
