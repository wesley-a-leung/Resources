#ifndef ALGORITHMS_GRAPH_MATCHING_HOPCROFTKARPMAXMATCHCOMPACT_H_
#define ALGORITHMS_GRAPH_MATCHING_HOPCROFTKARPMAXMATCHCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

class HopcroftKarpMaxMatchCompact {
private:
    int N;
    vector<vector<int>> adj;
    vector<bool> color;
    vector<int> mate;
    vector<int> dist;
    vector<int> typeA;
    int pathDist = INT_MAX;

    bool hasPath() {
        queue<int> q;
        for (int v : typeA) {
            if (mate[v] == -1) {
                dist[v] = 0;
                q.push(v);
            } else {
                dist[v] = INT_MAX;
            }
        }
        pathDist = INT_MAX;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int w : adj[v]) {
                if (mate[w] == -1) {
                    if (pathDist == INT_MAX) pathDist = dist[v] + 1;
                } else if (dist[mate[w]] == INT_MAX) {
                    dist[mate[w]] = dist[v] + 1;
                    if (pathDist == INT_MAX) q.push(mate[w]);
                }
            }
        }
        return pathDist != INT_MAX;
    }

    bool dfs(int v) {
        for (int w : adj[v]) {
            if (mate[w] == -1) {
                if (pathDist == dist[v] + 1) {
                    mate[w] = v;
                    mate[v] = w;
                    return true;
                }
            } else if (dist[mate[w]] == dist[v] + 1) {
                if (dfs(mate[w])) {
                    mate[w] = v;
                    mate[v] = w;
                    return true;
                }
            }
        }
        dist[v] = INT_MAX;
        return false;
    }

public:
    HopcroftKarpMaxMatchCompact(int N, vector<bool> &color) : N(N), adj(N), color(color), mate(N), dist(N) {
        fill(mate.begin(), mate.end(), -1);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    int maxMatch() {
        int cardinality = 0;
        for (int v = 0; v < N; v++) if (color[v]) typeA.push_back(v);
        while (hasPath()) for (int v : typeA) if (mate[v] == -1 && dfs(v)) cardinality++;
        return cardinality;
    }

    int getMate(int v) {
        return mate[v];
    }
};

#endif /* ALGORITHMS_GRAPH_MATCHING_HOPCROFTKARPMAXMATCHCOMPACT_H_ */
