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
    vector<bool> vis;
    vector<int> dist;
    vector<int> cur;

    inline bool isMatched(int v) {
        return mate[v] != -1;
    }

    bool isResidual(int v, int w) {
        if ((mate[v] != w) && color[v]) return true;
        if ((mate[v] == w) && !color[v]) return true;
        return false;
    }

    bool isLevel(int v, int w) {
        return dist[w] == dist[v] + 1 && isResidual(v, w);
    }

    bool hasPath() {
        for (int v = 0; v < N; v++) {
            vis[v] = false;
            dist[v] = INT_MAX;
        }
        queue<int> q;
        for (int v = 0; v < N; v++) {
            if (color[v] && !isMatched(v)) {
                q.push(v);
                vis[v] = true;
                dist[v] = 0;
            }
        }
        bool hasPath = false;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int w : adj[v]) {
                if (isResidual(v, w)) {
                    if (!vis[w]) {
                        dist[w] = dist[v] + 1;
                        vis[w] = true;
                        if (!isMatched(w)) hasPath = true;
                        if (!hasPath) q.push(w);
                    }
                }
            }
        }
        return hasPath;
    }

public:
    HopcroftKarpMaxMatchCompact(int N, vector<bool> &color) : N(N), adj(N), color(color), mate(N), vis(N), dist(N), cur(N) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    int maxMatch() {
        int cardinality = 0;
        for (int v = 0; v < N; v++) mate[v] = -1;
        while (hasPath()) {
            for (int v = 0; v < N; v++) cur[v] = 0;
            for (int s = 0; s < N; s++) {
                if (isMatched(s) || !color[s]) continue;
                stack<int> path;
                path.push(s);
                while (!path.empty()) {
                    int v = path.top();
                    if (cur[v] >= (int) adj[v].size()) path.pop();
                    else {
                        int w = adj[v][cur[v]++];
                        if (!isLevel(v, w)) continue;
                        path.push(w);
                        if (!isMatched(w)) {
                            while (!path.empty()) {
                                int x = path.top();
                                path.pop();
                                int y = path.top();
                                path.pop();
                                mate[x] = y;
                                mate[y] = x;
                            }
                            cardinality++;
                        }
                    }
                }
            }
        }
        return cardinality;
    }
};

#endif /* ALGORITHMS_GRAPH_MATCHING_HOPCROFTKARPMAXMATCHCOMPACT_H_ */
