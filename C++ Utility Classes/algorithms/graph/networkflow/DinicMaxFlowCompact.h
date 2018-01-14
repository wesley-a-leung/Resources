#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

typedef int unit;
const unit INF = 0x3f3f3f3f;
const unit EPS = 0;

class DinicMaxFlow {
private:
    struct Edge {
        int dest;
        unit cost;
        int next;
        Edge(int dest, unit cost, int next) : dest(dest), cost(cost), next(next) {}
    };

    int N, s = 0, t = 0;
    vector<int> last, level;
    vector<Edge> e;

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i = last[v]; i != -1; i = e[i].next) {
                if (e[i].cost > 0 && level[e[i].dest] == -1) {
                    level[e[i].dest] = level[v] + 1;
                    q.push(e[i].dest);
                }
            }
        }
        return level[t] != -1;
    }

    unit dfs(int v, unit flow) {
        if (v == t) return flow;
        unit ret = 0;
        for (int i = last[v]; i != -1; i = e[i].next) {
            if (e[i].cost > 0 && level[e[i].dest] == level[v] + 1) {
                unit res = dfs(e[i].dest, min(flow, e[i].cost));
                ret += res;
                e[i].cost -= res;
                e[i ^ 1].cost += res;
                flow -= res;
                if (abs(flow) <= EPS) break;
            }
        }
        return ret;
    }

public:
    DinicMaxFlow(int N) : N(N), last(N), level(N) {
        fill(last.begin(), last.end(), -1);
    }

    void addEdge(int v, int w, int vw, int wv = 0) {
        e.push_back(Edge(w, vw, last[v]));
        last[v] = ((int) e.size()) - 1;
        e.push_back(Edge(v, wv, last[w]));
        last[w] = ((int) e.size()) - 1;
    }

    unit getFlow(int src, int sink) {
        s = src;
        t = sink;
        unit res = 0;
        while (bfs()) res += dfs(s, INF);
        return res;
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOWCOMPACT_H_ */
