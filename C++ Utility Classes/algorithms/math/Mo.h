#ifndef ALGORITHMS_MATH_MO_H_
#define ALGORITHMS_MATH_MO_H_

#include <bits/stdc++.h>

using namespace std;

class Mo {
private:
    int n, m, sz, res, maxVal = 0;
    int *cnt, *a, *ans;

    struct Query {
        int l, r, ind;

        bool operator < (const Query &q) const {
            if ((l - 1) / sz != (q.l - 1) / sz) return (l - 1) / sz < (q.l - 1) / sz;
            return r < q.r;
        }
    } *q;

    void update(int i) {
        cnt[i]++;
        if (cnt[i] == 1) res++;
    }

    void remove(int i) {
        cnt[i]--;
        if (cnt[i] == 0) res--;
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
     * Computes the number of unique numbers, provided the queries are known beforehand.
     *
     * @param arr the array (must contain only positive values)
     * @param N the number of elements in the array
     * @param queries the array of pairs containing the queries
     * @param Q the number of queries
     */
    Mo(int *arr, int N, pair<int, int> *queries, int Q) {
        a = new int[N];
        for (int i = 1; i <= n; i++) {
            a[i] = arr[i - 1];
            maxVal = max(maxVal, arr[i - 1]);
        }
        q = new Query[Q];
        for (int i = 0; i < Q; i++) {
            q[i].l = queries[i].first;
            q[i].r = queries[i].second;
            q[i].ind = i;
        }
        sort(q, q + Q);
        cnt = new int[maxVal + 1];
        ans = new int[Q];
        int l = 1, r = 0;
        for (int i = 0; i < Q; i++) {
            while (r > q[i].r) {
                remove(a[r]);
                r--;
            }
            while (r < q[i].r) {
                r++;
                update(a[r]);
            }
            while (l < q[i].l) {
                remove(a[l]);
                l++;
            }
            while (l > q[i].l) {
                l--;
                update(a[l]);
            }
            ans[q[i].ind] = res;
        }
    }
};

#endif /* ALGORITHMS_MATH_MO_H_ */
