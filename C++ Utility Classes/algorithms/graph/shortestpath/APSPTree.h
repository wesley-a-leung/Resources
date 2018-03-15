#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_APSPTREE_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_APSPTREE_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"
#include "algorithms/graph/lca/WeightedLCA.h"

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();
// rules for INF (infinity): (length of the longest path + length of greatest weight) < INF

/**
 * The {@code APSPTree} class represents a data type for solving the all-pairs shortest
 * paths problem in tree. The edge weights can be positive, negative, or zero.
 *
 * This implementation uses a lowest common ancestor algorithm that performs queries
 * in log <em>V</em> time.
 *
 * The constructor takes time proportional to <em>V</em> + <em>E</em>. Afterwards, the
 * {@code dist()} takes time proportional to log <em>V</em>.
 *
 * @author Wesley Leung
 *
 */
class APSPTree {
private:
    unit *distTo; // stores the distance from an arbitrary root
    WeightedLCA *LCA;

    void bfs(WeightedGraph *G, int s) {
        queue<int> q;
        bool marked[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            marked[v] = false;
            distTo[v] = INF;
        }
        distTo[s] = 0;
        marked[s] = true;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (WeightedEdge *e: G->adj(v)) {
                int w = e->other(v);
                if (!marked[w]) {
                    distTo[w] = distTo[v] + e->getWeight();
                    marked[w] = true;
                    q.push(w);
                }
            }
        }
    }

public:
    APSPTree(WeightedGraph *G) {
        distTo = new unit[G->getV()];
        LCA = new WeightedLCA(G);
        bfs(G, 0);
    }

    /**
     * Returns the length of a shortest path from vertex {@code s} to vertex {@code t}.
     * @param  s the source vertex
     * @param  t the destination vertex
     * @return the length of a shortest path from vertex {@code s} to vertex {@code t};
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException unless {@code 0 <= t < V}
     */
    unit dist(int s, int t) {
        return distTo[s] + distTo[t] - 2 * distTo[LCA->lca(s, t)];
    }
};

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_APSPTREE_H_ */
