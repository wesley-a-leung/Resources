#ifndef DATASTRUCTURES_RMQSPARSETABLE_H_
#define DATASTRUCTURES_RMQSPARSETABLE_H_

#include <bits/stdc++.h>
using namespace std;

struct RMQSparseTable {
private:
    int n, lg;
    int *a;
    int **rmq;

public:
    RMQSparseTable(int *arr, int N) {
        n = N;
        lg = 32 - __builtin_clz(n);
        a = new int[n + 1];
        rmq = new int*[lg];
        for (int i = 0; i < lg; i++) rmq[i] = new int[n];
        for (int i = 0; i < n; i++) rmq[0][i] = i;
        for (int i = 0; i < lg - 1; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                int x = rmq[i][j];
                int y = rmq[i][j + (1 << i)];
                rmq[i + 1][j] = a[x] <= a[y] ? x : y;
            }
        }
    }

    /**
     * Returns the index of the minimum element between indices l and r inclusive.
     * Ties are broken by the the index.
     *
     * @param l the 0-indexed left bound
     * @param r the 0-indexed right bound
     * @return the index of the minimum element between indices l and r inclusive
     */
    int minPos(int l, int r) {
        int i = 31 - __builtin_clz(r - l + 1);;
        int x = rmq[i][l];
        int y = rmq[i][r - (1 << i) + 1];
        return a[x] <= a[y] ? x : y;
    }
};

#endif /* DATASTRUCTURES_RMQSPARSETABLE_H_ */
