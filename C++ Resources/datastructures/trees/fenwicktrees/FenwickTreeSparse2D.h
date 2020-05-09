#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "../../sqrt/SqrtBufferSimple.h"
#include "../../sqrt/SqrtBuffer.h"
using namespace std;
using namespace __gnu_pbds;

// A collection of sparse fenwick trees implemented in various methods. Some are only sparse in a single dimension,
// while others are sparse in both dimensions. Certain implementations only allow for increments and decrements of 1.
// In general, the offline fenwick trees are faster than the sqrt implementations, which are faster than the pbds implementations.

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions (sparse in 1 dimension)
// all update indices must be known beforehand
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(Q (log Q + log N) + N) for Q updates
//   update, rsq: O(log N log Q) for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/dmopc19c7p5
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType> struct FenwickTreeSemiSparse2DOffline {
    int N; vector<int> st, cnt; vector<IndexType> inds; vector<T> BIT;
    int getInd(int i, IndexType j) { return upper_bound(inds.begin() + st[i], inds.begin() + st[i] + cnt[i], j) - inds.begin() - st[i]; }
    FenwickTreeSemiSparse2DOffline(int N, vector<pair<int, IndexType>> updateInds) : N(N), st(N + 1, 0), cnt(N + 1, 0) {
        sort(updateInds.begin(), updateInds.end(), [&] (const pair<int, IndexType> &a, const pair<int, IndexType> &b) {
            return a.second < b.second;
        });
        vector<IndexType> last(N + 1, T());
        for (auto &&u : updateInds) for (int i = u.first + 1; i <= N; i += i & -i) if (cnt[i] == 0 || u.second != last[i]) {
            cnt[i]++;
            last[i] = u.second;
        }
        for (int i = 1; i <= N; i++) st[i] = st[i - 1] + cnt[i - 1];
        inds.resize(st[N] + cnt[N]); BIT.resize(st[N] + cnt[N]); fill(cnt.begin(), cnt.end(), 0);
        for (auto &&u : updateInds) for (int i = u.first + 1; i <= N; i += i & -i) {
            if (cnt[i] == 0 || u.second != inds[st[i] + cnt[i] - 1]) inds[st[i] + cnt[i]++] = u.second;
        }
    }
    void update(int i, IndexType j, T v) {
        for (i++; i <= N; i += i & -i) for (int s = st[i], c = cnt[i], y = getInd(i, j); y <= c; y += y & -y) BIT[s + y - 1] += v;
    }
    T rsq(int d, IndexType r) {
        T ret = T();
        for (d++; d > 0; d -= d & -d) for (int s = st[d], y = getInd(d, r); y > 0; y -= y & -y) ret += BIT[s + y - 1];
        return ret;
    }
    T rsq(int d, IndexType l, IndexType r) { return rsq(d, r) - rsq(d, l - 1); }
    T rsq(int u, int d, IndexType l, IndexType r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions (sparse in 2 dimensions)
// all update indices must be known beforehand
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(Q log Q) for Q updates
//   update, rsq: O((log Q)^2) for Q updates
// Memory Complexity: O(Q log Q) for Q updates
// Tested:
//   https://dmoj.ca/problem/dmopc19c7p5
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType1, class IndexType2> struct FenwickTreeSparse2DOffline {
    int U; vector<int> st, cnt; vector<IndexType1> inds1; vector<IndexType2> inds2; vector<T> BIT;
    int getInd1(IndexType1 i) { return upper_bound(inds1.begin(), inds1.end(), i) - inds1.begin(); }
    int getInd2(int i, IndexType2 j) { return upper_bound(inds2.begin() + st[i], inds2.begin() + st[i] + cnt[i], j) - inds2.begin() - st[i]; }
    FenwickTreeSparse2DOffline(vector<pair<IndexType1, IndexType2>> updateInds) : inds1(updateInds.size()) {
        sort(updateInds.begin(), updateInds.end(), [&] (const pair<IndexType1, IndexType2> &a, const pair<IndexType1, IndexType2> &b) {
            return a.second < b.second;
        });
        for (int i = 0; i < int(updateInds.size()); i++) inds1[i] = updateInds[i].first;
        sort(inds1.begin(), inds1.end()); inds1.erase(unique(inds1.begin(), inds1.end()), inds1.end());
        U = int(inds1.size()); st.assign(U + 1, 0); cnt.assign(U + 1, 0); vector<IndexType2> last(U + 1, T());
        for (auto &&u : updateInds) for (int i = getInd1(u.first); i <= U; i += i & -i) if (cnt[i] == 0 || u.second != last[i]) {
            cnt[i]++;
            last[i] = u.second;
        }
        for (int i = 1; i <= U; i++) st[i] = st[i - 1] + cnt[i - 1];
        inds2.resize(st[U] + cnt[U]); BIT.resize(st[U] + cnt[U]); fill(cnt.begin(), cnt.end(), 0);
        for (auto &&u : updateInds) for (int i = getInd1(u.first); i <= U; i += i & -i) {
            if (cnt[i] == 0 || u.second != inds2[st[i] + cnt[i] - 1]) inds2[st[i] + cnt[i]++] = u.second;
        }
    }
    void update(IndexType1 i, IndexType2 j, T v) {
        for (int x = getInd1(i); x <= U; x += x & -x) for (int s = st[x], c = cnt[x], y = getInd2(x, j); y <= c; y += y & -y) BIT[s + y - 1] += v;
    }
    T rsq(IndexType1 d, IndexType2 r) {
        T ret = T();
        for (int x = getInd1(d); x > 0; x -= x & -x) for (int s = st[x], y = getInd2(x, r); y > 0; y -= y & -y) ret += BIT[s + y - 1];
        return ret;
    }
    T rsq(IndexType1 d, IndexType2 l, IndexType2 r) { return rsq(d, r) - rsq(d, l - 1); }
    T rsq(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions
// using SqrtBufferSimple (sparse in 1 dimension)
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   add, rem: O(log N) amortized
//   rsq: O(log N sqrt Q) amortized for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/dmopc19c7p5
template <class IndexType> struct FenwickTreeSemiSparse2DSimpleSqrt {
    int N; vector<SqrtBufferSimple<IndexType>> IN, OUT;
    FenwickTreeSemiSparse2DSimpleSqrt(int N, const double SCALE_FACTOR = 1) : N(N),
        IN(N + 1, SqrtBufferSimple<IndexType>(SCALE_FACTOR)), OUT(N + 1, SqrtBufferSimple<IndexType>(SCALE_FACTOR)) {}
    void add(int i, IndexType j) { for (i++; i <= N; i += i & -i) IN[i].insert(j); }
    void rem(int i, IndexType j) { for (i++; i <= N; i += i & -i) OUT[i].insert(j); }
    int rsq(int d, IndexType r) { int ret = 0; for (d++; d > 0; d -= d & -d) ret += IN[d].aboveInd(r) - OUT[r].aboveInd(r); return ret; }
    int rsq(int d, IndexType l, IndexType r) { int ret = 0; for (d++; d > 0; d -= d & -d) ret += IN[d].count(l, r) - OUT[d].count(l, r); return ret; }
    int rsq(int u, int d, IndexType l, IndexType r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions
// using SqrtBuffer (sparse in 1 dimension)
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   update: O(log N) amortized
//   rsq: O(log N sqrt Q) amortized for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/apio19p3
//   https://dmoj.ca/problem/ioi01p1
template <class T, class IndexType> struct FenwickTreeSemiSparse2DSqrt {
    int N; vector<SqrtBuffer<T, IndexType>> BIT;
    FenwickTreeSemiSparse2DSqrt(int N, const double SCALE_FACTOR = 1) : N(N), BIT(N + 1, SqrtBuffer<T, IndexType>(SCALE_FACTOR)) {}
    void update(int i, IndexType j, T v) { for (i++; i <= N; i += i & -i) BIT[i].emplace(j, v); }
    T rsq(int d, IndexType r) { T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].aboveInd(r); return ret; }
    T rsq(int d, IndexType l, IndexType r) { T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].count(l, r); return ret; }
    T rsq(int u, int d, IndexType l, IndexType r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937_64 rng64(seq);const std::size_t RANDOM=std::uniform_int_distribution<std::size_t>(0,std::numeric_limits<std::size_t>::max())(rng64);
template<class T,class H=std::hash<T>>struct rand_hash{
    static uint64_t splitmix64(uint64_t x){x+=0x9e3779b97f4a7c15;x=(x^(x>>30))*0xbf58476d1ce4e5b9;x=(x^(x>>27))*0x94d049bb133111eb;return x^(x>>31);}
    std::size_t operator()(const T&x)const{return splitmix64(H{}(x)+RANDOM);}
};
template<class K,class V,class H=rand_hash<K>,class...Ts>using hashmap=__gnu_pbds::gp_hash_table<K,V,H,Ts...>;
template<class K,class C=std::less<K>,class...Ts>using treeset=__gnu_pbds::tree<K,__gnu_pbds::null_type,C,__gnu_pbds::rb_tree_tag,__gnu_pbds::tree_order_statistics_node_update,Ts...>;
template<class T1,class T2,class H1=rand_hash<T1>,class H2=rand_hash<T2>>struct pair_hash{
    std::size_t operator()(const std::pair<T1,T2>&p)const{return 31*H1{}(p.first)+H2{}(p.second);}
};

// Sparse Fenwick Tree supporting point updates (with value 1) and range queries in 2 dimensions using pbds tree (sparse in 1 dimension)
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   add, rem, rsq: O(log N log Q) for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
template <class IndexType> struct FenwickTreeSemiSparse2DSimpleTreeset {
    int N, stamp; vector<treeset<pair<IndexType, int>>> BIT;
    FenwickTreeSemiSparse2DSimpleTreeset(int N) : N(N), stamp(0), BIT(N + 1, treeset<pair<IndexType, int>>()) {}
    void add(int i, IndexType j) { for (i++; i <= N; i += i & -i) BIT[i].insert(make_pair(j, stamp++)); }
    void rem(int i, IndexType j) { for (i++; i <= N; i += i & -i) BIT[i].erase(BIT[i].lower_bound(make_pair(j, 0))); }
    int rsq(int d, IndexType r) { int ret = 0; for (d++; d > 0; d -= d & -d) ret += BIT[d].order_of_key(make_pair(r, stamp)); return ret; }
    int rsq(int d, IndexType l, IndexType r) { return rsq(d, r) - rsq(d, l - 1); }
    int rsq(int u, int d, IndexType l, IndexType r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions using pbds hash_table (sparse in 1 dimension)
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N log M) on avareage
// Memory Complexity: O(N + Q log N log M) for Q updates
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType, class Container = hashmap<IndexType, T>> struct FenwickTreeSemiSparse2D {
    int N, M; vector<Container> BIT;
    FenwickTreeSemiSparse2D(int N, int M) : N(N), M(M), BIT(N + 1) {}
    void update(int i, IndexType j, T v) {
        i++; j++;
        for (int x = i; x <= N; x += x & -x) for (IndexType y = j; y <= M; y += y & -y) {
            auto it = BIT[x].find(y);
            if (it == BIT[x].end()) BIT[x][y] += v;
            else if ((it->second += v) == T()) BIT[x].erase(it->first);
        }
    }
    T rsq(int d, IndexType r) {
        d++; r++; T ret = T();
        for (int x = d; x > 0; x -= x & -x) for (IndexType y = r; y > 0; y -= y & -y) {
            auto it = BIT[x].find(y);
            if (it != BIT[x].end()) ret += it->second;
        }
        return ret;
    }
    T rsq(int d, IndexType l, IndexType r) { return rsq(d, r) - rsq(d, l - 1); }
    T rsq(int u, int d, IndexType l, IndexType r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};

// Sparse Fenwick Tree supporting point updates (with any value) and range queries in 2 dimensions using pbds hash_table (sparse in 2 dimensions)
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(1)
//   update, rsq: O(log N log M) on average
// Memory Complexity: O(Q log N log M) for Q updates
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType1, class IndexType2, class Container = hashmap<pair<IndexType1, IndexType2>, T, pair_hash<IndexType1, IndexType2>>>
        struct FenwickTreeSparse2D {
    int N, M; Container BIT;
    FenwickTreeSparse2D(int N, int M) : N(N), M(M) {}
    void update(IndexType1 i, IndexType2 j, T v) {
        i++; j++;
        for (IndexType1 x = i; x <= N; x += x & -x) for (IndexType2 y = j; y <= M; y += y & -y) {
            auto it = BIT.find(make_pair(x, y));
            if (it == BIT.end()) BIT[make_pair(x, y)] += v;
            else if ((it->second += v) == T()) BIT.erase(it->first);
        }
    }
    T rsq(IndexType1 d, IndexType2 r) {
        d++; r++; T ret = T();
        for (IndexType1 x = d; x > 0; x -= x & -x) for (IndexType2 y = r; y > 0; y -= y & -y) {
            auto it = BIT.find(make_pair(x, y));
            if (it != BIT.end()) ret += it->second;
        }
        return ret;
    }
    T rsq(IndexType1 d, IndexType2 l, IndexType2 r) { return rsq(d, r) - rsq(d, l - 1); }
    T rsq(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) { return rsq(d, l, r) - rsq(u - 1, l, r); }
};
