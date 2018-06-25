#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOWCOMPACT_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOWCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

typedef int unit;
const unit INF = (1 << 30);
const unit EPS = 0;

class EdmondsKarpMaxFlow {
private:
    struct Edge {
        int to;
        unit cap;
        int next;
        Edge(int to, unit cap, int next) : to(to), cap(cap), next(next) {}
    };

    int N, s = 0, t = 0;
    vector<int> edgeTo;
    vector<bool> marked;
    vector<Edge> e;
    vector<int> last;

    bool bfs() {
        fill(marked.begin(), marked.end(), false);
        fill(edgeTo.begin(), edgeTo.end(), -1);
        queue<int> q;
        q.push(s);
        marked[s] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i = last[v]; i != -1; i = e[i].next) {
                if (e[i].cap > EPS && !marked[e[i].to]) {
                    marked[e[i].to] = true;
                    edgeTo[e[i].to] = i;
                    q.push(e[i].to);
                }
            }
        }
        return marked[t];
    }

public:
    EdmondsKarpMaxFlow(int N) : N(N), edgeTo(N), marked(N), last(N, -1) {}

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
        while (bfs()) {
            unit bottle = INF;
            for (int v = t; v != s; v = e[edgeTo[v] ^ 1].to) {
                bottle = min(bottle, e[edgeTo[v]].cap);
            }
            for (int v = t; v != s; v = e[edgeTo[v] ^ 1].to) {
                e[edgeTo[v]].cap -= bottle;
                e[edgeTo[v] ^ 1].cap += bottle;
            }
            res += bottle;
        }
        return res;
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOWCOMPACT_H_ */
