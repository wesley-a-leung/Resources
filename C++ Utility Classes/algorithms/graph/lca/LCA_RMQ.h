#ifndef ALGORITHMS_GRAPH_LCA_LCA_RMQ_H_
#define ALGORITHMS_GRAPH_LCA_LCA_RMQ_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * The {@code LCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in a tree using range minimum queries.
 * The constructor takes time proportional to V log V + E and afterwards,
 * the {@code lca()} method takes time constant time to find the lowest common ancestor.
 */
class LCA {
private:
    vector<vector<int>> adj;
    vector<int> head, depth;
    vector<vector<int>> rmq;
    int ind;
    int lg;

    void dfs(int v, int prev) {
        head[v] = ind;
        rmq[0][ind++] = v;
        for (int w : adj[v]) {
            if (w == prev) continue;
            depth[w] = depth[v] + 1;
            dfs(w, v);
            rmq[0][ind++] = v;
        }
    }

    inline int minDepth(int v, int w) {
        return depth[v] < depth[w] ? v : w;
    }

    inline int RMQ(int l, int r) {
        int i = 31 - __builtin_clz(r - l + 1);
        return minDepth(rmq[i][l], rmq[i][r - (1 << i) + 1]);
    }

public:
    /**
     * Initializes the structure.
     *
     * @param V the number of vertices
     */
    LCA(int V) : adj(V), head(V), depth(V) {
        lg = 32 - __builtin_clz(V * 2 - 1);
        for (int i = 0; i < lg; i++) rmq.push_back(vector<int>(V * 2 - 1));
        fill(head.begin(), head.end(), -1);
        ind = 0;
    }

    /**
     * Adds a bidirectional edge from v to w.
     *
     * @param v one vertex of the edge
     * @param w the other vertex of the edge
     */
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    /**
     * Begins the LCA process.
     *
     * @param root the root of the tree
     */
    void compute(int root = 0) {
        depth[root] = 0;
        dfs(root, -1);
        for (int i = 0; i < lg - 1; i++) {
            for (int j = 0; j < ind; j++) {
                rmq[i + 1][j] = minDepth(rmq[i][j], rmq[i][min(j + (1 << i), ind - 1)]);
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
        if (head[v] > head[w]) swap(v, w);
        return RMQ(head[v], head[w]);
    }
};

#endif /* ALGORITHMS_GRAPH_LCA_LCA_RMQ_H_ */
