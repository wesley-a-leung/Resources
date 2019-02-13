#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "../SqrtOrderMaintenance.h"
using namespace std;
using namespace __gnu_pbds;

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq);const size_t RANDOM=uniform_int_distribution<size_t>(0,(numeric_limits<size_t>::max)())(rng64);
template<class T,class H=hash<T>>struct rand_hash{
    static uint64_t splitmix64(uint64_t x){x+=0x9e3779b97f4a7c15;x=(x^(x>>30))*0xbf58476d1ce4e5b9;x=(x^(x>>27))*0x94d049bb133111eb;return x^(x>>31);}
    size_t operator()(const T&x)const{return splitmix64(H{}(x)+RANDOM);}
};
template<class K,class V,class H=rand_hash<K>,class...Ts>using hashmap=gp_hash_table<K,V,H,Ts...>;
template<class K,class C=less<K>,class...Ts>using treeset=tree<K,null_type,C,rb_tree_tag,tree_order_statistics_node_update,Ts...>;

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions using pbds tree
// Time Complexity:
//   add, rem, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, class T> struct SparseFenwickTree2DSimple {
    treeset<pair<T, int>> BIT[MAXN]; int stamp = 0;
    void clear() { stamp = 0; for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void add(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].insert(make_pair(y, stamp++)); }
    void rem(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].erase(BIT[x].lower_bound(make_pair(y, 0))); }
    T rsq(int x, int y) { T ret = 0; for (; x > 0; x -= x & -x) ret += BIT[x].order_of_key(make_pair(y, stamp)); return ret; }
    T rsq(int x1, int y1, int x2, int y2) { return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1); }
};

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions using sqrt order maintenance
// Time Complexity:
//   add, rem: O(log N) amortized
//   rsq: O(log N (log M + sqrt M)) amortized
// Memory Complexity: O(NM)
template <const int MAXN, class T, class Tree = SqrtOrderMaintenance<T>> struct SparseFenwickTree2DSqrt {
    Tree IN[MAXN], OUT[MAXN];
    void init(const double SCALE_FACTOR = 1) { for (int i = 0; i < MAXN; i++) { IN[i] = Tree(SCALE_FACTOR); OUT[i] = Tree(SCALE_FACTOR); } }
    void clear() { for (int i = 0; i < MAXN; i++) { IN[i].clear(); OUT[i].clear(); } }
    void add(int x, int y) { for (; x < MAXN; x += x & -x) IN[x].insert(y); }
    void rem(int x, int y) { for (; x < MAXN; x += x & -x) OUT[x].insert(y); }
    T rsq(int x, int y) { T ret = 0; for (; x > 0; x -= x & -x) ret += IN[x].aboveInd(y) - OUT[x].aboveInd(y); return ret; }
    T rsq(int x1, int y1, int x2, int y2) { return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions
// Time Complexity:
//   update, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, class T> struct SparseFenwickTree2D {
    hashmap<int, int> BIT[MAXN];
    void clear() { for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void update(int x, int y, T v) {
        for (int i = x; i < MAXN; i += i & -i) for (int j = y; j < MAXN; j += j & -j) {
            auto it = BIT[i].find(j);
            if (it == BIT[i].end()) {
                BIT[i][j] += v;
            } else {
                it->second += v;
                if (it->second == 0) BIT[i].erase(it->first);
            }
        }
    }
    T rsq(int x, int y) {
        T ret = 0;
        for (int i = x; i > 0; i -= i & -i) for (int j = y; j > 0; j -= j & -j) {
            auto it = BIT[i].find(j);
            if (it != BIT[i].end()) ret += it->second;
        }
        return ret;
    }
    T rsq(int x1, int y1, int x2, int y2) {
        return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1);
    }
};
