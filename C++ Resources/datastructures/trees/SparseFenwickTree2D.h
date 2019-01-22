#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

template <class K, class V, class H = hash<K>, class E = equal_to<K>> using hash_map = gp_hash_table<K, V, H, E>;
template <class T, class Comparator = less<T>> using ordered_set = tree<T, null_type, Comparator, rb_tree_tag, tree_order_statistics_node_update>;

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions
// Time Complexity:
//   add, rem, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, class T> struct SparseFenwickTree2DSimple {
    ordered_set<pair<T, int>> BIT[MAXN]; int stamp = 0;
    void clear() { stamp = 0; for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void add(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].insert(make_pair(y, stamp++)); }
    void rem(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].erase(BIT[x].lower_bound(make_pair(y, 0))); }
    T rsq(int x, int y) { T ret = 0; for (; x > 0; x -= x & -x) ret += BIT[x].order_of_key(make_pair(y, stamp)); return ret; }
    T rsq(int x1, int y1, int x2, int y2) { return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1); }
};

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq);

const size_t RANDOM = uniform_int_distribution<size_t>(0, (numeric_limits<size_t>::max)())(rng64);

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions
// Time Complexity:
//   update, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, class T> struct SparseFenwickTree2D {
    struct rand_hash { size_t operator ()(T x) const { return x ^ RANDOM; } };
    hash_map<int, int, rand_hash> BIT[MAXN];
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
