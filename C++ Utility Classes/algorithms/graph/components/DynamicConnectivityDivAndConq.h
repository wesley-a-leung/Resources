#ifndef ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYDIVANDCONQ_H_
#define ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYDIVANDCONQ_H_

#include <bits/stdc++.h>
using namespace std;

class DynamicConnectivityDivAndConq {
private:
    struct Query {
        int type, v, w, otherTime;
    };

    int V, curTime, cnt;
    vector<int> UF;
    vector<Query> queries;
    vector<int> ans;
    vector<unordered_map<int, int>> presentEdges;
    stack<pair<pair<int, int>, int>> history;

    int find(int v) {
        while (UF[v] >= 0) v = UF[v];
        return v;
    }

    void join(int v, int w) {
        v = find(v);
        w = find(w);
        if (v == w) return;
        if (UF[v] > UF[w]) swap(v, w);
        history.push({{v, w}, UF[w]});
        UF[v] += UF[w];
        UF[w] = v;
        cnt--;
    }

    void undo() {
        int v = history.top().first.first, w = history.top().first.second, ufw = history.top().second;
        history.pop();
        UF[w] = ufw;
        UF[v] -= UF[w];
        cnt++;
    }
    
    void solve(int l, int r) {
        if (l == r && queries[l].type == 0) ans.push_back(cnt);
        if (l >= r) return;
        int m = l + (r - l) / 2;
        int curSize = history.size();
        for (int i = m + 1; i <= r; i++) if (queries[i].otherTime < l) join(queries[i].v, queries[i].w);
        solve(l, m);
        while ((int) history.size() > curSize) undo();
        for (int i = l; i <= m; i++) if (queries[i].otherTime > r) join(queries[i].v, queries[i].w);
        solve(m + 1, r);
        while ((int) history.size() > curSize) undo();
    }
    
public:
    DynamicConnectivityDivAndConq(int V): V(V), curTime(0), cnt(V), UF(V, -1), presentEdges(V) {}

    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        presentEdges[v][w] = curTime++;
        queries.push_back({1, v, w, INT_MAX});
    }

    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = presentEdges[v][w];
        queries.push_back({-1, v, w, insTime});
        queries[insTime].otherTime = curTime++;
        presentEdges[v].erase(w);
    }

    void query() {
        queries.push_back({0, -1, -1, curTime++});
    }

    vector<int> solve() {
        solve(0, curTime - 1);
        return ans;
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_DYNAMICCONNECTIVITYDIVANDCONQ_H_ */
