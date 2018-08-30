#ifndef ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYSQRTDECOMP_H_
#define ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYSQRTDECOMP_H_

#include <bits/stdc++.h>
using namespace std;

class DynamicConnectivitySqrtDecomp {
private:
    struct Query {
        int type, v, w;
    };

    int V;
    vector<int> vis, root;
    vector<unordered_set<int>> adj, toRemove;
    vector<unordered_map<int, int>> adj2;
    vector<Query> queries;
    vector<int> ans;

    void dfs1(int v, int r) {
        root[v] = r;
        for (int w : adj[v]) if (root[w] == -1 && !toRemove[v].count(w)) dfs1(w, r);
    }

    bool dfs2(int v, int t, int i) {
        if (v == t) return true;
        vis[v] = i;
        for (pair<int, int> e : adj2[v]) if (vis[e.first] != i && dfs2(e.first, t, i)) return true;
        return false;
    }

public:
    DynamicConnectivitySqrtDecomp(int V): V(V), vis(V, -1), root(V, -1), adj(V), toRemove(V), adj2(V) {}

    void addEdge(int v, int w) {
        queries.push_back({1, v, w});
    }

    void removeEdge(int v, int w) {
        queries.push_back({-1, v, w});
    }

    void query() {
        queries.push_back({0, -1, -1});
    }

    vector<int> solve(int blockSize) {
        int cnt = V;
        for (int st = 0; st < (int) queries.size(); st += blockSize) {
            fill(root.begin(), root.end(), -1);
            for (int i = st; i < min(st + blockSize, (int) queries.size()); i++) {
                if (queries[i].type != -1) continue;
                toRemove[queries[i].v].insert(queries[i].w);
                toRemove[queries[i].w].insert(queries[i].v);
            }
            for (int v = 0; v < V; v++) {
                if (root[v] == -1) dfs1(v, v);
                adj2[v].clear();
            }
            for (int v = 0; v < V; v++) {
                for (int w : adj[v]) if (root[v] != root[w]) adj2[root[v]][root[w]]++;
                toRemove[v].clear();
            }
            for (int i = st; i < min(st + blockSize, (int) queries.size()); i++) {
                if (queries[i].type == 1) {
                    adj[queries[i].v].insert(queries[i].w);
                    adj[queries[i].w].insert(queries[i].v);
                    int rv = root[queries[i].v], rw = root[queries[i].w];
                    if (rv != rw) {
                        if (!dfs2(rv, rw, i)) cnt--;
                        adj2[rv][rw]++;
                        adj2[rw][rv]++;
                    }
                } else if (queries[i].type == -1) {
                    adj[queries[i].v].erase(queries[i].w);
                    adj[queries[i].w].erase(queries[i].v);
                    int rv = root[queries[i].v], rw = root[queries[i].w];
                    if (rv != rw) {
                        if (--adj2[rv][rw] == 0) adj2[rv].erase(rw);
                        if (--adj2[rw][rv] == 0) adj2[rw].erase(rv);
                        if (!dfs2(rv, rw, i)) cnt++;
                    }
                } else {
                    ans.push_back(cnt);
                }
            }
        }
        return ans;
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYSQRTDECOMP_H_ */
