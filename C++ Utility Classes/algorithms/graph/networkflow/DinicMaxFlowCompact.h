#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

typedef int unit;
const unit INF = (1 << 30);
const unit EPS = 0;

// O(V ^ 2 * E) in the worst case, O(min(V ^ (2 / 3), E ^ (1 / 2)) * E) for unit values
class DinicMaxFlow {
private:
    struct Edge {
        int to;
        unit cap, origCap;
        int next;
        Edge(int to, unit cap, int next) : to(to), cap(cap), origCap(cap), next(next) {}
    };

    int N, s = 0, t = 0;
    vector<int> last, level;
    vector<Edge> e;
    vector<bool> cut;

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i = last[v]; i != -1; i = e[i].next) {
                if (e[i].cap > EPS && level[e[i].to] == -1) {
                    level[e[i].to] = level[v] + 1;
                    q.push(e[i].to);
                }
            }
        }
        return level[t] != -1;
    }

    unit dfs(int v, unit flow) {
        if (v == t) return flow;
        unit ret = 0;
        for (int i = last[v]; i != -1; i = e[i].next) {
            if (e[i].cap > EPS && level[e[i].to] == level[v] + 1) {
                unit res = dfs(e[i].to, min(flow, e[i].cap));
                ret += res;
                e[i].cap -= res;
                e[i ^ 1].cap += res;
                flow -= res;
                if (abs(flow) <= EPS) break;
            }
        }
        return ret;
    }

    void inferMinCutDfs(int v) {
        cut[v] = true;
        for (int i = last[v]; i != -1; i = e[i].next) {
            if (e[i].cap > 0 && !cut[e[i].to]) inferMinCutDfs(e[i].to);
        }
    }

public:
    DinicMaxFlow(int N) : N(N), last(N), level(N), cut(N) {
        fill(last.begin(), last.end(), -1);
        fill(cut.begin(), cut.end(), false);
    }

    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e.emplace_back(w, vw, last[v]);
        last[v] = ((int) e.size()) - 1;
        e.emplace_back(v, wv, last[w]);
        last[w] = ((int) e.size()) - 1;
    }

    unit getFlow(int src, int sink) {
        s = src;
        t = sink;
        unit res = 0;
        while (bfs()) res += dfs(s, INF);
        return res;
    }

    unit inferMinCut() {
        inferMinCutDfs(s);
        unit res = 0;
        for (int v = 0; v < N; v++) {
            if (!cut[v]) continue;
            for (int i = last[v]; i != -1; i = e[i].next) {
                if (cut[e[i].to]) continue;
                res += e[i].origCap;
            }
        }
        return res;
    }

    bool isCut(int v) {
        return cut[v];
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_ */
