#ifndef ALGORITHMS_GRAPH_COMPONENTS_DYNAMICBICONNECTIVITY_H_
#define ALGORITHMS_GRAPH_COMPONENTS_DYNAMICBICONNECTIVITY_H_

#include <bits/stdc++.h>
using namespace std;

class DynamicBiconnectivity {
private:
    struct Query {
        int type, v, w;
    };

    int V;
    vector<vector<int>> adj;
    vector<int> UF, parent, depth;
    vector<Query> queries;
    vector<int> ans;
    
    int find(int v) {
        return UF[v] < 0 ? v : UF[v] = find(UF[v]);
    }
    
    bool join(int v, int w) {
        v = find(v);
        w = find(w);
        if (v == w) return false;
        if (UF[v] > UF[w]) swap(v, w);
        UF[v] += UF[w];
        UF[w] = v;
        return true;
    }
    
    void dfs(int v, int prev, int dep) {
        depth[v] = dep;
        parent[v] = prev;
        for (int w : adj[v]) if (w != prev) dfs(w, v, dep + 1);
    }
    
public:
    DynamicBiconnectivity(int V): V(V), adj(V), UF(V, -1), parent(V, -1), depth(V, -1) {}

    void addEdge(int v, int w) {
        if (join(v, w)) {
            adj[v].push_back(w);
            adj[w].push_back(v);
            queries.push_back({1, v, w});
        } else {
            queries.push_back({2, v, w});
        }
    }
    
    void query() {
        queries.push_back({0, -1, -1});
    }
    
    vector<int> solve() {
        for (int v = 0; v < V; v++) if (parent[v] == -1) dfs(v, -1, 0);
        fill(UF.begin(), UF.end(), -1);
        int cnt = 0;
        for (auto &&q : queries) {
            if (q.type == 0) ans.push_back(cnt);
            else if (q.type == 1) cnt++;
            else {
                int a = q.v, b = q.w;
                while ((a = find(a)) != (b = find(b))) {
                    if (depth[a] < depth[b]) swap(a, b);
                    UF[a] = find(parent[a]);
                    a = parent[a];
                    cnt--;
                }
            }
        }
        return ans;
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_DYNAMICBICONNECTIVITY_H_ */
