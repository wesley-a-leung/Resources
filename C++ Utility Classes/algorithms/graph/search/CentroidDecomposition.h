#ifndef ALGORITHMS_GRAPH_SEARCH_CENTROIDDECOMPOSITION_H_
#define ALGORITHMS_GRAPH_SEARCH_CENTROIDDECOMPOSITION_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"

using namespace std;

class CentroidDecomposition {
private:
    Graph *G;
    bool *exclude;

    int getSize(int v, int prev) {
        int size = 1;
        for (int w : G->adj(v)) {
            if (w != prev && !exclude[w]) size += getSize(w, v);
        }
        return size;
    }

    int getCentroid(int v, int prev, int treeSize) {
        int n = treeSize;
        int size = 1;
        bool hasCentroid = true;
        for (int w : G->adj(v)) {
            if (w == prev || exclude[w]) continue;
            int ret = getCentroid(w, v, treeSize);
            if (ret >= 0) return ret;
            hasCentroid &= -ret <= n / 2;
            size += -ret;
        }
        hasCentroid &= n - size <= n / 2;
        return hasCentroid ? v : -size;
    }

public:
    CentroidDecomposition(Graph *G) {
        this->G = G;
        exclude = new bool[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            exclude[v] = false;
        }
    }

    /**
     * Returns the centroid of the current subtree containing v, then divides the tree.
     *
     * @param v the vertex
     * @return the centroid of the current subtree containing v
     */
    int getCentroid(int v) {
        int c = getCentroid(v, -1, getSize(v, -1));
        exclude[c] = true;
        return c;
    }
};

#endif /* ALGORITHMS_GRAPH_SEARCH_CENTROIDDECOMPOSITION_H_ */
