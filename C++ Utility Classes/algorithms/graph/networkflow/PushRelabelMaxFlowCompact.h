#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOWCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOWCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

typedef int unit;
const unit INF = (1 << 30);
const unit EPS = 0;

class PushRelabelMaxFlowCompact {
private:
    struct Edge {
        int to;
        unit cap;
        int next;
        Edge(int to, unit cap, int next) : to(to), cap(cap), next(next) {}
    };

    int V;
    vector<int> last;
    vector<Edge> e;
    vector<int> height;
    vector<unit> excess;
    vector<bool> done;

public:
    PushRelabelMaxFlowCompact(int V) : V(V), last(V, -1), height(V, 0), excess(V, 0), done(V, false) {}

    void addEdge(int v, int w, unit vw, unit wv = 0) {
        e.emplace_back(w, vw, last[v]);
        last[v] = ((int) e.size()) - 1;
        e.emplace_back(v, wv, last[w]);
        last[w] = ((int) e.size()) - 1;
    }

    unit getFlow(int s, int t)  {
        if (s == t) throw invalid_argument("Source equals sink");
        queue<int> q;
        height[s] = V;
        for (int i = last[s]; i != -1; i = e[i].next) {
            int v = e[i].to;
            excess[s] -= e[i].cap;
            excess[v] += e[i].cap;
            e[i ^ 1].cap = e[i].cap;
            e[i].cap = 0;
            if (v != t) {
                done[v] = true;
                q.push(v);
            }
        }
        int cnt = 0;
        unit lastEx = -1;
        while (!q.empty()) {
            int v = q.front();
            int tempHeight = 2 * V;
            for (int i = last[v]; i != -1; i = e[i].next) {
                if (abs(excess[v]) <= EPS) break;
                int w = e[i].to;
                if (e[i].cap > EPS) {
                    if (height[v] > height[w]) {
                        unit tempFlow = min(excess[v], e[i].cap);
                        e[i].cap -= tempFlow;
                        e[i ^ 1].cap += tempFlow;
                        excess[v] -= tempFlow;
                        excess[w] += tempFlow;
                        if (!done[w] && w != s && w != t) {
                            done[w] = true;
                            q.push(w);
                        }
                    } else if (height[w] < tempHeight) {
                        tempHeight = height[w];
                    }
                }
            }
            if (excess[v] > EPS) height[v] = tempHeight + 1;
            else {
                done[v] = false;
                q.pop();
            }
            if (lastEx == excess[t]) {
                if (++cnt > 2 * V) break;
            } else {
                lastEx = excess[t];
                cnt = 0;
            }
        }
        return excess[t];
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOWCOMPACT_H_ */
