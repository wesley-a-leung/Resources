#ifndef ALGORITHMS_GRAPH_COMPONENTS_BICONNECTED_H_
#define ALGORITHMS_GRAPH_COMPONENTS_BICONNECTED_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"

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
    vector<unordered_set<int>> components;
    int cnt;
    bool *articulation;
    unordered_set<int> *bridgeList;  // adjacency bridge list
    stack<pair<int, int>> s;

    void dfs(Graph *G, int v, int prev) {
        int children = 0;
        pre[v] = low[v] = cnt++;
        for (int w : G->adj(v)) {
            if (pre[w] == -1) {
                children++;
                s.push(make_pair(v, w));
                dfs(G, w, v);
                // update low number
                low[v] = min(low[v], low[w]);
                // non-root of DFS is an articulation point if low[w] >= pre[v]
                if ((prev == v && children > 1) || (prev != v && low[w] >= pre[v])) {
                    articulation[v] = true;
                    components.push_back(unordered_set<int>());
                    while (s.top().first != v || s.top().second != w) {
                        components.back().insert(s.top().first);
                        components.back().insert(s.top().second);
                        s.pop();
                    }
                    components.back().insert(s.top().first);
                    components.back().insert(s.top().second);
                    s.pop();
                }
                if (low[w] == pre[w]) {
                    bridgeList[v].insert(w);
                    bridges++;
                }
            }
            // update low number - ignore reverse of edge leading to v
            else if (w != prev && pre[w] < low[v]) {
                low[v] = pre[w];
                s.push(make_pair(v, w));
            }
        }
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
            if (!s.empty()) {
                components.push_back(unordered_set<int>());
                while (!s.empty()) {
                    components.back().insert(s.top().first);
                    components.back().insert(s.top().second);
                    s.pop();
                }
            }
        }
    }

    // is vertex v an articulation point?
    bool isArticulation(int v) {
        return articulation[v];
    }

    vector<unordered_set<int>> &getComponents() {
        return components;
    }

    unordered_set<int> &getComponent(int id) {
        return components[id];
    }

    int getSize(int id) {
        return components[id].size();
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
