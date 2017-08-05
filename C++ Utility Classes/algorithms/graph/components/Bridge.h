/*
 * Bridge.h
 *
 *  Created on: Aug 1, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_COMPONENTS_BRIDGE_H_
#define ALGORITHMS_GRAPH_COMPONENTS_BRIDGE_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 *  Identifies bridge edges. This decomposes a directed graph into
 *  two-edge connected components.
 *  Runs in O(E + V) time.
 *
 *  Key quantity:  low[v] = minimum DFS preorder number of v
 *  and the set of vertices w for which there is a back edge (x, w)
 *  with x a descendant of v and w an ancestor of v.
 *
 *  Note: code assumes no parallel edges, e.g., two parallel edges
 *  would be (incorrectly) identified as bridges.
 */
class Bridge {
private:
    int bridges;      // number of bridges
    int cnt;          // counter
    int *pre;         // pre[v] = order in which dfs examines v
    int *low;         // low[v] = lowest preorder of any vertex connected to v
    unordered_set<int> *bridgeList;  // adjacency bridge list

    void dfs(Graph *G, int u, int v) {
        pre[v] = cnt++;
        low[v] = pre[v];
        for (int w : G->adj(v)) {
            if (pre[w] == -1) {
                dfs(G, v, w);
                low[v] = min(low[v], low[w]);
                if (low[w] == pre[w]) {
                    bridgeList[v].insert(w);
                    bridges++;
                }
            }

            // update low number - ignore reverse of edge leading to v
            else if (w != u)
                low[v] = min(low[v], pre[w]);
        }
    }

public:
    Bridge(Graph *G) {
        bridgeList = new unordered_set<int>[G->getV()];
        low = new int[G->getV()];
        pre = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            low[v] = -1;
            pre[v] = -1;
        }

        for (int v = 0; v < G->getV(); v++)
            if (pre[v] == -1)
                dfs(G, v, v);
    }

    int countBridges() { return bridges; }

    int components() { return bridges + 1; }

    bool isBridge(int v, int w) {
        return bridgeList[v].count(w);
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_BRIDGE_H_ */
