#ifndef ALGORITHMS_MATH_MO_H_
#define ALGORITHMS_MATH_MO_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Mo's Algorithm
 *
 * Decomposes the array into blocks of size sqrt(N) to answer queries.
 *
 * Time complexity: Q log Q + Q sqrt(N)
 */
class Mo {
private:
    int res;
    int *a, *cnt, *ans;

    struct Query {
        int l, r, ind, block;

        bool operator < (const Query &q) const {
            if (block != q.block) return block < q.block;
            return r < q.r;
        }
    } *q;

    inline void add(int a) {
        cnt[a]++;
        if (cnt[a] == 1) res++;
    }

    inline void remove(int a) {
        cnt[a]--;
        if (cnt[a] == 0) res--;
    }

public:
    /**
     * Returns the answer to the ith query.
     *
     * @param i the index of the query
     * @return the answer the query
     */
    int query(int i) {
        return ans[i];
    }

    /**
     * Computes the number of unique values between l and r inclusive, provided the
     * queries are known beforehand.
     *
     * @param a the array (must contain only positive values)
     * @param N the number of elements in the array
     * @param oneIndexed whether the array is 1-indexed or not
     * @param queries the array of pairs containing the queries
     * @param Q the number of queries
     */
    Mo(int *arr, int N, bool oneIndexed, pair<int, int> *queries, int Q) {
        int sz = (int) sqrt(N);
        a = new int[N + oneIndexed];
        cnt = new int[N];
        int temp[N + oneIndexed];
        for (int i = oneIndexed; i < N + oneIndexed; i++) {
            temp[i] = arr[i];
        }
        sort(temp + oneIndexed, temp + N + oneIndexed);
        int k = unique(temp + oneIndexed, temp + N + oneIndexed) - temp;
        for (int i = 0; i < N; i++) {
            a[i + oneIndexed] = lower_bound(temp + oneIndexed, temp + k + oneIndexed, arr[i + oneIndexed]) - temp - oneIndexed;
        }
        q = new Query[Q];
        for (int i = 0; i < Q; i++) {
            q[i].l = queries[i].first;
            q[i].r = queries[i].second;
            q[i].ind = i;
            q[i].block = q[i].l / sz;
        }
        sort(q, q + Q);
        ans = new int[Q];
        int l = q[0].l;
        int r = l - 1;
        res = 0;
        for (int i = 0; i < Q; i++) {
            while (l < q[i].l) {
                remove(a[l]);
                l++;
            }
            while (l > q[i].l) {
                l--;
                add(a[l]);
            }
            while (r < q[i].r) {
                r++;
                add(a[r]);
            }
            while (r > q[i].r) {
                remove(a[r]);
                r--;
            }
            ans[q[i].ind] = res;
        }
    }
};

#endif /* ALGORITHMS_MATH_MO_H_ */
