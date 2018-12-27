#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

template <class T, class Comparator = less<T>> using ordered_set = tree<T, null_type, Comparator, rb_tree_tag, tree_order_statistics_node_update>;

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions
// Time Complexity:
//   add, rem, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, class T> struct SparseFenwickTree2D {
    ordered_set<pair<T, int>> BIT[MAXN]; int stamp = 0;
    void clear() { stamp = 0; for (int i = 0; i < MAXN; i++) BIT[i].clear(); }
    void add(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].insert(make_pair(y, stamp++)); }
    void rem(int x, int y) { for (; x < MAXN; x += x & -x) BIT[x].erase(BIT[x].lower_bound(make_pair(y, 0))); }
    T rsq(int x, int y) { T ret = 0; for (; x > 0; x -= x & -x) ret += BIT[x].order_of_key(make_pair(y, stamp)); return ret; }
    T rsq(int x1, int y1, int x2, int y2) { return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1); }
};
