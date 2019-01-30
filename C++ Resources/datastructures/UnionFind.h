#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find / Disjoint Sets with Half Compression
// Time Complexity:
//   init: O(N)
//   find, join, connected, getSize: Inverse Ackerman
// Memory Complexity: O(N)
template <const int MAXN, const bool ONE_INDEXED> struct UnionFind {
    int UF[MAXN], cnt;
    void init(int N) { cnt = N; fill(UF, UF + N + ONE_INDEXED, -1); }
    int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
    bool join(int v, int w) {
        if ((v = find(v)) == (w = find(w))) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w]; UF[w] = v; cnt--; return true;
    }
    bool connected(int v, int w) { return find(v) == find(w); }
    int getSize(int v) { return -UF[find(v)]; }
};
