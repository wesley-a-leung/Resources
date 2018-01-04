#ifndef ALGORITHMS_MATH_MO_H_
#define ALGORITHMS_MATH_MO_H_

#include <bits/stdc++.h>

using namespace std;

class Mo {
private:
    int n, m, sz, res = 0, maxVal = 0;
    int *cnt, *ans;

    struct Query {
        int l, r, ind;

        bool operator < (const Query &q) const {
            if (l / sz != q.l / sz) return l / sz < q.l / sz;
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
     * @param a the array (must contain only positive values)
     * @param N the number of elements in the array
     * @param oneIndexed whether the array is 1-indexed or not
     * @param queries the array of pairs containing the queries
     * @param Q the number of queries
     */
    Mo(int *a, int N, bool oneIndexed, pair<int, int> *queries, int Q) {
        sz = (int) sqrt(n);
        for (int i = 0; i < n; i++) {
            maxVal = max(maxVal, a[i + oneIndexed]);
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
        int l = q[0].l;
        int r = l - 1;
        for (int i = 0; i < Q; i++) {
            while (l < q[i].l) {
                remove(a[l]);
                l++;
            }
            while (l > q[i].l) {
                l--;
                update(a[l]);
            }
            while (r < q[i].r) {
                r++;
                update(a[r]);
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
