/*
 * UF.h
 *
 *  Created on: May 27, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_UF_H_
#define DATASTRUCTURES_MATH_UF_H_

#include <bits/stdc++.h>;

using namespace std;

struct UF {
private:
    int* parent;
    int* rank;
    int count;

public:
    /**
     * Initializes an empty union–find data structure with {@code n} sites
     * {@code 0} through {@code n-1}. Each site is initially in its own
     * component.
     *
     * @param  n the number of sites
     */
    UF(int n) {
        count = n;
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    /**
     * Returns the component identifier for the component containing site {@code p}.
     *
     * @param  p the integer representing one site
     * @return the component identifier for the component containing site {@code p}
     */
    int find(int p) {
        while (p != parent[p]) {
            parent[p] = parent[parent[p]];    // path compression by halving
            p = parent[p];
        }
        return p;
    }

    /**
     * Returns the number of components.
     *
     * @return the number of components (between {@code 1} and {@code n})
     */
    int getCount() {
        return count;
    }

    /**
     * Returns true if the the two sites are in the same component.
     *
     * @param  p the integer representing one site
     * @param  q the integer representing the other site
     * @return {@code true} if the two sites {@code p} and {@code q} are in the same component;
     *         {@code false} otherwise
     */
    bool connected(int p, int q) {
        return find(p) == find(q);
    }

    /**
     * Returns the rank by size of the component containing p
     *
     * @param p the integer representing one site
     * @return the rank by size of the component containing p
     */
    int getRank(int p) {
        return rank[find(p)];
    }

    /**
     * Merges the component containing site {@code p} with the
     * the component containing site {@code q}.
     *
     * @param  p the integer representing one site
     * @param  q the integer representing the other site
     */
    void join(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;

        // make root of smaller rank point to root of larger rank
        if      (rank[rootP] < rank[rootQ]) parent[rootP] = rootQ;
        else if (rank[rootP] > rank[rootQ]) parent[rootQ] = rootP;
        else {
            parent[rootQ] = rootP;
            rank[rootP]++;
        }
        count--;
    }

    /**
     * Unmerges the component containing site {@code p} with the
     * the component containing site {@code q}.
     *
     * @param  p the integer representing one site
     * @param  q the integer representing the other site
     */
    void disjoin(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if      (rank[rootP] < rank[rootQ]) parent[rootP] = rootP;
        else if (rank[rootP] > rank[rootQ]) parent[rootQ] = rootQ;
        else {
            parent[rootQ] = rootQ;
            rank[rootP]--;
        }
        count++;
    }
};

#endif /* DATASTRUCTURES_MATH_UF_H_ */
