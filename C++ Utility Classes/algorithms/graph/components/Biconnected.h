/*
 * Biconnected.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_COMPONENTS_BICONNECTED_H_
#define ALGORITHMS_GRAPH_COMPONENTS_BICONNECTED_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 *  Identifies articulation points and bridges.
 *  This can be used to decompose a graph into biconnected components.
 *  Runs in O(E + V) time.
 */
class Biconnected {
private:
    int bridges;
    int *low;
    int *pre;
    vector<vector<pair<int, int>>> components;
    int cnt;
    bool *articulation;
    unordered_set<int> *bridgeList;  // adjacency bridge list
    stack<pair<int, int>> s;

    void dfs(Graph *G, int u, int v) {
        int children = 0;
        pre[v] = cnt++;
        low[v] = pre[v];
        for (int w : G->adj(v)) {
            if (pre[w] == -1) {
                children++;
                s.push(make_pair(v, w));
                dfs(G, v, w);
                // update low number
                low[v] = min(low[v], low[w]);
                // non-root of DFS is an articulation point if low[w] >= pre[v]
                if (low[w] >= pre[v] && u != v) {
                    articulation[v] = true;
                    components.push_back(vector<pair<int, int>>());
                    while (s.top().first != u && s.top().second != v) {
                        components.back().push_back(make_pair(s.top().first, s.top().second));
                        s.pop();
                    }
                    components.back().push_back(make_pair(s.top().first, s.top().second));
                    s.pop();
                }
                if (low[w] == pre[w]) {
                    bridgeList[v].insert(w);
                    bridges++;
                }
            }
            // update low number - ignore reverse of edge leading to v
            else if (w != u) low[v] = min(low[v], pre[w]);
        }
        // root of DFS is an articulation point if it has more than 1 child
        if (u == v && children > 1) articulation[v] = true;
    }

public:
    Biconnected(Graph *G) {
        bridgeList = new unordered_set<int>[G->getV()];
        bridges = 0;
        cnt = 0;
        low = new int[G->getV()];
        pre = new int[G->getV()];
        articulation = new bool[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            low[v] = -1;
            pre[v] = -1;
            articulation[v] = false;
        }
        for (int v = 0; v < G->getV(); v++) {
            if (pre[v] == -1) dfs(G, v, v);
        }
    }

    // is vertex v an articulation point?
    bool isArticulation(int v) {
        return articulation[v];
    }

    vector<vector<int>> &getComponents() {
        return components;
    }

    int countComponents() {
        return components.size();
    }

    int countBridges() {
        return bridges;
    }

    bool isBridge(int v, int w) {
        return bridgeList[v].count(w);
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_BICONNECTED_H_ */
