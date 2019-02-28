#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "../SqrtOrderMaintenanceSimple.h"
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
template<class T1,class T2,class H1=rand_hash<T1>,class H2=rand_hash<T2>>struct pair_hash{
    size_t operator()(const pair<T1,T2>&p)const{return 31*H1{}(p.first)+H2{}(p.second);}
};

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions using pbds tree (sparse in 1 dimension)
// Time Complexity:
//   add, rem, rsq: O(log N log M)
// Memory Complexity: O(N + Q log M) for Q updates
template <class IndexType, const int MAXN, const bool ONE_INDEXED, class Tree = treeset<pair<IndexType, int>>> struct SemiSparseFenwickTree2DSimpleTreeset {
    Tree BIT[MAXN]; int stamp = 0;
    void clear() { stamp = 0; for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void add(int x, IndexType y) { for (x += !ONE_INDEXED; x < MAXN; x += x & -x) BIT[x].insert(make_pair(y, stamp++)); }
    void rem(int x, IndexType y) { for (x += !ONE_INDEXED; x < MAXN; x += x & -x) BIT[x].erase(BIT[x].lower_bound(make_pair(y, 0))); }
    int rsq(int x, IndexType y) { int ret = 0; for (x += !ONE_INDEXED; x > 0; x -= x & -x) ret += BIT[x].order_of_key(make_pair(y, stamp)); return ret; }
    int rsq(int x, IndexType y1, IndexType y2) { return rsq(x, y2) - rsq(x, y1 - 1); }
    int rsq(int x1, IndexType y1, int x2, IndexType y2) { return rsq(x2, y1, y2) - rsq(x1 - 1, y1, y2); }
};

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions
// using sqrt order maintenance (sparse in 1 dimension)
// Time Complexity:
//   add, rem: O(log N) amortized
//   rsq: O(log N (log M + sqrt M)) amortized
// Memory Complexity: O(N + Q log M) for Q updates
template <class IndexType, const int MAXN, const bool ONE_INDEXED, class Tree = SqrtOrderMaintenanceSimple<IndexType>> struct SemiSparseFenwickTree2DSimpleSqrt {
    Tree IN[MAXN], OUT[MAXN];
    void init(const double SCALE_FACTOR = 1) { for (int i = 0; i < MAXN; i++) { IN[i] = Tree(SCALE_FACTOR); OUT[i] = Tree(SCALE_FACTOR); } }
    void clear() { for (int i = 0; i < MAXN; i++) { IN[i].clear(); OUT[i].clear(); } }
    void add(int x, IndexType y) { for (x += !ONE_INDEXED; x < MAXN; x += x & -x) IN[x].insert(y); }
    void rem(int x, IndexType y) { for (x += !ONE_INDEXED; x < MAXN; x += x & -x) OUT[x].insert(y); }
    int rsq(int x, IndexType y) { int ret = 0; for (x += !ONE_INDEXED; x > 0; x -= x & -x) ret += IN[x].aboveInd(y) - OUT[x].aboveInd(y); return ret; }
    int rsq(int x, IndexType y1, IndexType y2) {
        int ret = 0;
        for (x += !ONE_INDEXED; x > 0; x -= x & -x) ret += IN[x].count(y1, y2) - OUT[x].count(y1, y2);
        return ret;
    }
    int rsq(int x1, IndexType y1, int x2, IndexType y2) { return rsq(x2, y1, y2) - rsq(x1 - 1, y1, y2); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions
// using sqrt order maintenance (sparse in 1 dimension)
// Time Complexity:
//   update: O(log N) amortized
//   rsq: O(log N (log M + sqrt M)) amortized
// Memory Complexity: O(N + Q log M) for Q updates
template <class T, class IndexType, const int MAXN, const bool ONE_INDEXED, class Tree = SqrtOrderMaintenance<IndexType, T>> struct SemiSparseFenwickTree2DSqrt {
    Tree BIT[MAXN];
    void init(const double SCALE_FACTOR = 1) { for (int i = 0; i < MAXN; i++) BIT[i] = Tree(SCALE_FACTOR); }
    void clear() { for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void update(int x, IndexType y, T v) { for (x += !ONE_INDEXED; x < MAXN; x += x & -x) BIT[x].emplace(y, v); }
    T rsq(int x, IndexType y) { T ret = 0; for (x += !ONE_INDEXED; x > 0; x -= x & -x) ret += BIT[x].aboveInd(y); return ret; }
    T rsq(int x, IndexType y1, IndexType y2) { T ret = 0; for (x += !ONE_INDEXED; x > 0; x -= x & -x) ret += BIT[x].count(y1, y2); return ret; }
    T rsq(int x1, IndexType y1, int x2, IndexType y2) { return rsq(x2, y1, y2) - rsq(x1 - 1, y1, y2); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions (sparse in 1 dimension)
// Time Complexity:
//   update, rsq: O(log N log M)
// Memory Complexity: O(N + Q log M) for Q updates
template <class T, class IndexType, const int MAXN, const IndexType MAXM, const bool ONE_INDEXED,
        class Container = hashmap<IndexType, T>> struct SemiSparseFenwickTree2D {
    Container BIT[MAXN];
    void clear() { for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void update(int x, IndexType y, T v) {
        for (int i = x + !ONE_INDEXED; i < MAXN; i += i & -i) for (IndexType j = y + !ONE_INDEXED; j < MAXM; j += j & -j) {
            auto it = BIT[i].find(j);
            if (it == BIT[i].end()) BIT[i][j] += v;
            else if ((it->second += v) == 0) BIT[i].erase(it->first);
        }
    }
    T rsq(int x, IndexType y) {
        T ret = 0;
        for (int i = x + !ONE_INDEXED; i > 0; i -= i & -i) for (IndexType j = y + !ONE_INDEXED; j > 0; j -= j & -j) {
            auto it = BIT[i].find(j);
            if (it != BIT[i].end()) ret += it->second;
        }
        return ret;
    }
    T rsq(int x, IndexType y1, IndexType y2) { return rsq(x, y2) - rsq(x, y1 - 1); }
    T rsq(int x1, IndexType y1, int x2, IndexType y2) { return rsq(x2, y1, y2) - rsq(x1 - 1, y1, y2); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions (sparse in 2 dimension)
// Time Complexity:
//   update, rsq: O(log N log M)
// Memory Complexity: O(Q log N loq M) for Q updates
template <class T, class IndexType1, class IndexType2, const IndexType1 MAXN, const IndexType2 MAXM, const bool ONE_INDEXED,
        class Container = hashmap<pair<IndexType1, IndexType2>, T, pair_hash<IndexType1, IndexType2>>> struct SparseFenwickTree2D {
    Container BIT;
    void clear() { BIT.clear(); }
    void update(IndexType1 x, IndexType2 y, T v) {
        for (IndexType1 i = x + !ONE_INDEXED; i < MAXN; i += i & -i) for (IndexType2 j = y + !ONE_INDEXED; j < MAXN; j += j & -j) {
            auto it = BIT.find({i, j});
            if (it == BIT.end()) BIT[{i, j}] += v;
            else if ((it->second += v) == 0) BIT.erase(it->first);
        }
    }
    T rsq(IndexType1 x, IndexType2 y) {
        T ret = 0;
        for (IndexType1 i = x + !ONE_INDEXED; i > 0; i -= i & -i) for (IndexType2 j = y + !ONE_INDEXED; j > 0; j -= j & -j) {
            auto it = BIT.find({i, j});
            if (it != BIT.end()) ret += it->second;
        }
        return ret;
    }
    T rsq(IndexType1 x1, IndexType2 y1, IndexType1 x2, IndexType2 y2) { return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1); }
};
