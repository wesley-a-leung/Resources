#ifndef ALGORITHMS_GRAPH_QUERIES_DSUTREE_H_
#define ALGORITHMS_GRAPH_QUERIES_DSUTREE_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"
using namespace std;

/**
 * The {@code DSUTree} class represents a data structure for computing queries on graphs that involve
 * finding the number of nodes in a subtree with a certain property.
 *
 * Time Complexity: O(N log N + Q)
 */
class DSUTree {
private:
    vector<pair<int, int>> *queries;
    int *ans;
    int *color;
    int *size;
    int *cnt;
    bool *isHeavy;

    void getSize(Graph *G, int v, int prev) {
        size[v] = 1;
        for (int w : G->adj(v)) {
            if (w == prev) continue;
            getSize(G, w, v);
            size[w] += size[v];
        }
    }

    void add(Graph *G, int v, int prev, int delta) {
        cnt[color[v]] += delta;
        for (int w : G->adj(v)) if (w != prev && !isHeavy[w]) add(G, w, v, delta);
    }

    void dfs(Graph *G, int v, int prev, bool keep = 0) {
        int maxSize = -1, heavyInd = -1;
        for (int w : G->adj(v)) {
            if (w != prev && size[w] > maxSize) {
                maxSize = size[w];
                heavyInd = w;
            }
        }
        for (int w : G->adj(v)) if (w != prev && w != heavyInd) dfs(G, w, v, 0);
        if (heavyInd != -1) {
            dfs(G, heavyInd, v, 1);
            isHeavy[heavyInd] = 1;
        }
        add(G, v, prev, 1);
        for (pair<int, int> q : queries[v]) ans[q.second] = cnt[q.first];
        if (heavyInd != -1) isHeavy[heavyInd] = 0;
        if (!keep) add(G, v, prev, -1);
    }

public:
    DSUTree(Graph *G, int Q, int *queries, int numColors, int *color) {
        cnt = new int[numColors + 1];
        for (int i = 0; i <= numColors; i++) cnt[i] = 0;
        int V = G->getV();
        this->color = new int[V];
        for (int i = 0; i < V; i++) this->color[i] = color[i];
        this->isHeavy = new bool[V];
        for (int i = 0; i < V; i++) isHeavy[i] = false;
        this->size = new int[V];
        getSize(G, 0, -1);
        ans = new int[Q];
        this->queries = new vector<pair<int, int>>[Q];
        for (int i = 0; i < Q; i++) this->queries[queries[i]].push_back(make_pair(queries[i], i));
        dfs(G, 0, -1, 0);
    }

    int getAnswer(int i) {
        return ans[i];
    }
};

#endif /* ALGORITHMS_GRAPH_QUERIES_DSUTREE_H_ */
