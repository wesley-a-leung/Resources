#ifndef DATASTRUCTURES_MATH_WEIGHTEDUF_H_
#define DATASTRUCTURES_MATH_WEIGHTEDUF_H_

#include <bits/stdc++.h>
using namespace std;

struct WeightedUF {
private:
    int *parent;
    int *size;
    int count;
    vector<pair<pair<int, int>, int>> history;

public:
    /**
     * Initializes an empty union-find data structure with {@code n} sites
     * {@code 0} through {@code n-1}. Each site is initially in its own
     * component.
     *
     * @param  n the number of sites
     */
    WeightedUF(int n) {
        count = n;
        parent = new int[n];
        size = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
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
     * Returns the size of the component containing p
     *
     * @param p the integer representing one site
     * @return the size of the component containing p
     */
    int getSize(int p) {
        return size[find(p)];
    }

    /**
     * Merges the component containing site {@code p} with the
     * the component containing site {@code q}.
     *
     * @param  p the integer representing one site
     * @param  q the integer representing the other site
     * @return true if the components were merged, false otherwise
     */
    bool join(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return false;
        if (size[rootP] > size[rootQ]) swap(rootP, rootQ);
        history.push_back(make_pair(make_pair(rootP, rootQ), parent[rootP]));
        // make smaller root point to larger one
        parent[rootP] = rootQ;
        size[rootQ] += size[rootP];
        count--;
        return true;
    }

    /**
     * Undos the last merge, returns false if there is no last merge,
     * true otherwise.
     */
    bool undo() {
        if ((int) history.size() == 0) return false;
        int rootP = history.back().first.first;
        int rootQ = history.back().first.second;
        int parP = history.back().second;
        history.pop_back();
        parent[rootP] = parP;
        size[rootQ] -= size[rootP];
        count++;
        return true;
    }
};

#endif /* DATASTRUCTURES_MATH_WEIGHTEDUF_H_ */
