#ifndef ALGORITHMS_GRAPH_LCA_LCA_DP_H_
#define ALGORITHMS_GRAPH_LCA_LCA_DP_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"
using namespace std;

/**
 * The {@code LCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in a tree using dynamic programming.
 * The constructor takes time proportional to V + E and afterwards,
 * the {@code lca()} method takes time proportional to log(V) to find the lowest common ancestor.
 */
class LCA {
private:
    int V, LGV;
    int *depth, **parent;

    void dfs(Graph *G, int v, int d, int prev) {
        depth[v] = d;
        parent[v][0] = prev;
        for (int w : G->adj(v)) {
            if (w != prev) dfs(G, w, d + 1, v);
        }
    }

public:
    LCA(Graph *G) {
        V = G->getV();
        LGV = (int) (ceil(log2(V)) + 1);
        depth = new int[V];
        parent = new int*[V];
        for (int i = 0; i < V; i++) {
            parent[i] = new int[LGV];
            for (int j = 0; j < LGV; j++) {
                parent[i][j] = -1;
            }
        }
        dfs(G, 0, 0, -1);
        for (int j = 1; j < LGV; j++) {
            for (int i = 0; i < V; i++) {
                if (parent[i][j - 1] != -1) parent[i][j] = parent[parent[i][j - 1]][j - 1];
            }
        }
    }

    /**
     * Returns the lowest common ancestor of vertex {@code v} and {@code w}.
     *
     * @param v the first vertex
     * @param w the first vertex
     * @return the lowest common ancestor of vertex {@code v} and {@code w}
     */
    int lca(int v, int w) {
        if (depth[v] < depth[w]) swap(v, w);
        for (int i = LGV - 1; i >= 0; i--) {
            if (parent[v][i] != -1 && depth[parent[v][i]] >= depth[w]) v = parent[v][i];
        }
        if (v == w) return v;
        for (int i = LGV - 1; i >= 0; i--) {
            if (parent[v][i] != parent[w][i]) {
                v = parent[v][i];
                w = parent[w][i];
            }
        }
        return parent[v][0];
    }
};

#endif /* ALGORITHMS_GRAPH_LCA_LCA_DP_H_ */
