#ifndef ALGORITHMS_GRAPH_LCA_LCA_EULER_H_
#define ALGORITHMS_GRAPH_LCA_LCA_EULER_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"
using namespace std;

/**
 * The {@code LCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in a tree using and Euler tour.
 * The constructor takes time proportional to V + E and afterwards,
 * the {@code lca()} method takes time proportional to log(V) to find the lowest common ancestor.
 */
class LCA {
private:
    int *to, *id, *vertex, *head, *order;
    int sz, cnt, num;

    void dfs(Graph *G, int v, int prev) {
        int cur = num++;
        id[v] = cur;
        vertex[cur] = v;
        for (int w : G->adj(v)) {
            if (w == prev) continue;
            order[sz + cnt++] = cur;
            if (head[cur] == -1) head[cur] = cnt - 1;
            dfs(G, w, v);
        }
        order[sz + cnt++] = cur;
        if (head[cur] == -1) head[cur] = cnt - 1;
    }

public:
    LCA(Graph *G) {
        to = new int[G->getV()];
        id = new int[G->getV()];
        vertex = new int[G->getV()];
        head = new int[G->getV()];
        sz = G->getV() * 2 - 1;
        order = new int[sz * 2];
        num = 0;
        cnt = 0;
        for (int i = 0; i < G->getV(); i++) {
            head[i] = -1;
        }
        dfs(G, 0, -1);
        for (int i = 2 * sz - 2; i > 1; i -= 2) {
            order[i / 2] = min(order[i], order[i ^ 1]);
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
        int lo = min(head[id[v]], head[id[w]]);
        int hi = max(head[id[v]], head[id[w]]);
        int ans = INT_MAX;
        for (lo += sz, hi += sz; lo <= hi; lo = (lo + 1) / 2, hi = (hi - 1) / 2) {
            ans = min(ans, min(order[lo], order[hi]));
        }
        return vertex[ans];
    }
};

#endif /* ALGORITHMS_GRAPH_LCA_LCA_EULER_H_ */
